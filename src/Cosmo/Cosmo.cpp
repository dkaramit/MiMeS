#include"Cosmo.hpp"

#ifndef LONG
    #define LONG long
#endif

#ifndef LLD
    #define LLD LONG double
#endif


extern "C"{
    //this functions return the different cosmological parameters we neeed. 
    LLD heff(LLD T){return cosmo.heff(T);}

    LLD geff(LLD T){return cosmo.geff(T);}

    LLD dgeffdT(LLD T){return cosmo.dgeffdT(T);}

    LLD dheffdT(LLD T){return cosmo.dheffdT(T);}

    LLD dh(LLD T){return cosmo.dh(T);}

    LLD s(LLD T){return cosmo.s(T);}

    LLD rhoR(LLD T){return cosmo.rhoR(T);}

    LLD Hubble(LLD T){return cosmo.Hubble(T);}
    
    LLD getT0(){return T0;}

    LLD getrho_crit(){return rho_crit;}
    
    LLD geth_hub(){return h_hub;}
    
    LLD getrelicDM(){return relicDM_obs;}
}