#include<iostream>
#include<functional>
#include"src/AxionMass/AxionMass.hpp"

// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif


// use this to cast void* to axionMass*
#define Cast(axionMass) static_cast<mimes::AxionMass<LD>*>(axionMass)

typedef LD(* funcType )(LD,LD);
extern "C"{
    //constructor
    void* INIT_interpolation(char* path, LD minT, LD maxT){ 
        return new mimes::AxionMass<LD>(path,minT,maxT);
    }

    void* INIT_function(funcType ma2){ 

        return new mimes::AxionMass<LD>( ma2 );
    }

    // destructor to delete the void*
    void DEL(void *axionMass){  delete Cast(axionMass); axionMass=nullptr; }

    // axion mass squared
    LD ma2(LD T, LD fa, void* axionMass){return Cast(axionMass)->ma2(T,fa);}



}
