#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string>
#include"src/Axion/AxionSolve.hpp"


// #define preintPoints
// #define printPeaks


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
    std::string  inputFile=argv[8];

    mimes::Axion<LD> Ax(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile);

    Ax.solveAxion();


    unsigned int N=Ax.peaks.size();
    LD theta,zeta,T;
    T=Ax.peaks[N-1][1];
    theta=Ax.peaks[N-1][2];
    zeta=Ax.peaks[N-1][3];

    std::cout<<std::setprecision(25)
    <<theta_i<<"\t"<<fa<<Ax.theta_osc<<"\t"<<Ax.T_osc<<"\t"<<Ax.relic<<"\n";


    // print all the points
    #ifdef preintPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]"<<std::endl;
    for(int i=0; i<Ax.pointSize; ++i ){
        for(int j=0; j<5; ++j){
            std::cout<<Ax.points[i][j];
            if(j==4){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif

    // print all the peaks
    #ifdef printPeaks
    std::cout<<"---------------------peaks:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]\tadiabatic_inv [GeV]"<<std::endl;
    for(int i=0; i<Ax.peakSize; ++i ){
        for(int j=0; j<6; ++j){
            std::cout<<Ax.peaks[i][j];
            if(j==5){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif



    return 0;
}
