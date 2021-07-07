#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string>
#include"src/Axion/AxionSolve.hpp"

/*uncomment to print all the points of integration*/
// #define printPoints  

/*uncomment to print only the peaks*/
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

    // solver parameters
    LD tmax = atof(argv[3]); //t at which the integration stops 
    LD TSTOP = atof(argv[4]); // temperature at which integration stops
    LD ratio_ini=atof(argv[5]); // 3H/m_a at which integration begins (should be larger than 500 or so)
    
    // stopping conditions.
    // integration stops after the adiabatic invariant hasn't changed 
    // more than  convergence_lim% for N_convergence_max consecutive peaks
    unsigned int N_convergence_max=atoi(argv[6]);
    LD convergence_lim=atof(argv[7]);

    //file in which the cosmology is defined. the columns should be : t T[GeV] logH
    std::string  inputFile=argv[8];

    mimes::Axion<LD> Ax(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile);

    Ax.solveAxion();

    unsigned int N=Ax.peaks.size();
    LD theta,zeta,T;
    T=Ax.peaks[N-1][1];
    theta=Ax.peaks[N-1][2];
    zeta=Ax.peaks[N-1][3];

    // print theta_i, fa, along with 
    // T and theta at the point oscillations start
    // and the relic
    std::cout<<std::setprecision(25)
    <<theta_i<<"\t"<<fa<<"\t"<<Ax.theta_osc<<"\t"<<Ax.T_osc<<"\t"<<Ax.relic<<"\n";


    // print all the points
    #ifdef printPoints
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
