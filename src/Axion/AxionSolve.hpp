#ifndef SolveAxion_included
#define SolveAxion_included
#include <iostream>

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


//---Get the eom of the axion--//
#include "src/Axion/AxionEOM.hpp"


/*get static variables (includes cosmological parameters, axion mass, and anharmonic factor)*/
#include "src/static.hpp"
/*================================*/


/*================================*/
#ifndef METHOD
    #define METHOD RODASPR2
#endif

// macros for the solver
#define minimum_step_size 1e-8
/*---------------------------------------------------*/
#define initial_step_size 1e-3
#define maximum_step_size 1e-2
//-----------------------------------------------//
#define maximum_No_steps int(1e7)
#define absolute_tolerance 1e-11
#define relative_tolerance 1e-11
#define beta 0.9
#define fac_max 1.05
#define fac_min 0.5
/*================================*/

namespace mimes{

    template<class LD>
    class Axion{
        //-----Function type--------//
        using sys= std::function<void (Array<LD> &lhs, Array<LD> &y, LD t)>;
        using Solver=Ros<sys, Neqs, METHOD<LD>, Jacobian<sys, Neqs, LD>, LD>;

        public:
        LD theta_i,fa,tmax,TSTOP,ratio_ini;
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
        tmax: if t>tmax the integration stops (rempember that t=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM, 
        in order to make the interpolations start at this point)
        
        N_convergence_max and convergence_lim: integration stops after the adiabatic invariant 
        hasn't changed more than convergence_lim% for N_convergence_max consecutive peaks

        inputFile: file that describes the cosmology. the columns should be: t T[GeV] logH
        */ 
        Axion(LD theta_i, LD fa, LD tmax, LD TSTOP, LD ratio_ini, 
                unsigned int N_convergence_max, LD convergence_lim, std::string inputFile){
            this->theta_i=theta_i;
            this->fa=fa;

            this->tmax=tmax;
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
        
        axionEOM.makeInt();//make the interpolations of t,T,logH from inputFile
        
        //You find these as you load the data from inputFile 
        //(it is done automatically in the constructor of axionEOM)
        T_osc=axionEOM.T_osc;
        a_osc=std::exp(axionEOM.t_osc);

        //use a lambda to pass axionEOM in the solver (the overhead should be minimal)
        sys EOM = [&axionEOM](Array<LD> &lhs, Array<LD> &y, LD t){axionEOM(lhs, y, t);};

        // instance of the solver
        Solver System(EOM, y0, tmax,
                        initial_step_size, minimum_step_size, maximum_step_size, maximum_No_steps,
                        absolute_tolerance, relative_tolerance, beta, fac_max,fac_min);

        // these parameters are helpful..
        unsigned int current_step=0;//count the steps the solver takes

        //check is used to identify the peaks, osc_check is used to find the oscillation temperature
        bool check=true, osc_check=true;
        

        //use these to count the peaks in order to apply the stopping conditions
        unsigned int Npeaks=0, N_convergence=0;

        //these variables will be used to fill points (the points the solver takes)
    
        LD theta,zeta,t,a,T,tmp,H2,ma2;
        LD rho_axion;
        std::vector<LD> adiabatic_invariant;
        LD an_diff;

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

            //stop if you exceed tmax or if the solver takes more than maximum_No_steps, stop
            if(System.tn>=System.tmax or current_step==System.max_N){break;}
            
            //take the next step
            System.next_step(); 
            //update y (y[0]=theta, y[1]=zeta)
            for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
            // increase t
            System.tn+=System.h;

            //rescale theta and zeta which will be stored in points
            //(we rescale them if theta_i<1e-3 in order to avoid roundoff errors)
            theta=System.ynext[0]/theta_ini*theta_i;
            zeta=System.ynext[1]/theta_ini*theta_i;

            //remember that t=log(a/a_i)
            t=System.tn;
            a=std::exp(t);

            //get the temperature which corresponds to t
            T=axionEOM.Temperature(t);
            //get the Hubble parameter squared which corresponds to t
            H2=std::exp(axionEOM.logH2(t));

            //if you use as T_osc the once provided by axionEOM, you will not have theta_osc.
            //So use this (it doesn't really matter, as T_osc is not a very well defined parameter)
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
                //increas the total number of peaks 
                Npeaks++;
                //set check=true (this resets the check until the next peak)
                check=true; 
                
                //compute the adiabatic invariant
                tmp=anharmonicFactor(theta)*theta*theta *std::sqrt(ma2) * a*a*a ;
                peaks.push_back(std::vector<LD>{a,T,theta,zeta,rho_axion,tmp});

                //store current adiabatic invariant
                adiabatic_invariant.push_back(tmp);


                // if the total number of peaks is greater than 2, then you can check for convergence
                if(Npeaks>=2){
                    //difference of adiabatic invariant between current and previous peak 
                    an_diff=std::abs(adiabatic_invariant[Npeaks-1]-adiabatic_invariant[Npeaks-2]);
                    if(adiabatic_invariant[Npeaks-1]>adiabatic_invariant[Npeaks-2]){
                        an_diff=an_diff/adiabatic_invariant[Npeaks-1];
                    }else{
                        an_diff=an_diff/adiabatic_invariant[Npeaks-2];
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
                gamma=cosmo.s(axionEOM.T_stop)/cosmo.s(T)*std::exp(3*(axionEOM.t_stop-System.tn));
                
                //the relic of the axion
                relic=h_hub*h_hub/rho_crit*cosmo.s(T0)/cosmo.s(T)/gamma*0.5*
                       std::sqrt(axionMass.ma2(T0,1)*axionMass.ma2(T,1))*
                       theta*theta*anharmonicFactor(theta);
                

                break;
            }

        }

        pointSize=points.size();
        peakSize=peaks.size();
    };

};






#endif