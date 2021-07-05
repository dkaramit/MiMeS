#ifndef COSMO_head
#define COSMO_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "Interpolation.hpp"
#include "path.hpp"

template<class LD>
class Cosmo {
    using VecLD=std::vector<LD>;
    
    protected:
    CubicSpline<LD> h,g;
    VecLD Ttab,htab,gtab;
    LD TMin, TMax, hMin, hMax, gMin, gMax;
    LD Mp;
    public:
    LD mP;

    Cosmo(std::string path, LD minT=0, LD maxT=1e10){    
        /*
        path is the pat of the data
        minT (maxT) the minimum (maximum) T you need for the interpolation
        */
        // the data are assumed to be: T  h  g
        // the currently used data are taken from arXiv: 2005.03544 
        
        mP=1.22e19;
        
        unsigned int N;
        LD tmp1,tmp2,tmp3;
        std::ifstream data_file(path);


        while (not data_file.eof()){
            data_file>>tmp1;
            data_file>>tmp2;
            data_file>>tmp3;
            if(tmp1>=minT and tmp1<=maxT){
                Ttab.push_back(tmp1);
                htab.push_back(tmp2);
                gtab.push_back(tmp3);
            }
        }
        N=Ttab.size();
        TMin=Ttab[0];
        TMax=Ttab[N-1];
        hMin=htab[0];
        hMax=htab[N-1];
        gMin=gtab[0];
        gMax=gtab[N-1];

        this->h=CubicSpline<LD>(&Ttab,&htab);
        this->g=CubicSpline<LD>(&Ttab,&gtab);
    }

    LD heff(LD T){
        if(T>=TMax){return hMax;}
        if(T<=TMin){return hMin;}
        return h(T);
    }

    LD dh(LD T){
        if(T>=TMax){return 1;}
        if(T<=TMin){return 1;}
        return 1.+1/3.*T/h(T)*h.derivative_1(T);    
    }
    
    LD geff(LD T){
        if(T>=TMax){return gMax;}
        if(T<=TMin){return gMin;}
        return g(T);
    }
    LD dgeffdT(LD T){
        if(T>=TMax){return 0;}
        if(T<=TMin){return 0;}
        return g.derivative_1(T);
    }
    LD dheffdT(LD T){
        if(T>=TMax){return 0;}
        if(T<=TMin){return 0;}
        return h.derivative_1(T);
    }

    LD s(LD T){
        //s=\dfrac{2\pi^2}{45} h_{eff} T^3.
        return (2*M_PI*M_PI)/(45.)*heff(T)*T*T*T;
    }

    LD rhoR(LD T){
        //\rho_R=\dfrac{\pi^2}{30} g_{eff} T^4
        return M_PI*M_PI/30.*geff(T)*T*T*T*T ;
    }


    LD Hubble(LD T){
        //H=\sqrt{ \dfrac{8\pi}{3 M_p^2} \rho_R(T) }
        return std::sqrt(8*M_PI/3. *rhoR(T) )/mP ;
}
};




#ifndef LONG
    #define LONG long
#endif

#ifndef LLD
    #define LLD LONG double
#endif

static Cosmo<LLD> _cosmo(h_PATH,1e-5,3e3);


static const LLD T0= 2.7255 * 8.62e-14; //Today's CMB temperature in GeV
static const LLD h_hub=0.674; //dimensionless hubble parameter
static const LLD rho_crit=(0.197*1e-13)*(0.197*1e-13)*(0.197*1e-13)*(1.05*1e-5)*h_hub*h_hub; //critical density today in GeV^4
static const LLD relicDM_obs=0.12; //DM relic abandance today
static const LLD mP=_cosmo.mP;//Planck mass


LLD heff(LLD T){
    return _cosmo.heff(T);
}

LLD geff(LLD T){
    return _cosmo.geff(T);
}

LLD dgeffdT(LLD T){
    return _cosmo.dgeffdT(T);
}
LLD dheffdT(LLD T){
    return _cosmo.dheffdT(T);
}

LLD dh(LLD T){
    return _cosmo.dh(T);
}

LLD s(LLD T){
    return _cosmo.s(T);
}

LLD rhoR(LLD T){
    return _cosmo.rhoR(T);
}


LLD Hubble(LLD T){
    return _cosmo.Hubble(T);
}

#endif