#include"AnharmonicFactor.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif




extern "C"{
    LLD _anharmonicFactor(LLD theta_max){return anharmonicFactor(theta_max);}
}
