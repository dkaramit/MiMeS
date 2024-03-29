#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 

#include"MiMeS.hpp"

// #define printPoints // print the entire evolution of the axion angle, its derivative, and other quantities
// #define printPeaks // print these quantities at the peaks of the oscillation
// #define printError //print only the results
// #define smallPrint //print the results in a human readable form
#define resultPrint //print only the results


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

    if(argc!=18){
        std::cerr<<"usage: Axion.run theta_i f_a umax TSTOP ratio_ini N_convergence_max convergence_lim inputFile\n \
        initial_step_size minimum_step_size maximum_step_size absolute_tolerance relative_tolerance beta \n \
        fac_max fac_min maximum_No_steps\n";
        
        std::cerr<<"With:\n\n \
        theta_i: initial angle\n\n \
        fa: PQ scale in GeV (the temperature dependent mass is defined as m_a^2(T) = \\chi(T)/f^2)\n\n \
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))\n\n \
        TSTOP: if the temperature drops below this, integration stops\n\n \
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM,\n \
        in order to make the interpolations start at this point)\n\n \
        N_convergence_max and convergence_lim: integration stops when the relative difference\n \
        between two consecutive peaks is less than convergence_lim for N_convergence_max\n \
        consecutive peaks\n\n \
        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH\n\n \
        -----------Optional arguments------------------------\n\n \
        initial_stepsize: initial step the solver takes.\n\n \
        maximum_stepsize: This limits the sepsize to an upper limit.\n\n \
        minimum_stepsize: This limits the sepsize to a lower limit.\n\n \
        absolute_tolerance: absolute tolerance of the RK solver.\n\n \
        relative_tolerance: relative tolerance of the RK solver.\n\n \
        Note:\n \
        Generally, both absolute and relative tolerances should be 1e-8.\n\n \
        In some cases, however, one may need more accurate result (eg if f_a is extremely high,\n \
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the\n \
        tolerances are below 1e-8, long doubles *must* be used.\n\n \
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.\n\n \
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.\n\n \
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must\n \
        tweak them in order to avoid instabilities.\n\n \
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration\n \
        is not finished.\n";

        return 1;
    }

    
    //timeit.sh adds some (insignificant) overhead. So, alternatively, you can use
    // mimes::util::Timer _time_; 
    // The _time_ instance  prints the time the program took in stderr when the program exits (when _time_ goes out of scope in general).  

    // you can change the axion mass function for T above the interpolation range like this (these are not correct; they only show how it can be done):
    // axionMass<LD>.ma2_MAX = [](LD T, LD fa)->LD{ return 0; }; // this is fine for T>>T_{QCD}
    // axionMass<LD>.ma2_MIN = [](LD T, LD fa)->LD{return 0;}; //this gives Omegah^2 = 0 (but you are sure that it works)

    int ar=0;

    //model parameters
    LD theta_i = atof(argv[++ar]) ;
    LD fa = atof(argv[++ar]);
    // solver parameters
    LD umax = atof(argv[++ar]); //u at which the integration stops 
    LD TSTOP = atof(argv[++ar]); // temperature at which integration stops
    LD ratio_ini=atof(argv[++ar]); // 3H/m_a at which integration begins (should be larger than 500 or so)
    // stopping conditions.
    // integration stops after the adiabatic invariant hasn't changed 
    // more than  convergence_lim% for N_convergence_max consecutive peaks
    unsigned int N_convergence_max=atoi(argv[++ar]);
    LD convergence_lim=atof(argv[++ar]);
    //file in which the cosmology is defined. the columns should be : u T[GeV] logH
    std::string  inputFile=argv[++ar];
    
    /*options for the solver (These variables are optional. Yoou can use the Axion class without them).*/
    LD initial_step_size=atof(argv[++ar]); //initial step the solver takes. 
    LD minimum_step_size=atof(argv[++ar]); //This limits the sepsize to an upper limit. 
    LD maximum_step_size=atof(argv[++ar]); //This limits the sepsize to a lower limit.
    LD absolute_tolerance=atof(argv[++ar]); //absolute tolerance of the RK solver
    LD relative_tolerance=atof(argv[++ar]); //relative tolerance of the RK solver
    LD beta=atof(argv[++ar]); //controls how agreesive the adaptation is. Generally, it should be around but less than 1.
    /*
    the stepsize does not increase more than fac_max, and less than fac_min. 
    This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
    tweak them in order to avoid instabilities.
    */
    LD fac_max=atof(argv[++ar]); 
    LD fac_min=atof(argv[++ar]);
    unsigned int maximum_No_steps=atoi(argv[++ar]); //maximum steps the solver can take Quits if this number is reached even if integration is not finished.
    

    // use chi_PATH to interpolate the axion mass.
    mimes::AxionMass<LD> axionMass(chi_PATH,0,mimes::Cosmo<LD>::mP);
    /*set ma2 for T>TMax*/
    LD TMax=axionMass.getTMax();    
    LD chiMax=axionMass.getChiMax();    
    axionMass.set_ma2_MAX( [&chiMax,&TMax](LD T, LD fa){return chiMax/fa/fa*std::pow(T/TMax,-8.16);});  
    
    /*set ma2 for T<TMin*/
    LD TMin=axionMass.getTMin();  
    LD chiMin=axionMass.getChiMin();    
    axionMass.set_ma2_MIN( [&chiMin,&TMin](LD T, LD fa){return chiMin/fa/fa;});

    // you can define the axion mass using a function, like this 
    // std::function<LD(LD,LD)> ma2 = [](LD T,LD fa){
    //     LD TQCD=150*1e-3;
    //     LD ma20=3.1575e-05/fa/fa;
    //     if(T<=TQCD){return ma20;}
    //     else{return ma20*std::pow((TQCD/T),8.16);}
    // };
    // mimes::AxionMass<LD> axionMass(ma2);


    // instance of Axion
    mimes::Axion<LD,SOLVER,METHOD<LD>> ax(theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max,
    convergence_lim, inputFile, &axionMass,
    initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance, beta,
    fac_max, fac_min, maximum_No_steps);

    // Solve the Axion EOM
    ax.solveAxion();

    //get the most important results 
    #ifdef resultPrint
    std::cout<<std::setprecision(16)
    <<theta_i<<" "<< fa<<" "<<ax.theta_osc<<" "<<ax.T_osc<<" "<<ax.relic<<"\n";
    #endif 

    #ifdef smallPrint
    std::cout<<std::setprecision(25)
    <<"theta_i="<<theta_i<<" "<<"f_a="<< fa<<" GeV\n"<<"theta_osc~="<<ax.theta_osc<<" "
    <<"T_osc~="<<ax.T_osc<<"GeV \n"
    <<"Omega h^2="<<ax.relic<<"\n";
    #endif 


    // print all the points
    #ifdef printPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]"<<std::endl;
    for(int i=0; i<ax.pointSize; ++i ){
        for(int j=0; j<5; ++j){
            std::cout<<ax.points[i][j];
            if(j==4){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif

    // print all the peaks
    #ifdef printPeaks
    std::cout<<"---------------------peaks:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]\tadiabatic_inv [GeV]"<<std::endl;
    for(int i=0; i<ax.peakSize; ++i ){
        for(int j=0; j<6; ++j){
            std::cout<<ax.peaks[i][j];
            if(j==5){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif

    // print the local errors
    #ifdef printError
    std::cout<<"---------------------local errors:---------------------\n";
    std::cout<<"dtheta\tdzeta"<<std::endl;
    for(size_t i=0; i<ax.pointSize; ++i ){
        std::cout<<ax.dtheta[i]<<"\t"<<ax.dzeta[i]<<"\n";
    }
    #endif


    return 0;
}
