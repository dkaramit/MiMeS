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
        // these are the functions you can use in order to get the mass and its derivative beyond the 
        // interpolation limits.
        // ma2_MAX and dma2dT_MAX denote the approximation for T>TMAX,
        // and  ma2_MIN and dma2dT_MIN the approximation for T<TMIN.  
        LD TMin, TMax, chiMax, chiMin;
        func ma2_MAX,ma2_MIN;
        public:
        func ma2;

        AxionMass()=default;
            
        AxionMass(func ma2);    
        AxionMass(std::string path, LD minT, LD maxT);

        AxionMass& operator=(const AxionMass& axionMass)=delete;//delete it explicitely, in order to prevent copies

        /*get the values of Tmin, Tmax, chi(Tmin), and chi(Tmax)*/
        LD getTMin(){return TMin;}
        LD getTMax(){return TMax;}
        LD getChiMin(){return chiMin;}
        LD getChiMax(){return chiMax;}

        void set_ma2(func ma2){ this->ma2=ma2;}
        
        /*set the functions for ma2 beyond the interpolation limits*/
        void set_ma2_MAX(func ma2_MAX){ this->ma2_MAX=ma2_MAX;}
        void set_ma2_MIN(func ma2_MIN){ this->ma2_MIN=ma2_MIN;}

        LD operator()(LD T, LD fa){return ma2(T,fa);}//overload this just in case you need it.
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

            this->ma2_MAX=[this](LD T, LD fa){return this->chiMax/fa/fa;};
            this->ma2_MIN=[this](LD T, LD fa){return this->chiMin/fa/fa;};
            
            this->chi=CubicSpline<LD>(&Ttab,&chitab);

            ma2=[this](LD T, LD fa){
                // axion mass squared at temperature T and f_\alpha=fa
                if(T>=TMax){return ma2_MAX(T,fa);}//use this beyond the upper limit
                if(T<=TMin){return ma2_MIN(T,fa);}//use this beyond the lower limit
                return this->chi(T)/fa/fa; 
            };
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
            this->ma2=ma2;
    }


};



#endif