#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include <functional> 

#include"MiMeS.hpp"


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=4){
        std::cerr<<"usage: AxionMass.run fa minT maxT\n";
        std::cerr<<"With:\n \
        fa: PQ scale.\n \
        minT: minimum interpolation tempareture.\n \
        maxT: maximum interpolation tempareture.\n\n \
        Beyond these limits, everything is taken constant, so one can use minT=1e-5 GeV and maxT=1e3 with good accuracy.\n";

        return 1;
    }

    LD fa=atof(argv[1]);

    LD minT=atof(argv[2]);
    LD maxT=atof(argv[3]);
    
    

    // just a function. This is the approximation for the axion mass! 
    std::function<LD(LD,LD)> ma2 = [](LD T,LD fa){
        LD TQCD=150*1e-3;
        LD ma20=3.1575e-05/fa/fa;
        if(T<=TQCD){return ma20;}
        else{return ma20*std::pow((TQCD/T),8.16);}
    };

    // In this AxionMass instance, you only provide the axion mass squared.
    mimes::AxionMass<LD> axionMass(ma2);

    if(minT==maxT){
        std::cout<<axionMass.ma2(minT,fa)<<"\t"<<axionMass.ma2(maxT,fa)<<"\n";
        return 0;
    }
    

        // print N points
    unsigned int N=50;
    
    // take logarithmically spaced points 
    std::vector<LD> T;
    T = mimes::util::logspace<LD>(std::log10(minT), std::log10(maxT),N);
    // fill a vector with the mass at these temperatures
    std::vector<LD> ma2T;
    ma2T = mimes::util::map<LD>(T, [&fa,&axionMass](LD T){return axionMass.ma2(T,fa);});

    for(unsigned int i=0 ; i<N; ++i){ std::cout<<T[i]<<"\t"<<ma2T[i]<<"\n";}


    return 0;
}
