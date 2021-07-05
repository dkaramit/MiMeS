#include <iostream>
#include <iomanip> 
#include <cmath> 
#include"src/Axion/Axion_eom.hpp"


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
    LD theta_i = atof(argv[1]) ;
    LD fa = atof(argv[2]);
    LD umax = atof(argv[3]);
    LD TSTOP = atof(argv[4]);
    LD ratio_ini=atof(argv[5]);

    Axion_eom<LD> Ax(theta_i, fa, umax, TSTOP, ratio_ini,"input.dat");
    Ax.makeInt();
    LD t, T, logH2;
    // check if the file is read
    #ifdef readCheck
    std::cout<<"------------check if the file is read-----------"<<std::endl;
    for( int i=0; i<Ax.t_tab.size(); ++i ){
        std::cout<<Ax.t_tab[i]<<"\t"<<Ax.T_tab[i]<<"\t"<<Ax.logH2_tab[i]<<"\n";
    }
    #endif



    // check if interpolation gives the same numbers on the points from the file
    #ifdef interpolationVSreadCheck
    std::cout<<"------------check if interpolation gives the same numbers on the points from the file-----------"<<std::endl;
    for(auto i=0; i<Ax.t_tab.size(); ++i ){
        t=Ax.t_tab[i];
        std::cout<<t<<"\t";
        T=Ax.T_tab[i];
        std::cout<<std::abs((T - Ax.Temperature(t))/T) <<"\t";
        logH2=Ax.logH2_tab[i];
        std::cout<<std::abs((logH2 - Ax.logH2(t))/logH2)<<"\n";
    }
    #endif

    // check if interpolation works
    #ifdef interpolationCheck
    std::cout<<"------------check if interpolation works-----------"<<std::endl;
    t=0;
    int Npoints=500;
    for(auto i=0; i<Npoints; ++i ){
        std::cout<<t<<"\t";
        std::cout<<Ax.Temperature(t) <<"\t";
        std::cout<<Ax.logH2(t)<<"\t";
        std::cout<<Ax.dlogH2dt(t)<<"\n";
        t+=Ax.t_stop/(Npoints-10) ;///the last few points should be the same as I make the interpolation return T_stop and logH2_stop if I go beyond t_stop 
    }
    #endif

    return 0;
}
