#include <iostream>
#include <iomanip> 
#include <cmath> 
#include"src/Axion/AxionSolve.hpp"




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
    LD tmax = atof(argv[3]);
    LD TSTOP = atof(argv[4]);
    LD ratio_ini=atof(argv[5]);
    unsigned int N_convergence_max=atoi(argv[6]);
    LD convergence_lim=atof(argv[7]);

    Axion<LD> Ax(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,"input.dat");

    Ax.solveAxion();


    unsigned int N=Ax.peaks.size();
    LD theta,zeta,T;
    T=Ax.peaks[N-1][1];
    theta=Ax.peaks[N-1][2];
    zeta=Ax.peaks[N-1][3];

    std::cout<<std::setprecision(5)
    <<theta_i<<" "<< fa<<" "<< tmax<<" "<< TSTOP<<" "<<T<<" "<<theta<<" "<<zeta
    <<" "<<Ax.theta_osc<<" "<<Ax.T_osc<<" "<<Ax.relic<<"\n";

    std::cout<<Ax.peaks.size()<<std::endl;

    #if false
    for(int i=0; i<Ax.size; ++i ){
        for(int j=0; j<5; ++j){
            std::cout<<Ax.points[i][j];
            if(j==4){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif
    return 0;
}
