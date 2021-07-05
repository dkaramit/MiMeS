#include"Cosmo.hpp"

#ifndef LONG
    #define LONG long
#endif

#ifndef LLD
    #define LLD LONG double
#endif


typedef LLD(*FuncType)(LLD);



extern "C"{
    //this functions return h,g,rho,Hubble, etc. So in python I will a copy of this (see Hubble.py)
    FuncType hsub(  ){
        return heff;
    }
    FuncType gsub(  ){
        return geff;
    }
    FuncType dgdTsub(  ){
        return dgeffdT;
    }
    FuncType dhdTsub(  ){
        return dheffdT;
    }
    FuncType dhsub(  ){
        return dh;
    }
    FuncType rsub(  ){
        return rhoR;
    }
    FuncType Hsub(  ){
        return Hubble;
    }
    
    FuncType Ssub(  ){
        return s;
    }
    
    LLD getT0(){return T0;}
    LLD getrho_crit(){return rho_crit;}
    LLD geth_hub(){return h_hub;}
    LLD getrelicDM(){return relicDM_obs;}

}
