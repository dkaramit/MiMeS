#ifndef Static_head
#define Static_head

#include<functional>

#include"src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"
#include"src/AnharmonicFactor/AnharmonicFactor.hpp"


#include "src/misc_dir/path.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

/*cosmological parameters*/
//it is better not to use all the available data h_PATH, because there are a lot of points.
//interpolating up to T=3 TeV should be enough (the difference is less than 1%)...
static mimes::Cosmo<LD> cosmo(cosmo_PATH,0,mimes::Cosmo<LD>::mP);

static const LD T0=  mimes::Cosmo<LD>::T0;
static const LD h_hub= mimes::Cosmo<LD>::h_hub;
static const LD rho_crit= mimes::Cosmo<LD>::rho_crit;
static const LD relicDM_obs=mimes::Cosmo<LD>::relicDM_obs;
static const LD mP=mimes::Cosmo<LD>::mP;


// axion mass
//it is better not to use all the available data chi_PATH, 
//because there could a lot of useless points that would make the interpolation slow.
//interpolating up to T=10GeV should be enough...
static mimes::AxionMass<LD> axionMass(chi_PATH,0,mimes::Cosmo<LD>::mP);

// anharmonic factor 
static mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

#undef LD

#endif