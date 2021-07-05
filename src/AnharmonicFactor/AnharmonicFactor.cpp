#include"AnharmonicFactor.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif


typedef LLD(*FuncType)(LLD);



extern "C"{
    FuncType anharmonic_factor(  ){
        return _anharmonic_factor;
    }

}
