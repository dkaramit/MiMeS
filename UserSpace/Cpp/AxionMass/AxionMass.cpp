#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
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
    


    mimes::AxionMass<LD> axM(chi_PATH,minT,maxT);


    if(minT==maxT){
        std::cout<<axM.ma2(minT,fa)<<"\t"<<axM.ma2(maxT,fa)<<"\n";
        return 0;
    }
    // print N points
    unsigned int N=50;
    
    // take logarithmically spaced points 
    std::vector<LD> T;
    logspace<LD>(std::log10(minT), std::log10(maxT),N,T);
    
    std::cout<<"-----Interpolation-----\n";
    std::cout<<"T[GeV]\tm_a^2[GeV^2]\tdm_a^2dT[GeV]\n";
    for(unsigned int i = 0; i < N; ++i){
        std::cout<<T[i]<<"\t"<<axM.ma2(T[i],fa)<<"\t"<<axM.dma2dT(T[i],fa)<<"\n";
    }
    
    std::cout<<"-----Approximations-----\n";
    std::cout<<"T[GeV]\tm_{a,approx}^2[GeV^2]\tdm_a^2dT_{approx}[GeV]\n";
    for(unsigned int i = 0; i < N; ++i){
        std::cout<<T[i]<<"\t"<<axM.ma2_approx(T[i],fa)<<"\t"<<axM.dma2dT_approx(T[i],fa)<<"\n";
    }
    


    return 0;
}
