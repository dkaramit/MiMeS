#include"src/Cosmo/Cosmo.hpp"
#include "src/misc_dir/mimes_paths.hpp"


/*cosmological parameters*/
//it is better not to use all the available data h_PATH, because there are a lot of points.
//interpolating up to T=3 TeV should be enough (the difference is less than 1%)...
// template<class LD> mimes::Cosmo<LD> cosmo(cosmo_PATH,0,mimes::Cosmo<LD>::mP);



// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif

// use this to cast void* to Axion*
#define Cast(cosmo) static_cast<mimes::Cosmo<LD>*>(cosmo)

extern "C"{
    void* INIT(char* path, LD minT, LD maxT){return new mimes::Cosmo<LD>(path,minT,maxT);}
    void DEL(void *cosmo){  delete Cast(cosmo); cosmo=nullptr; }

    //this functions return the different cosmological parameters we neeed. 
    LD heff(LD T, void* cosmo){return Cast(cosmo)->heff(T);}

    LD geff(LD T, void* cosmo){return Cast(cosmo)->geff(T);}

    LD dgeffdT(LD T, void* cosmo){return Cast(cosmo)->dgeffdT(T);}

    LD dheffdT(LD T, void* cosmo){return Cast(cosmo)->dheffdT(T);}

    LD dh(LD T, void* cosmo){return Cast(cosmo)->dh(T);}

    LD s(LD T, void* cosmo){return Cast(cosmo)->s(T);}

    LD rhoR(LD T, void* cosmo){return Cast(cosmo)->rhoR(T);}

    LD Hubble(LD T, void* cosmo){return Cast(cosmo)->Hubble(T);}
    
    LD getT0(){return mimes::Cosmo<LD>::T0;}

    LD getrho_crit(){return mimes::Cosmo<LD>::rho_crit;}
    
    LD geth_hub(){return mimes::Cosmo<LD>::h_hub;}
    
    LD getrelicDM(){return mimes::Cosmo<LD>::relicDM_obs;}

    LD getMP(){return mimes::Cosmo<LD>::mP;}

}