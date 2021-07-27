#include"src/static.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif



extern "C"{
    LD ma2(LD T, LD fa){
        return axionMass.ma2(T,fa);
    }
    LD ma2_approx(LD T, LD fa){
        return axionMass.ma2_approx(T,fa);
    }

    LD dma2dT(LD T, LD fa){
        return axionMass.dma2dT(T,fa);
    }
    LD dma2dT_approx(LD T, LD fa){
        return axionMass.dma2dT_approx(T,fa);
    }
}
