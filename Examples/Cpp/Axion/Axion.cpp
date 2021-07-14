#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/Axion/AxionSolve.hpp"


// #define preintPoints
// #define printPeaks
// #define smallPrint
#define largePrint


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=9){
        std::cout<<"usage: AxionExample.run theta_i f_a umax TSTOP ratio_ini N_convergence_max convergence_lim inputFile\n";
        std::cout<<"With:\n\n \
        theta_i: initial angle\n\n \
        fa: PQ scale in GeV (the temperature dependent mass is defined as m_a^2(T) = \\chi(T)/f^2)\n\n \
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))\n\n \
        TSTOP: if the temperature drops below this, integration stops\n\n \
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM,\n \
        in order to make the interpolations start at this point)\n\n \
        N_convergence_max and convergence_lim: integration stops when the relative difference\n \
        between two consecutive peaks is less than convergence_lim for N_convergence_max\n \
        consecutive peaks\n\n \
        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH\n";

        return 0;
    }


    //model parameters
    LD theta_i = atof(argv[1]) ;
    LD fa = atof(argv[2]);
    // solver parameters
    LD umax = atof(argv[3]); //u at which the integration stops 
    LD TSTOP = atof(argv[4]); // temperature at which integration stops
    LD ratio_ini=atof(argv[5]); // 3H/m_a at which integration begins (should be larger than 500 or so)
    // stopping conditions.
    // integration stops after the adiabatic invariant hasn't changed 
    // more than  convergence_lim% for N_convergence_max consecutive peaks
    unsigned int N_convergence_max=atoi(argv[6]);
    LD convergence_lim=atof(argv[7]);
    //file in which the cosmology is defined. the columns should be : u T[GeV] logH
    std::string  inputFile=argv[8];
    

    // instance of Axion
    mimes::Axion<LD> Ax(theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile);

    // Solve the Axion EOM
    Ax.solveAxion();

    //get the most important results 
    #ifdef largePrint
    std::cout<<std::setprecision(25)
    <<theta_i<<" "<< fa<<" "<<Ax.theta_osc<<" "<<Ax.T_osc<<" "<<Ax.relic<<"\n";
    #endif 

    #ifdef smallPrint
    std::cout<<std::setprecision(25)
    <<"theta_i="<<theta_i<<" "<<"f_a="<< fa<<" GeV\n"<<"theta_osc~="<<Ax.theta_osc<<" "
    <<"T_osc~="<<Ax.T_osc<<"GeV \n"
    <<"Omega h^2="<<Ax.relic<<"\n";
    #endif 


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
