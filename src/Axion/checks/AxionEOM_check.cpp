#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/Axion/AxionEOM.hpp"


#define readCheck
#define interpolationVSreadCheck
#define interpolationCheck




#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 
    //model parameters
    LD fa = 1e12;
    LD ratio_ini=100;
    std::string  inputFile=std::string(rootDir)+std::string("/UserSpace/InputExamples/RDinput.dat");
    

    mimes::AxionEOM<LD> axionEOM(fa, ratio_ini,inputFile);
    axionEOM.makeInt();

    std::cout<<"T_osc~= "<<axionEOM.T_osc<<" GeV"<<std::endl;
    std::cout<<"a_osc~= "<<std::exp(axionEOM.u_osc)<<std::endl;

    LD t, T, logH2;
    // check if the file is read
    #ifdef readCheck
    std::cout<<"------------check if the file is read-----------"<<std::endl;
    for(size_t i=0; i<axionEOM.u_tab.size(); ++i ){
        std::cout<<axionEOM.u_tab[i]<<"\t"<<axionEOM.T_tab[i]<<"\t"<<axionEOM.logH2_tab[i]<<"\n";
    }
    #endif



    // check if interpolation gives the same numbers on the points from the file
    #ifdef interpolationVSreadCheck
    std::cout<<"------------check if interpolation gives the same numbers on the points from the file-----------"<<std::endl;
    for(size_t i=0; i<axionEOM.u_tab.size(); ++i ){
        t=axionEOM.u_tab[i];
        std::cout<<t<<"\t";
        T=axionEOM.T_tab[i];
        std::cout<<std::abs((T - axionEOM.Temperature(t))/T) <<"\t";
        logH2=axionEOM.logH2_tab[i];
        std::cout<<std::abs((logH2 - axionEOM.logH2(t))/logH2)<<"\n";
    }
    #endif

    // check if interpolation works
    #ifdef interpolationCheck
    std::cout<<"------------check if interpolation works-----------"<<std::endl;
    t=0;
    int Npoints=500;
    for(auto i=0; i<Npoints; ++i ){
        std::cout<<t<<"\t";
        std::cout<<axionEOM.Temperature(t) <<"\t";
        std::cout<<axionEOM.logH2(t)<<"\t";
        std::cout<<axionEOM.dlogH2du(t)<<"\n";
        t+=axionEOM.u_stop/(Npoints-10) ;///the last few points should be the same as I make the interpolation return T_stop and logH2_stop if I go beyond t_stop 
    }
    #endif


    return 0;
}
