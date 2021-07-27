#ifndef CHI_head
#define CHI_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "src/SimpleSplines/Interpolation.hpp"

namespace mimes{
    template<class LD>
    class AxionMass {
        private:
        using VecLD=std::vector<LD>;
        CubicSpline<LD> chi;
        VecLD Ttab,chitab;
        LD TMin, TMax, chiMin, chiMax;
        LD LambdaQCD;
        public:

        AxionMass(std::string path, LD minT=0, LD maxT=1e5){    
            
            unsigned int N=0;
            LD T,chi;
            
            std::ifstream data_file(path,std::ios::in);

            while (not data_file.eof()){
                data_file>>T;
                data_file>>chi;
                
                if(T>=minT and T<=maxT){
                    //if there is an empty line the temperature does not change, so do skip it.
 
                    Ttab.push_back(T*1e-3); //temperature in GeV
                    chitab.push_back(chi*0.197*0.197*0.197*0.197); //chi in GeV**4
                    
                    N++;
                }
            }
            data_file.close();

            TMin=Ttab[0];
            TMax=Ttab[N-1];
            chiMin=chitab[N-1];
            chiMax=chitab[0];

            this->chi=CubicSpline<LD>(&Ttab,&chitab);

            LambdaQCD=0.4;
        }

        LD ma2(LD T, LD fa){
            // axion mass squared at temperature T and f_\alpha=fa
            if(T>=TMax){return chiMin/fa/fa*std::pow(T/TMax,-8.16);}
            if(T<=TMin){return chiMax/fa/fa;}
            return chi(T)/fa/fa;
        }


        LD dma2dT(LD T, LD fa){
            // axion mass squared derivative
            if(T>=TMax){return -8.16*chiMin/fa/fa*std::pow(T/TMax,-9.16);}
            
            if(T<=TMin){return 0;}

            return chi.derivative_1(T)/fa/fa;
        }

        LD ma2_approx(LD T, LD fa){
            // axion mass squared at temperature T and f_\alpha=fa
            LD ma20=chiMax/fa/fa;
            LD b=4e-4;
            LD fT=std::pow(T/LambdaQCD,-8.16);
            if(b*fT<=1){return b*fT*ma20;}
            else{return ma20;}    
        
        }
        LD dma2dT_approx(LD T, LD fa){
            // axion mass squared at temperature T and f_\alpha=fa
            LD ma20=chiMax/fa/fa;
            LD b=4e-4;
            LD fT=std::pow(T/LambdaQCD,-8.16);
            if(b*fT<=1){return -8.16/(T/LambdaQCD)*b*fT*ma20;}
            else{return 0;}    
        
        }
    };
};

#endif