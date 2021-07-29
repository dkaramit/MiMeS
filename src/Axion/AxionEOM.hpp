#ifndef SYSTEM_AxionEOM
#define SYSTEM_AxionEOM
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <string>
#include "src/SimpleSplines/Interpolation.hpp"

/*get static variables (includes cosmological parameters, axion mass, and anharmonic factor)*/
#include "src/static.hpp"

namespace mimes{
    constexpr unsigned int Neqs=2;
    template<class LD> using  Array = std::array<LD,Neqs>; 

    template<class LD>
    class AxionEOM{  
        public:
        LD fa,ratio_ini;
        LD T_stop, logH2_stop, u_stop;//temperature, logH^2, and u where we stop interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        LD T_ini, logH2_ini;//temperature and logH^2 (t=0 by definition) where we start interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        LD u_osc, T_osc;//temperature and logH^2 (t=0 by definition) where we start interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        
        std::vector<LD> u_tab,T_tab,logH2_tab;//these will store the data from inputFile
        CubicSpline<LD> T_int; //interpolation of the temperature
        CubicSpline<LD> logH2_int; //interpolation of logH^2

        AxionEOM()=default;
        ~AxionEOM()=default;

        // constructor of AxionEOM.
        /*
        fa: PQ scale in GeV (the temperature dependent mass is defined as m_a^2(T) = \chi(T)/f^2)
        ratio_ini: interpolations start when 3H/m_a<~ratio_ini
        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH
        */ 
        AxionEOM(LD fa, LD ratio_ini, std::string inputFile){

            this->fa=fa;
            this->ratio_ini=ratio_ini;

            LD u=0,T=0,logH=0;//current line in file
            LD u_prev=0,T_prev=0,logH_prev=0;//previous line in file

            std::ifstream data_file(inputFile,std::ios::in);

            //quit if the file does not exist.
            if(not data_file.good()){
                std::cerr << inputFile<< " does not exist.";
                std::cerr<<" Please make sure to provide a valid path.\n";
                exit(1);
            }

            bool ini_check=true; //check when ratio_ini is reached
            bool osc_check=true; //check when T_osc is reached
            LD u_ini=0; //check when ratio_ini is reached in order to rescale u to start at 0 in the interpolations

            LD ratio=0;// I will use ratio to store 3H/m_a as I read the file
            //read the file and fill u_tab, T_tab, and  logH2_tab; and find T_osc.

            unsigned int N=0;
            while (not data_file.eof()){
                data_file>>u;
                data_file>>T;
                data_file>>logH;
                
                //if there is an empty line, u does not change, so do skip this line :).
                if(N>1 and u==u_prev){continue;}

                ratio = 3*std::exp(logH) / std::sqrt(axionMass.ma2(T,fa));

                if(ratio <= ratio_ini ){
                    //we need the following check in order to find u_ini (it is better to start at the point before ratio_ini is reached) 
                    if(ini_check){  
                        T_ini=T_prev;
                        logH2_ini=2*logH_prev;

                        u_ini=u_prev;
                        u_tab.push_back(0);
                        T_tab.push_back(T_prev);
                        logH2_tab.push_back(2*logH_prev);

                        ini_check=false;
                    }
                    u_tab.push_back(u - u_ini);  
                    T_tab.push_back(T);  
                    logH2_tab.push_back(2*logH);  
                }
                //find T and u when oscillation begins.
                if(ratio<=1){if(osc_check){osc_check=false; u_osc=u_prev-u_ini;T_osc=T_prev;}}



                u_prev=u;
                T_prev=T;
                logH_prev=logH;
                N++;
            }
            u_stop=u - u_ini;
            T_stop=T;
            logH2_stop=2*logH;
            data_file.close();

        };

        // make the interpolations
        void makeInt(){
            T_int=CubicSpline<LD>{&u_tab,&T_tab};
            logH2_int=CubicSpline<LD>{&u_tab,&logH2_tab};
        }

        //the temperature at t. in order to avoid getting out of bounds, we take T to be constant for
        //t<0 and t>t_stop (not the best idea, but it works with the solver)
        LD Temperature(LD u){
            if(u<=0){return T_ini;}
            if(u>=u_stop){return T_stop;}
            return T_int(u);
        }

        //logH^2 at u. in order to avoid getting out of bounds, we take logH2 to be constant for
        //u<0 and u>u_stop (not the best idea, but it works with the solver)
        LD logH2(LD u){
            if(u<=0){return logH2_ini;}
            if(u>=u_stop){return logH2_stop;}
            return logH2_int(u);
        }

        //the temperature at u. in order to avoid getting out of bounds, we take it to be 0 for
        //t<0 and t>t_stop (not the best idea, but it works with the solver)
        LD dlogH2du(LD u){
            if(u<=0){return 0;}
            if(u>=u_stop){return 0;}
            return logH2_int.derivative_1(u); 
        }

        //overload operator() in order to call it from the solver
        void operator()(Array<LD> &lhs, Array<LD> &y, LD u){
            //remember that t=log{a/a_i}
            
            LD theta=y[0];
            LD zeta=y[1];

            // you need to load a file that contains u, T, logH, and interpolate T and logH^2
            LD _T= Temperature(u);//temperature
            LD _H2= std::exp(logH2(u)) ;// e^{log(H^2)}=H^2
            LD _dlogH2du=dlogH2du(u);// dlogH^2/du

            // in radiation dominated Universe, you can also use:
            // LD _dlogH2du=-(cosmo.dgeffdT(_T)*_T/cosmo.geff(_T) + 4 )/cosmo.dh(_T);


            lhs[0]=zeta;//dtheta/du
            lhs[1]=-(0.5*_dlogH2du +3)*zeta - axionMass.ma2(_T,fa)/_H2*std::sin(theta);//dzeta/du

        };
    };
};

#endif