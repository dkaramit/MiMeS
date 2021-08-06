#include"src/static.hpp"

// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif


extern "C"{
    //this functions return the different cosmological parameters we neeed. 
    LD heff(LD T){return cosmo<LD>.heff(T);}

    LD geff(LD T){return cosmo<LD>.geff(T);}

    LD dgeffdT(LD T){return cosmo<LD>.dgeffdT(T);}

    LD dheffdT(LD T){return cosmo<LD>.dheffdT(T);}

    LD dh(LD T){return cosmo<LD>.dh(T);}

    LD s(LD T){return cosmo<LD>.s(T);}

    LD rhoR(LD T){return cosmo<LD>.rhoR(T);}

    LD Hubble(LD T){return cosmo<LD>.Hubble(T);}
    
    LD getT0(){return mimes::Cosmo<LD>::T0;}

    LD getrho_crit(){return mimes::Cosmo<LD>::rho_crit;}
    
    LD geth_hub(){return mimes::Cosmo<LD>::h_hub;}
    
    LD getrelicDM(){return mimes::Cosmo<LD>::relicDM_obs;}

    LD getMP(){return mimes::Cosmo<LD>::mP;}

}