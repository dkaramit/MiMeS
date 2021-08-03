#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/Axion/AxionSolve.hpp"

// define here what you want to print.
#define preintPoints
#define printPeaks
#define printError

// macros for the solver
#ifndef SOLVER
    #define SOLVER 1
    #define METHOD RODASPR2
#endif


// macros for the numeric type
#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    //model parameters
    LD theta_i = 0.94435;
    LD fa = 1e12;

    // solver parameters
    LD umax = 500; //t at which the integration stops 
    LD TSTOP = 1e-3; // temperature at which integration stops
    LD ratio_ini=1e3; // 3H/m_a at which integration begins (should be larger than 500 or so)
    
    // stopping conditions.
    // integration stops after the adiabatic invariant hasn't changed 
    // more than  convergence_lim% for N_convergence_max consecutive peaks
    unsigned int N_convergence_max=5;
    LD convergence_lim=1e-2;
    std::string  inputFile=std::string(rootDir)+std::string("/UserSpace/InputExamples/RDinput.dat");


    /*options for the solver*/
    LD initial_step_size=1e-2; //initial step the solver takes. 
    LD minimum_step_size=1e-8; //This limits the sepsize to an upper limit. 
    LD maximum_step_size=1e-2; //This limits the sepsize to a lower limit.
    LD absolute_tolerance=1e-8; //absolute tolerance of the RK solver
    LD relative_tolerance=1e-8; //relative tolerance of the RK solver
    LD beta=0.9; //controls how agreesive the adaptation is. Generally, it should be around but less than 1.
    
    /*
    the stepsize does not increase more than fac_max, and less than fac_min. 
    This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
    tweak them in order to avoid instabilities.
    */
    LD fac_max=1.2; 
    LD fac_min=0.8;
    int maximum_No_steps=int(1e7); //maximum steps the solver can take Quits if this number is reached even if integration is not finished.



    mimes::Axion<LD,SOLVER,METHOD<LD>> Ax(theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile,
    initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance, beta,
    fac_max, fac_min, maximum_No_steps);

    Ax.solveAxion();

    std::cout<<std::setprecision(5)
    <<"theta_i="<<theta_i<<" "<<"f_a="<< fa<<" GeV\n"<<"theta_osc~="<<Ax.theta_osc<<" "
    <<"T_osc~="<<Ax.T_osc<<"GeV \n"
    <<"Omega h^2="<<Ax.relic<<"\n";

    // print all the points
    #ifdef preintPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]"<<std::endl;
    for(size_t i=0; i<Ax.pointSize; ++i ){
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
    for(size_t i=0; i<Ax.peakSize; ++i ){
        for(int j=0; j<6; ++j){
            std::cout<<Ax.peaks[i][j];
            if(j==5){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif

    // print the local errors
    #ifdef printError
    std::cout<<"---------------------local errors:---------------------\n";
    std::cout<<"dtheta\tdzeta"<<std::endl;
    for(size_t i=0; i<Ax.pointSize; ++i ){
        std::cout<<Ax.dtheta[i]<<"\t"<<Ax.dzeta[i]<<"\n";
    }
    #endif



    return 0;
}
