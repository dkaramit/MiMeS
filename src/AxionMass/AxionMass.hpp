#ifndef CHI_head
#define CHI_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>
#include<functional>
#include"src/Cosmo/Cosmo.hpp"

#include "src/SimpleSplines/Interpolation.hpp"

namespace mimes{
    template<class LD, bool useData=true>
    class AxionMass {
        private:
        using VecLD=std::vector<LD>;
        using func=std::function<LD(LD,LD)>;
        
        CubicSpline<LD> chi;
        VecLD Ttab,chitab;
        LD TMin, TMax, chiMin, chiMax;
        LD LambdaQCD;
        func ma2_MAX,ma2_MIN;
        public:
        /*
        The constructor of the class.
        path: a string with a valid path of the data file. The columns of the file must be:
                    T [GeV] chi [GeV^4]
            Here chi is defined through m_a^2(T) = chi(T)/f_a^2, ie chi=m_a^2_{f_a=1 GeV}.

        minT: minimum temperature of interpolation. Below this temperature m_a assumed to be constant.
        maxT: maximum temperature of interpolation. Above this temperature m_a assumed to follow
                m_a^2(T)=chiMin/f_a^2*(T/TMax)^{-8.16},
            with chiMin=chi(TMax), and TMax the temberature closest to maxT.

        Note: If the file has only one point, the mass is assumed to be constant!

        */
        AxionMass(std::string path, LD minT=0, LD maxT=Cosmo<LD>::mP){    
            
            unsigned int N=0;
            LD T,chi;
            
            std::ifstream data_file(path,std::ios::in);
            //quit if the file does not exist.
            if(not data_file.good()) {
                std::cerr << path << " does not exist.";
                std::cerr <<" Please make sure to provide a valid path for the AxionMass datafile";
                std::cerr <<" in MiMeS/Definitions.mk\n"; 
                exit(1);
            }

            while (not data_file.eof()){
                data_file>>T;
                data_file>>chi;
                
                if(T>=minT and T<=maxT){
                    Ttab.push_back(T); //temperature in GeV
                    chitab.push_back(chi); //chi in GeV**4
                    N++;
                }
            }
            data_file.close();

            TMin=Ttab[0];
            TMax=Ttab[N-1];
            chiMin=chitab[N-1];
            chiMax=chitab[0];

            // if TMin == TMax, the mass assumed to be constant
            if(TMin != TMax){ this->chi=CubicSpline<LD>(&Ttab,&chitab); }

            LambdaQCD=0.4;

            ma2_MAX=[this](LD T, LD fa){return this->chiMin/fa/fa*std::pow(T/this->TMax,-8.16);};
            ma2_MIN=[this](LD T, LD fa){return this->chiMax/fa/fa;};

        }

        void set_ma2_approx(
            func  ma2_MAX=[this](LD T, LD fa){return this->chiMin/fa/fa*std::pow(T/this->TMax,-8.16);}, 
            func ma2_MIN=[this](LD T, LD fa){return this->chiMax/fa/fa;}){
            this->ma2_MAX=ma2_MAX;
            this->ma2_MIN=ma2_MIN;
        }

        LD ma2(LD T, LD fa){
            // axion mass squared at temperature T and f_\alpha=fa
            if(T>=TMax){return ma2_MAX(T,fa);}//use this beyond the upper limit
            if(T<=TMin){return ma2_MIN(T,fa);}//use this beyond the lower limit
            return chi(T)/fa/fa;//interpolate chi if TMin!=TMax and T is between TMin nad TMax 
        }


        LD dma2dT(LD T, LD fa){
            if(TMin==TMax){return 0;}

            // axion mass squared derivative
            if(T>=TMax){return -8.16*chiMin/fa/fa*std::pow(T/TMax,-9.16);}
            
            if(T<=TMin){return 0;}

            return chi.derivative_1(T)/fa/fa;
        }

    };
};


namespace mimes{
    template<class LD>
    class AxionMass<LD, false> {
        private:
        using func=std::function<LD(LD,LD)>;
        public:
        func  ma2,dma2dT;
        /*
        Constructor.
        
        LD ma2(LD fa,LD T): the axion mass squared, which is then accessed by AxionMass::ma2.
        The derivative wrt the temperature (AxionMass::dma2dT) is numerically approximated.
        Note: LD here is a macro for "double" of "long double"
        */
        AxionMass(func ma2){    
            // the mass of the axion squared
            this->ma2=ma2;

            // the derivative of ma2. The stepsize parameter, h, is optional and 
            // determines the stepsize of the numerical differentiation (usually h=1e-8 is enough).
            this->dma2dT=[this](LD fa,LD T, LD h = 1e-8){
                h=T*h+h;
                return (this->ma2(fa,T+h)-this->ma2(fa,T-h))/(2*h);
            };
        }

        /*
        Another constructor.
        
        LD ma2(LD fa, LD T): the axion mass squared, which is then accessed by AxionMass::ma2.
        LD dma2dT(LD fa, LD T): The derivative of ma2 wrt the temperature, which is then accessed by AxionMass::dma2dT.
        Note: LD here is a macro for "double" of "long double"
        */
    
       
        AxionMass(func ma2,func dma2dT){    
            this->ma2=ma2;
            this->dma2dT=dma2dT;
        }
    };

};

#endif