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

    /*get the values of Tmin, Tmax, chi(Tmin), and chi(Tmax)*/
    LD getTMin(void *axionMass){return Cast(axionMass)->getTMin();}
    LD getTMax(void *axionMass){return Cast(axionMass)->getTMax();}
    LD getChiMin(void *axionMass){return Cast(axionMass)->getChiMin();}
    LD getChiMax(void *axionMass){return Cast(axionMass)->getChiMax();}
    
    void set_ma2(funcType ma2,void *axionMass){ Cast(axionMass)->set_ma2(ma2); }
    
    /*set the functions for ma2 beyond the interpolation limits*/
    void set_ma2_MAX(funcType ma2_MAX,void *axionMass){ Cast(axionMass)->set_ma2_MAX(ma2_MAX); }
    void set_ma2_MIN(funcType ma2_MIN,void *axionMass){ Cast(axionMass)->set_ma2_MIN(ma2_MIN); }


    // axion mass squared
    LD ma2(LD T, LD fa, void* axionMass){return Cast(axionMass)->ma2(T,fa);}



}
