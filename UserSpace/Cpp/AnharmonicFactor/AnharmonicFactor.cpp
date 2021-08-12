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

    if(argc!=2){
        std::cerr<<"usage: AnharmonicFactor.run theta_max\n";
        std::cerr<<"With:\n \
        theta_max: maximum theta.\n";

        return 1;
    }


    mimes::AnharmonicFactor<LD> anharmonicFactor(anharmonic_PATH);

    std::cout<<anharmonicFactor(atof(argv[1]))<<"\n";
    


    return 0;
}
