#include"src/static.hpp"

// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif

extern "C"{
    LD ma2(LD T, LD fa){
        return axionMass<LD>.ma2(T,fa);
    }

    LD dma2dT(LD T, LD fa){
        return axionMass<LD>.dma2dT(T,fa);
    }
}
