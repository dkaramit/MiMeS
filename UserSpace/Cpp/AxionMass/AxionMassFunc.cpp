#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include <functional> 
#include"src/AxionMass/AxionMass.hpp"
#include"src/misc_dir/path.hpp"


//-- Get a number (length) of log_10-spaced points from 10^min to 10^max. --//
template<class LD>
void logspace(LD min, LD max, int length, std::vector<LD> &X ){
    for(int i = 0; i<length ; ++i){
        X.push_back(pow( 10, min + i*(max-min)/( length-1 )));
    }
}

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=3){
        std::cerr<<"usage: AxionMass.run fa minT maxT\n";
        std::cerr<<"With:\n \
        T: tempareture at which the mass is evaluated.\n \
        fa: PQ scale.\n";

        return 1;
    }

    LD T=atof(argv[1]);
    LD fa=atof(argv[2]);
    

    // just a function. This is the approximation for the axion mass! 
    std::function<LD(LD,LD)> ma2 = [](LD T,LD fa){
        LD TQCD=150*1e-3;
        LD ma20=3.1575e-05/fa/fa;
        if(T<=TQCD){return ma20;}
        else{return ma20*std::pow((TQCD/T),8.16);}
    };

    // In this axM instance, you only provide the axion mass squared.
    mimes::AxionMass<LD> axM(ma2);
    std::cout<<"m_a^2= "<<axM.ma2(T,fa)<<"\n";

    

    return 0;
}
