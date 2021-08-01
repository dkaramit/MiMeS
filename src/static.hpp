#ifndef Static_head
#define Static_head


#include"src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"
#include"src/AnharmonicFactor/AnharmonicFactor.hpp"


#include "src/misc_dir/path.hpp"

/*cosmological parameters*/
//it is better not to use all the available data h_PATH, because there are a lot of points.
//interpolating up to T=3 TeV should be enough (the difference is less than 1%)...
template<class LD> static mimes::Cosmo<LD> cosmo(cosmo_PATH,0,mimes::Cosmo<LD>::mP);


// axion mass
//it is better not to use all the available data chi_PATH, 
//because there could a lot of useless points that would make the interpolation slow.
//interpolating up to T=10GeV should be enough...
template<class LD> static mimes::AxionMass<LD> axionMass(chi_PATH,0,mimes::Cosmo<LD>::mP);

// anharmonic factor 
template<class LD> static mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

#endif