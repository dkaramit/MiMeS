#include"AxionMass.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif


typedef LLD(*FuncType)(LLD,LLD);



extern "C"{
    //this functions return h,g,rho,Hubble, etc. So in python I will a copy of this (see Hubble.py)
    FuncType ma2(  ){
        return m_alpha2;
    }
    FuncType ma2_approx(  ){
        return m_alpha2_approx;
    }
    FuncType dma2dT(  ){
        return m_alpha2_derivative_1;
    }
    FuncType dma2dT_approx(  ){
        return m_alpha2_derivative_1_approx;
    }
}
