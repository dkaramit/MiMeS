#include <iostream>
#include <iomanip> 
#include <cmath> 
#include"src/Axion/AxionSolve.hpp"


#define preintPoints
#define printPeaks


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
    LD ratio_ini=1000;
    unsigned int N_convergence_max=3;
    LD convergence_lim=1e-3;
    
    mimes::Axion<LD> Ax(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,"InputExamples/RDinput.dat");

    Ax.solveAxion();


    unsigned int N=Ax.peaks.size();
    LD theta,zeta,T;
    T=Ax.peaks[N-1][1];
    theta=Ax.peaks[N-1][2];
    zeta=Ax.peaks[N-1][3];

    std::cout<<std::setprecision(5)
    <<"theta_i="<<theta_i<<" "<<"f_a="<< fa<<" GeV\n"<<"theta_osc~="<<Ax.theta_osc<<" "
    <<"T_osc~="<<Ax.T_osc<<"GeV \n"
    <<"Omega h^2="<<Ax.relic<<"\n";


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
