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

static const LD T0= 2.7255 * 8.62e-14; //Today's CMB temperature in GeV
static const LD h_hub=0.674; //dimensionless hubble parameter
static const LD rho_crit=(0.197*1e-13)*(0.197*1e-13)*(0.197*1e-13)*(1.05*1e-5)*h_hub*h_hub; //critical density today in GeV^4
static const LD relicDM_obs=0.12; //DM relic abandance today
static const LD mP=cosmo.mP;//Planck mass

// anharmonic factor 
static mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

// axion mass
static mimes::AxionMass<LD> axionMass(chi_PATH,1e-5,3e3);


#undef LD

#endif