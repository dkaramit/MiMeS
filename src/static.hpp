#ifndef Static_head
#define Static_head

#include"src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"
#include"src/AnharmonicFactor/AnharmonicFactor.hpp"


#include "path.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

/*cosmological parameters*/
static mimes::Cosmo<LD> cosmo(h_PATH,1e-5,3e3);

static const LD T0=  mimes::Cosmo<LD>::T0;
static const LD h_hub= mimes::Cosmo<LD>::h_hub;
static const LD rho_crit= mimes::Cosmo<LD>::rho_crit;
static const LD relicDM_obs=mimes::Cosmo<LD>::relicDM_obs;
static const LD mP=mimes::Cosmo<LD>::mP;


// anharmonic factor 
static mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

// axion mass
static mimes::AxionMass<LD> axionMass(chi_PATH,1e-5,3e3);


#undef LD

#endif