#ifndef SolveAxion_included
#define SolveAxion_included
#include <cmath>
#include <string>
#include <functional>
#include <vector>




//----Solver-----//
#include "src/Rosenbrock/LU/LU.hpp"
#include "src/Rosenbrock/Ros_class.hpp"
#include "src/Rosenbrock/Ros_costructor.hpp"
#include "src/Rosenbrock/Ros_LU.hpp"
#include "src/Rosenbrock/Ros_calc_k.hpp"
#include "src/Rosenbrock/Ros_sums.hpp"
#include "src/Rosenbrock/Ros_step_control-simple.hpp"
// #include "src/Rosenbrock/Ros_step_control-PI.hpp"
#include "src/Rosenbrock/Ros_steps.hpp"
#include "src/Rosenbrock/Jacobian.hpp"
#include "src/Rosenbrock/Ros_METHOD.hpp"



/*================================*/
#ifndef METHOD
    #define METHOD RODASPR2
#endif

// macros for the solver
#define minimum_step_size 1e-8 //minimum stepsize of integration.
/*---------------------------------------------------*/
/*initial stepsize. This can be quite large, 
but generally if it is safer to be clase to the maximum allowed one.*/
#define initial_step_size 1e-2
/*maximum stepsize. This limits the sepsize to an upper limit.*/ 
#define maximum_step_size 1e-2 
//-----------------------------------------------//
/*If the solver takes more than maximum_No_steps, it quits, even if integration is not complete. 
One should tweak the other parameters, in order to avoid this. */
#define maximum_No_steps int(1e7)
/*relative and absolute tolerances. Generally, 1e-8 is good, but in some case one may need more accurate result. 
However, if the tolerances are below 1e-8, long doubles *must* beused.*/
#define absolute_tolerance 1e-8
#define relative_tolerance 1e-8
/*beta controls how agreesive the adaptation is. Generally, it should be around 0.9.*/
#define beta 0.9
/*the stepsize does not increase more than fac_max, and less than fac_min. This ensure
a better stability. Ideally, both should be close to 1, but in reality one must tweak them.*/
#define fac_max 1.05
#define fac_min 0.8
/*================================*/




//---Get the eom of the axion--//
#include "src/Axion/AxionEOM.hpp"


/*get static variables (includes cosmological parameters, axion mass, and anharmonic factor)*/
#include "src/static.hpp"
/*================================*/


namespace mimes{

    template<class LD>
    class Axion{
        //-----Function type--------//
        using sys= std::function<void (Array<LD> &lhs, Array<LD> &y, LD u)>;
        using Solver=Ros<sys, Neqs, METHOD<LD>, Jacobian<sys, Neqs, LD>, LD>;

        public:
        LD theta_i,fa,umax,TSTOP,ratio_ini;
        LD theta_osc, T_osc, a_osc; 

        LD gamma;//gamma is the entropy injection from the point of the last peak until T_stop (the point where interpolation stops)
        LD relic;

        std::vector<std::vector<LD>> points;
        std::vector<std::vector<LD>> peaks;


        unsigned int pointSize,peakSize;
        unsigned int N_convergence_max;
        LD convergence_lim;

        std::string inputFile;

        // constructor of Axion.
        /*
        theta_i: initial angle
        fa: PQ scale in GeV (the temperature dependent mass is defined as m_a^2(T) = \chi(T)/f^2)
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM, 
        in order to make the interpolations start at this point)
        
        N_convergence_max and convergence_lim: integration stops after the adiabatic invariant 
        hasn't changed more than convergence_lim% for N_convergence_max consecutive peaks

        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH
        */ 
        Axion(LD theta_i, LD fa, LD umax, LD TSTOP, LD ratio_ini, 
                unsigned int N_convergence_max, LD convergence_lim, std::string inputFile){
            this->theta_i=theta_i;
            this->fa=fa;

            this->umax=umax;
            this->TSTOP=TSTOP;
            this->ratio_ini=ratio_ini;

            this->N_convergence_max=N_convergence_max;
            this->convergence_lim=convergence_lim;

            this->inputFile=inputFile;
        }
        Axion(){}
        
        void solveAxion();
    };



    template<class LD>
    void Axion<LD>::solveAxion(){ 
        //whem theta_i<<1, we can refactor the eom so that it becomes independent from theta_i.
        // So, we can solve for theta_i=1e-3, and rescale the result to our desired initial theta.
        // This helps avoid any roundoff errors when the amplitude of the oscillation becomes very small.
        LD theta_ini=theta_i;
        if(theta_ini<1e-3){theta_ini=1e-3;}

        /*================================*/
        Array<LD> y0={theta_ini, 0.};//initial conditions
        /*================================*/

        AxionEOM<LD> axionEOM(theta_i, fa, ratio_ini, inputFile);
        
        axionEOM.makeInt();//make the interpolations of u,T,logH from inputFile
        
        //You find these as you load the data from inputFile 
        //(it is done automatically in the constructor of axionEOM)
        T_osc=axionEOM.T_osc;
        a_osc=std::exp(axionEOM.u_osc);

        //use a lambda to pass axionEOM in the solver (the overhead should be minimal)
        sys EOM = [&axionEOM](Array<LD> &lhs, Array<LD> &y, LD u){axionEOM(lhs, y, u);};

        // instance of the solver
        Solver System(EOM, y0, umax,
                        initial_step_size, minimum_step_size, maximum_step_size, maximum_No_steps,
                        absolute_tolerance, relative_tolerance, beta, fac_max,fac_min);

        // these parameters are helpful..
        int current_step=0;//count the steps the solver takes

        //check is used to identify the peaks, osc_check is used to find the oscillation temperature
        bool check=true, osc_check=true;
        

        //use these to count the peaks in order to apply the stopping conditions
        unsigned int Npeaks=0, N_convergence=0;

        //these variables will be used to fill points (the points the solver takes)
    
        LD theta=0,zeta=0,u=0,a=0,T=0,H2=0,ma2=0;
        LD rho_axion=0;

        // we will need these for the peaks
        LD zeta_prev=0,u_prev=0,theta_prev=0;
        LD theta_peak=0,zeta_peak=0,u_peak=0,a_peak=0,T_peak=0,ma2_peak=0;
        LD adInv_peak=0,rho_axion_peak=0;
        LD an_diff=0;

        // this is the initial step (ie points[0])
        theta=y0[0];
        zeta=y0[1];
        T=axionEOM.Temperature(0);
        H2=std::exp(axionEOM.logH2(0));
        ma2=axionMass.ma2(T,fa);
        if(std::abs(theta)<1e-3){rho_axion=fa*fa*(ma2*0.5*theta*theta);}
        else{rho_axion=fa*fa*(ma2*(1 - std::cos(theta)));}
        points.push_back(std::vector<LD>{1,T,theta,0,rho_axion});



        // the solver identifies the peaks in theta by finding points where zeta goes from positive to negative.
        // Once the peaks are identified, we can calculate the adiabatic invariant.
        // The solver stops when the idiabatic invariant is almost constant.
        while (true){
            current_step++;// incease number of steps that the solver takes

            //stop if you exceed umax or if the solver takes more than maximum_No_steps, stop
            if(System.tn>=System.tmax or current_step==System.max_N){break;}
            
            //take the next step
            System.next_step(); 
            //update y (y[0]=theta, y[1]=zeta)
            for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
            // increase tn
            System.tn+=System.h;

            //rescale theta and zeta which will be stored in points
            //(we rescale them if theta_i<1e-3 in order to avoid roundoff errors)
            theta=System.ynext[0]/theta_ini*theta_i;
            zeta=System.ynext[1]/theta_ini*theta_i;

            //remember that u=log(a/a_i)
            u=System.tn;
            a=std::exp(u);

            //get the temperature which corresponds to u
            T=axionEOM.Temperature(u);
            //get the Hubble parameter squared which corresponds to u
            H2=std::exp(axionEOM.logH2(u));

            // If you use as T_osc the one provided by axionEOM, you will not have theta_osc.
            // So use this (it doesn't really matter, as T_osc is not a very well defined parameter)
            // We could use interpolation to get more accurate T_osc and theta_osc, but it's not worth it,
            // because the oscillation temperature does not affect the results.
            if(T<=T_osc and osc_check){
                T_osc=T;
                a_osc=a;
                theta_osc=theta;
                osc_check=false;
            }

            //if the temperature is below TSTOP, stop the integration
            if(T<TSTOP){break;}

            //axion mass squared
            ma2=axionMass.ma2(T,fa);

            // If theta<~1e-8, we have roundoff errors due to cos(theta)
            // The solution is this (use theta<1e-3; it doesn't matter):
            if(std::abs(theta)<1e-3){rho_axion=fa*fa*(0.5*  H2*zeta*zeta+ ma2*0.5*theta*theta);}
            else{rho_axion=fa*fa*(0.5*  H2*zeta*zeta+ ma2*(1 - std::cos(theta)));}

            //store current step in points 
            points.push_back(std::vector<LD>{a,T,theta,zeta,rho_axion});
            
            //check if current point is a peak
            if(zeta>0){check=false;}
            if(zeta<=0 and check==false){
                //increase the total number of peaks 
                Npeaks++;
                //set check=true (this resets the check until the next peak)
                check=true; 
                
                // use linear interpolation to find u at zeta=0
                u_prev=std::log(points[current_step-1][0]);
                theta_prev=points[current_step-1][2];
                zeta_prev=points[current_step-1][3];
                // these are all the quantities at the peak
                u_peak=(-zeta_prev*u+zeta*u_prev)/(zeta-zeta_prev);
                a_peak=std::exp(u_peak);
                theta_peak=((theta-theta_prev)*u_peak+(theta_prev*u-theta*u_prev))/(u-u_prev);
                zeta_peak=((zeta-zeta_prev)*u_peak+(zeta_prev*u-zeta*u_prev))/(u-u_prev);
                T_peak=axionEOM.Temperature(u_prev);
                ma2_peak=axionMass.ma2(T_peak,fa);

                //compute the adiabatic invariant
                adInv_peak=anharmonicFactor(theta_peak)*theta_peak*theta_peak *std::sqrt(ma2_peak) * a_peak*a_peak*a_peak ;
                
                if(std::abs(theta)<1e-3){rho_axion_peak=fa*fa*(ma2_peak*0.5*theta_peak*theta_peak);}
                else{rho_axion_peak=fa*fa*(ma2_peak*(1 - std::cos(theta_peak)));}


                peaks.push_back(std::vector<LD>{a_peak,T_peak,theta_peak,zeta_peak,rho_axion_peak,adInv_peak});
                

                // if the total number of peaks is greater than 2, then you can check for convergence
                if(Npeaks>=2){
                    //difference of adiabatic invariant between current and previous peak 
                    an_diff=std::abs(adInv_peak-peaks[Npeaks-2][5]);
                    if(adInv_peak>peaks[Npeaks-2][5]){
                        an_diff=an_diff/adInv_peak;
                    }else{
                        an_diff=an_diff/peaks[Npeaks-2][5];
                    }
                    //if the difference is smaller than convergence_lim increase N_convergence
                    //else, reset N_convergence (we stop if the difference is small between consecutive steps )
                    if(an_diff<convergence_lim){N_convergence++;}else{N_convergence=0;}
                }
            }
            
            //if N_convergence>=N_convergence_max, compute the relic and stop the integration
            if(N_convergence>=N_convergence_max){
                //entropy injection from the point of the last peak until T_stop (the point where interpolation stops)
                //the assumption is that at T_stop the universe is radiation dominated with constant entropy.
                gamma=cosmo.s(axionEOM.T_stop)/cosmo.s(T_peak)*std::exp(3*(axionEOM.u_stop-u_peak));
                
                //the relic of the axion
                relic=h_hub*h_hub/rho_crit*cosmo.s(T0)/cosmo.s(T_peak)/gamma*0.5*
                       std::sqrt(axionMass.ma2(T0,1)*axionMass.ma2(T_peak,1))*
                       theta_peak*theta_peak*anharmonicFactor(theta_peak);
                

                //this is equivalent
                // relic=h_hub*h_hub/rho_crit*
                    //    cosmo.s(T0)/cosmo.s(axionEOM.T_stop)*std::exp(3*(t_peak-axionEOM.t_stop))*0.5*
                    //    std::sqrt(axionMass.ma2(T0,1)*axionMass.ma2(T_peak,1))*
                    //    theta_peak*theta_peak*anharmonicFactor(theta_peak);
                

                break;
            }

        }

        pointSize=points.size();
        peakSize=peaks.size();
    };

};






#endif