#include"src/static.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

extern "C"{
    LD _anharmonicFactor(LD theta_max){return anharmonicFactor(theta_max);}
}
