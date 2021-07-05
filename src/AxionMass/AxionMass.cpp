#include"AxionMass.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif


extern "C"{
    LLD ma2(LLD T, LLD fa){
        return axionMass.ma2(T,fa);
    }
    LLD ma2_approx(LLD T, LLD fa){
        return axionMass.ma2_approx(T,fa);
    }

    LLD dma2dT(LLD T, LLD fa){
        return axionMass.ma2_derivative_1(T,fa);
    }
    LLD dma2dT_approx(LLD T, LLD fa){
        return axionMass.ma2_derivative_1_approx(T,fa);
    }
}
