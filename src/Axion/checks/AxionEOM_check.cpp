#include <iostream>
#include <iomanip> 
#include <cmath> 
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
    LD theta_i = 0.93;
    LD fa = 1e12;
    LD tmax = 500;
    LD TSTOP = 1e-3;
    LD ratio_ini=100;
    
    if(argc>1){        
        theta_i = atof(argv[1]) ;
        fa = atof(argv[2]);
        tmax = atof(argv[3]);
        TSTOP = atof(argv[4]);
        ratio_ini=atof(argv[5]);
    }

    mimes::AxionEOM<LD> axionEOM(theta_i, fa, tmax, TSTOP, ratio_ini,"input.dat");
    axionEOM.makeInt();

    std::cout<<"T_osc~= "<<axionEOM.T_osc<<" GeV"<<std::endl;
    std::cout<<"a_osc~= "<<std::exp(axionEOM.t_osc)<<std::endl;

    LD t, T, logH2;
    // check if the file is read
    #ifdef readCheck
    std::cout<<"------------check if the file is read-----------"<<std::endl;
    for( int i=0; i<axionEOM.t_tab.size(); ++i ){
        std::cout<<axionEOM.t_tab[i]<<"\t"<<axionEOM.T_tab[i]<<"\t"<<axionEOM.logH2_tab[i]<<"\n";
    }
    #endif



    // check if interpolation gives the same numbers on the points from the file
    #ifdef interpolationVSreadCheck
    std::cout<<"------------check if interpolation gives the same numbers on the points from the file-----------"<<std::endl;
    for(auto i=0; i<axionEOM.t_tab.size(); ++i ){
        t=axionEOM.t_tab[i];
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
        std::cout<<axionEOM.dlogH2dt(t)<<"\n";
        t+=axionEOM.t_stop/(Npoints-10) ;///the last few points should be the same as I make the interpolation return T_stop and logH2_stop if I go beyond t_stop 
    }
    #endif


    return 0;
}
