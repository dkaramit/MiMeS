#ifndef Anharmonic_head
#define Anharmonic_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "src/SimpleSplines/Interpolation.hpp"


namespace mimes{

    template<class LD>
    class AnharmonicFactor {
        using VecLD=std::vector<LD>;
        protected:
        CubicSpline<LD> anharmonic_factor;
        VecLD theta_tab, anF_tab;
        LD maxTheta,minTheta;
        LD anFStart,anFEnd;

        public:

        AnharmonicFactor(std::string path){    
            
            unsigned int N=0;
            LD theta,anF;
            std::ifstream data_file(path,std::ios::in);
            //quit if the file does not exist.
            if(not data_file.good()) {
                std::cerr << path << " does not exist.";
                std::cerr <<" Please make sure to provide a valid path for the AnharmonicFactor datafile";
                std::cerr <<" in MiMeS/Definitions.mk\n"; 
                exit(1);
            }


            while (not data_file.eof()){
                data_file>>theta;
                data_file>>anF;

                theta_tab.push_back(theta);  
                anF_tab.push_back(anF);         
                
                N++;
            }
            data_file.close();
            maxTheta=theta_tab[N-1];
            minTheta=0;
            anFEnd=anF_tab[N-1];
            anFStart=anF_tab[0];

            anharmonic_factor=CubicSpline<LD>(&theta_tab,&anF_tab);
        }

        LD operator()(LD theta_peak){
            // the adiabatic invariant for theta_peak (ie at a point with dtheta/dt=0)

            if(theta_peak<=minTheta){return anFStart;}
            if(theta_peak>=maxTheta){return anFEnd;}
            return anharmonic_factor(theta_peak);
        }
        

        // these seem useless, but I'll keep them just in case
        LD mod(LD x, LD y) {
            return x - (int)(x/y) * y;
        }

        LD warped(LD theta_peak){
            /*
            The adiabatic invariant for theta_peak (ie at a point with dtheta/dt=0),
            but theta_peak is wraped between 0 and pi.
            */

            //theta_peak is positive by definition
            if(theta_peak<0){theta_peak*=-1;}
        
            //if theta_peak>pi, then wrap it to pi, since theta_peak is defined to be smaller (or equal) 
            // than pi. 
            if (theta_peak>M_PI){
                LD _tmp=mod(theta_peak, M_PI);
                if(_tmp==0){theta_peak=M_PI;}
                else{theta_peak= _tmp;}
            }
            
            // if theta_peak is between the maximum tabulated theta_peak and \pi, 
            // you can just use theta_peak=maxTheta, since maxTheta \approx pi. 
            // This may happen if one uses a more accurate  value for \pi than the one used in
            // the tabulated results.
            if(theta_peak>=maxTheta){return anharmonic_factor(maxTheta);}

            return anharmonic_factor(theta_peak);
        }

    };
};

#endif