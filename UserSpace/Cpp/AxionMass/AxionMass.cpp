#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 

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
    


    mimes::AxionMass<LD> axionMass(chi_PATH,minT,maxT);
    /*set ma2 for T>TMax*/
    LD TMax=axionMass.getTMax();    
    LD chiMax=axionMass.getChiMax();    
    axionMass.set_ma2_MAX( [&chiMax,&TMax](LD T, LD fa){return chiMax/fa/fa*std::pow(T/TMax,-8.16);});  
    
    /*set ma2 for T<TMin*/
    LD TMin=axionMass.getTMin();  
    LD chiMin=axionMass.getChiMin();    
    axionMass.set_ma2_MIN( [&chiMin,&TMin](LD T, LD fa){return chiMin/fa/fa;});


    if(minT==maxT){
        std::cout<<axionMass.ma2(minT,fa)<<"\t"<<axionMass.ma2(maxT,fa)<<"\n";
        return 0;
    }
    // print N points
    unsigned int N=50;
    
    // take logarithmically spaced points 
    std::vector<LD> T;
    /*---use logspace like this:---*/
    // mimes::util::logspace<LD>(std::log10(minT), std::log10(maxT),N,T);
    
    /*---or like this:---*/
    T = mimes::util::logspace<LD>(std::log10(minT), std::log10(maxT),N);
    
    std::cout<<"-----Interpolation-----\n";
    std::cout<<"T[GeV]\tm_a^2[GeV^2]\n";
    for(unsigned int i = 0; i < N; ++i){
        std::cout<<T[i]<<"\t"<<axionMass.ma2(T[i],fa)<<"\n";
    }
    

    return 0;
}
