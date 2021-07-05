#ifndef CHI_head
#define CHI_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include"Interpolation.hpp"
#include "path.hpp"

template<class LD>
class AxionMass {
    using VecLD=std::vector<LD>;
    protected:
    CubicSpline<LD> chi;
    VecLD Ttab,chitab;
    LD TMin, TMax, chiMin, chiMax;
    LD LambdaQCD;
    public:

    AxionMass(std::string path, LD minT=0, LD maxT=1e5){    
        
        unsigned int N;
        LD tmp1,tmp2;
        std::ifstream data_file(path);

        while (not data_file.eof()){
            data_file>>tmp1;
            data_file>>tmp2;
            if(tmp1>=minT and tmp1<=maxT){
                Ttab.push_back(tmp1*1e-3); //temperature in GeV
                chitab.push_back(tmp2*0.197*0.197*0.197*0.197); //chi in GeV**4
            }
        }
        N=Ttab.size();
        TMin=Ttab[0];
        TMax=Ttab[N-1];
        chiMin=chitab[N-1];
        chiMax=chitab[0];

        this->chi=CubicSpline<LD>(&Ttab,&chitab);

        LambdaQCD=0.4;
    }

    LD m_alpha2(LD T, LD fa){
        // axion mass squared at temperature T and f_\alpha=fa
        if(T>=TMax){return chiMin/fa/fa*std::pow(T/TMax,-8.16);}
        if(T<=TMin){return chiMax/fa/fa;}
        return chi(T)/fa/fa;
    }


    LD m_alpha2_derivative_1(LD T, LD fa){
        // axion mass squared derivative
        if(T>=TMax){return -8.16*chiMin/fa/fa*std::pow(T/TMax,-9.16);}
        
        if(T<=TMin){return 0;}

        return chi.derivative_1(T)/fa/fa;
    }

    LD m_alpha2_approx(LD T, LD fa){
        // axion mass squared at temperature T and f_\alpha=fa
        LD ma20=chiMax/fa/fa;
        LD b=4e-4;
        LD fT=std::pow(T/LambdaQCD,-8.16);
        if(b*fT<=1){return b*fT*ma20;}
        else{return ma20;}    
    
    }
    LD m_alpha2_derivative_1_approx(LD T, LD fa){
        // axion mass squared at temperature T and f_\alpha=fa
        LD ma20=chiMax/fa/fa;
        LD b=4e-4;
        LD fT=std::pow(T/LambdaQCD,-8.16);
        if(b*fT<=1){return -8.16/(T/LambdaQCD)*b*fT*ma20;}
        else{return 0;}    
    
    }
};


#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif



static AxionMass<LLD> _axion_chi(chi_PATH,1e-5,3e3);

LLD m_alpha2(LLD T, LLD fa){
    return _axion_chi.m_alpha2(T,fa);
}
LLD m_alpha2_approx(LLD T, LLD fa){
    return _axion_chi.m_alpha2_approx(T,fa);
}

LLD m_alpha2_derivative_1(LLD T, LLD fa){
    return _axion_chi.m_alpha2_derivative_1(T,fa);
}
LLD m_alpha2_derivative_1_approx(LLD T, LLD fa){
    return _axion_chi.m_alpha2_derivative_1_approx(T,fa);
}

#endif