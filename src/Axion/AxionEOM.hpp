#ifndef SYSTEM_AxionEOM
#define SYSTEM_AxionEOM
#include<iostream>
#include<fstream>
#include <cmath>
#include <string>
#include"Interpolation.hpp"

#include"src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"
#include"src/AnharmonicFactor/AnharmonicFactor.hpp"

namespace mimes{
    #define Neqs 2
    template<class LD> using  Array = LD[Neqs]; 

    template<class LD>
    class AxionEOM{  
        public:
        LD theta_i,fa,tmax,TSTOP,ratio_ini;
        LD T_stop, logH2_stop, t_stop;//temperature, logH^2, and t where we stop interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        LD T_ini, logH2_ini;//temperature and logH^2 (t=0 by definition) where we start interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        LD t_osc, T_osc;//temperature and logH^2 (t=0 by definition) where we start interpolation (the end of the file). They should be AFTER entropy conservation resumes (I usually stop any intergation at T<1 MeV or so, where the Universe expands in a standard way)!
        
        std::vector<LD> t_tab,T_tab,logH2_tab;
        CubicSpline<LD> T_int;
        CubicSpline<LD> logH2_int;

        AxionEOM(LD theta_i, LD fa, LD tmax, LD TSTOP, LD ratio_ini, std::string inputFile){
            this->theta_i=theta_i;
            this->fa=fa;

            this->tmax=tmax;
            this->TSTOP=TSTOP;
            this->ratio_ini=ratio_ini;


            LD t,T,logH;//current line in file
            LD t_prev,T_prev,logH_prev;//previous line in file

            std::ifstream data_file(inputFile);
            bool ini_check=true; //check when ratio_ini is reached
            bool osc_check=true; //check when T_osc is reached
            LD t_ini; //check when ratio_ini is reached in order to rescale t to start at 0 in the interpolations

            LD ratio;// I will use ratio to store 3H/m_a as I read the file
            while (not data_file.eof()){
                data_file>>t;
                data_file>>T;
                data_file>>logH;
                ratio = 3*std::exp(logH) / std::sqrt(axionMass.ma2(T,fa));

                //find T and t when oscillation begins.
                if(ratio<=1){if(osc_check){osc_check=false; t_osc=t_prev-t_ini;T_osc=T_prev;}}

                if(ratio <= ratio_ini ){
                    //we need the following check in order to find t_ini (it is better to start at the point before ratio_ini is reached) 
                    if(ini_check){  
                        T_ini=T_prev;
                        logH2_ini=2*logH_prev;

                        t_ini=t_prev;
                        t_tab.push_back(0);
                        T_tab.push_back(T_prev);
                        logH2_tab.push_back(2*logH_prev);

                        ini_check=false;
                    }
                    t_tab.push_back(t - t_ini);  
                    T_tab.push_back(T);  
                    logH2_tab.push_back(2*logH);  
                }

                t_prev=t;
                T_prev=T;
                logH_prev=logH;
            }
            t_stop=t - t_ini;
            T_stop=T;
            logH2_stop=2*logH;
        };


        void makeInt(){
            T_int=CubicSpline<LD>{&t_tab,&T_tab};
            logH2_int=CubicSpline<LD>{&t_tab,&logH2_tab};
        }

        LD Temperature(LD t){
            if(t<=0){return T_ini;}
            if(t>=t_stop){return T_stop;}
            return T_int(t);
        }

        LD logH2(LD t){
            if(t<=0){return logH2_ini;}
            if(t>=t_stop){return logH2_stop;}
            return logH2_int(t);
        }

        LD dlogH2dt(LD t){
            if(t<=0){return 0;}
            if(t>=t_stop){return 0;}
            return logH2_int.derivative_1(t); 
        }


        ~AxionEOM(){};


        void operator()(Array<LD> &lhs, Array<LD> &y, LD t){
            //remember that t=log{a/a_i}
            
            LD theta=y[0];
            LD zeta=y[1];

            // you need to load a file that contains u, T, logH, and interpolate T and logH^2
            LD _T= Temperature(t);//temperature
            LD _H2= std::exp(logH2(t)) ;// e^{log(H^2)}=H^2
            LD _dlogH2dt=dlogH2dt(t);// dlogH^2/dt


            lhs[0]=zeta;//dtheta/dt
            lhs[1]=-(0.5*_dlogH2dt +3)*zeta - axionMass.ma2(_T,fa)/_H2*std::sin(theta);//dzeta/dt

        };
    };
};

#endif