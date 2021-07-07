#ifndef Anharmonic_head
#define Anharmonic_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "Interpolation.hpp"


namespace mimes{

    template<class LD>
    class AnharmonicFactor {
        using VecLD=std::vector<LD>;
        protected:
        CubicSpline<LD> anharmonic_factor;
        VecLD theta_tab, anF_tab;
        LD max_Theta;

        public:

        AnharmonicFactor(std::string path){    
            
            unsigned int N=0;
            LD theta,anF;
            std::ifstream data_file(path,std::ios::in);

            //use this to prevent problems caused by empty lines...
            LD theta_prev=-1;
            while (not data_file.eof()){
                data_file>>theta;
                data_file>>anF;

                //if there is an empty line theta does not change, so do skip it.
                if(N>1 and theta==theta_prev){continue;}

                theta_tab.push_back(theta);  
                anF_tab.push_back(anF);         
                
                N++;
                theta_prev=theta;
            }
            data_file.close();
            max_Theta=theta_tab[N-1];

            anharmonic_factor=CubicSpline<LD>(&theta_tab,&anF_tab);
        }

        LD mod(LD x, LD y) {
            return x - (int)(x/y) * y;
        }

        LD operator()(LD theta_max){
            // the adiabatic invariant for theta_max (ie at a point with dtheta/dt=0)

            //theta_max is positive by definition
            if(theta_max<0){theta_max*=-1;}
        
            //if theta_max>pi, then wrap it to pi, since theta_max is defined to be smaller (or equal) 
            // than pi. 
            if (theta_max>M_PI){
                LD _tmp=mod(theta_max, M_PI);

                if(_tmp==0){theta_max=M_PI;}
                else{theta_max= _tmp;}
            }
            
            // if theta_max is between the maximum tabulated theta_max and \pi, 
            // you can just use theta_max=max_Theta, since max_Theta \approx pi. 
            // This may happen if one uses a more accurate  value for \pi than the one used in
            // the tabulated results.
            if(theta_max>=max_Theta){return anharmonic_factor(max_Theta);}

        
            
            return anharmonic_factor(theta_max);
        }

    };
};

#endif