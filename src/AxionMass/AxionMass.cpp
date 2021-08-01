#include"src/static.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif



extern "C"{
    LD ma2(LD T, LD fa){
        return axionMass<LD>.ma2(T,fa);
    }

    LD dma2dT(LD T, LD fa){
        return axionMass<LD>.dma2dT(T,fa);
    }
}
