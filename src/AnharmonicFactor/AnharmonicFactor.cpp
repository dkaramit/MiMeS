#include"src/AnharmonicFactor/AnharmonicFactor.hpp"
#include "src/misc_dir/mimes_paths.hpp"


// anharmonic factor 
template<class LD> mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif


extern "C"{
    LD _anharmonicFactor(LD theta_max){return anharmonicFactor<LD>(theta_max);}
}
