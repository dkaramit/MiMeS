#include"src/static.hpp"

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
