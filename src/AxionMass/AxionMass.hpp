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
    template<class LD>
    class AxionMass {
        private:
        using VecLD=std::vector<LD>;
        using func=std::function<LD(LD,LD)>;
        
        CubicSpline<LD> chi;
        VecLD Ttab,chitab;
        func ma2_func;
        func dma2dT_func;
        // these are the functions you can use in order to get the mass and its derivative beyond the 
        // interpolation limits.
        // ma2_MAX and dma2dT_MAX denote the approximation for T>TMAX,
        // and  ma2_MIN and dma2dT_MIN the approximation for T<TMIN.  
        public:
        LD TMin, TMax, chiMax, chiMin;
        func ma2_MAX,ma2_MIN;
        func dma2dT_MAX,dma2dT_MIN;
        
        AxionMass(func ma2);    
        AxionMass(func ma2, func dma2dT);    
        AxionMass(std::string path, LD minT, LD maxT);

        LD ma2(LD T, LD fa){
            // axion mass squared at temperature T and f_\alpha=fa
            if(T>=TMax){return ma2_MAX(T,fa);}//use this beyond the upper limit
            if(T<=TMin){return ma2_MIN(T,fa);}//use this beyond the lower limit
            return ma2_func(T,fa);//interpolate chi if TMin!=TMax and T is between TMin nad TMax 
        }

        LD dma2dT(LD T, LD fa){
            // axion mass squared derivative
            if(T>=TMax){return dma2dT_MAX(T,fa);}
            if(T<=TMin){return dma2dT_MIN(T,fa);}
            return dma2dT_func(T,fa);
        }

    };



    /*
    The constructor of the class.
    path: a string with a valid path of the data file. The columns of the file must be:
                T [GeV] chi [GeV^4]
        Here chi is defined through m_a^2(T) = chi(T)/f_a^2, ie chi=m_a^2_{f_a=1 GeV}.

    minT: minimum temperature of interpolation. Below this temperature m_a assumed to be constant.
    maxT: maximum temperature of interpolation. Above this temperature m_a assumed to follow
            m_a^2(T)=chiMax/f_a^2*(T/TMax)^{-8.16},
        with chiMax=chi(TMax), and TMax the temberature closest to maxT.

    Note: If the file has only one point, the mass is assumed to be constant!

    */
    template<class LD>
    AxionMass<LD>::AxionMass(std::string path, LD minT, LD maxT){    
            
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
            chiMax=chitab[N-1];
            chiMin=chitab[0];

            this->chi=CubicSpline<LD>(&Ttab,&chitab);

            ma2_func=[this](LD T, LD fa){return this->chi(T)/fa/fa;};
            dma2dT_func=[this](LD T, LD fa){return this->chi.derivative_1(T)/fa/fa;};

            ma2_MAX=[this](LD T, LD fa){return this->chiMax/fa/fa*std::pow(T/this->TMax,-8.16);};
            ma2_MIN=[this](LD T, LD fa){return this->chiMin/fa/fa;};

            dma2dT_MAX=[this](LD T, LD fa){return -8.16*this->chiMax/fa/fa*std::pow(T/this->TMax,-8.16)/T;};
            dma2dT_MIN=[this](LD T, LD fa){return static_cast<LD>(0.);};
        }


    /*
    Constructor.
    
    LD ma2(LD T,LD fa): the axion mass squared, which is then accessed by AxionMass::ma2.
    The derivative wrt the temperature (AxionMass::dma2dT) is numerically approximated.
    Note: LD here is a macro for "double" of "long double"
    */
    template<class LD>
    AxionMass<LD>::AxionMass(func ma2){    
            // the mass of the axion squared
            this->ma2_func=ma2;

            // the derivative of ma2. The stepsize parameter, h, is optional and 
            // determines the stepsize of the numerical differentiation (usually h=1e-8 is enough).
            this->dma2dT_func=[this](LD T,LD fa, LD h = 1e-8){
                h=T*h+h;
                return (this->ma2(T+h,fa)-this->ma2(T-h,fa))/(2*h);
            };
        }

    /*
    Another constructor.
    
    LD ma2(LD T,LD fa): the axion mass squared, which is then accessed by AxionMass::ma2.
    LD dma2dT(LD T,LD fa): The derivative of ma2 wrt the temperature, which is then accessed by AxionMass::dma2dT.
    Note: LD here is a macro for "double" of "long double"
    */
    template<class LD>
    AxionMass<LD>::AxionMass(func ma2,func dma2dT){    
            this->ma2_func=ma2;
            this->dma2dT_func=dma2dT;
        }

};



#endif