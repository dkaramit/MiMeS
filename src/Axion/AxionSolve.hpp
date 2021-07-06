#ifndef SolveAxion_included
#define SolveAxion_included
#include <iostream>

#include <cmath>
#include <string>
#include <functional>
#include <vector>




//----Solver-----//
#include "LU/LU.hpp"
#include "Ros_class.hpp"
#include "Ros_costructor.hpp"
#include "Ros_LU.hpp"
#include "Ros_calc_k.hpp"
#include "Ros_sums.hpp"
#include "Ros_step_control-simple.hpp"
// #include "Ros_step_control-PI.hpp"
#include "Ros_steps.hpp"
#include "Jacobian.hpp"
#include "Ros_METHOD.hpp"


//---Get the eom of the axion--//
#include "AxionEOM.hpp"


/*other stuff*/
#include "src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"
#include"src/AnharmonicFactor/AnharmonicFactor.hpp"
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


using std::vector;


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

    vector<vector<LD>> points;
    vector<vector<LD>> peaks;


    unsigned int pointSize,peakSize;
    unsigned int N_convergence_max;
    LD convergence_lim;

    std::string inputFile;

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
    // So, we can solve for theta_i=1e-3, and rescale the result to our desired initial theta
    LD theta_ini=theta_i;
    if(theta_ini<1e-3){theta_ini=1e-3;}

    /*================================*/
    Array<LD> y0={theta_ini, 0.};
    /*================================*/

    AxionEOM<LD> axionEOM(theta_i, fa, tmax, TSTOP, ratio_ini, inputFile);
    
    axionEOM.makeInt();
    
    //you can find these as you load the data
    T_osc=axionEOM.T_osc;
    a_osc=std::exp(axionEOM.t_osc);


    sys EOM = [&axionEOM](Array<LD> &lhs, Array<LD> &y, LD t){axionEOM(lhs, y, t);};

    Solver System(EOM, y0, tmax,
                    initial_step_size, minimum_step_size, maximum_step_size, maximum_No_steps,
                    absolute_tolerance, relative_tolerance, beta, fac_max,fac_min);


    unsigned int current_step=0;
    bool check=true, osc_check=true;
    LD theta=theta_i,zeta=0;
    LD t,a,T,tmp,H2,ma2;
    LD rho_axion;
    std::vector<LD> adiabatic_invariant;
    LD an_diff;
    unsigned int Npeaks=0, N_convergence=0;

    T=axionEOM.Temperature(0);
    H2=std::exp(axionEOM.logH2(0));
    ma2=axionMass.ma2(T,fa);
    if(std::abs(theta)<1e-3){
        rho_axion=fa*fa*(ma2*0.5*theta*theta);

    }else{
        rho_axion=fa*fa*(ma2*(1 - std::cos(theta)));
    }
    points.push_back(vector<LD>{1,T,theta,0,rho_axion});



    // the solver identifies the peaks in theta by finding points where zeta goes from negative to positive.
    // Once the peaks are identified, we can calculate the adiabatic invariant.
    // The solver stops when the idiabatic invariant is almost constant.
    while (true){
        current_step++;
        if( System.tn>=System.tmax  or current_step == System.max_N  ) {   break ;}

        System.next_step();
        for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
        // increase time
        System.tn+=System.h;

        theta=System.ynext[0]/theta_ini*theta_i;
        zeta=System.ynext[1]/theta_ini*theta_i;

        t=System.tn;
        a=std::exp(t);

        T=axionEOM.Temperature(t);
        H2=std::exp(axionEOM.logH2(t));

        if(T<=T_osc and osc_check){
            T_osc=T;
            a_osc=a;
            theta_osc=theta;
            osc_check=false;
        }
        
        if(T<TSTOP){break;}

        ma2=axionMass.ma2(T,fa);

        // when reading from python, if theta<~1e-8, we have roundoff errors due to the cos(theta)
        // The solution is this:
        if(std::abs(theta)<1e-3){
            rho_axion=fa*fa*(0.5*  H2*zeta*zeta+ ma2*0.5*theta*theta);

        }else{
            rho_axion=fa*fa*(0.5*  H2*zeta*zeta+ ma2*(1 - std::cos(theta)));
        }

        points.push_back(vector<LD>{a,T,theta,zeta,rho_axion});

        
        if(zeta>0){check=false;}

        if(zeta<=0 and check==false){
            Npeaks++;
            check=true; 

            tmp=anharmonicFactor(theta)*theta*theta *std::sqrt(ma2) * a*a*a ;
            peaks.push_back(vector<LD>{a,T,theta,zeta,rho_axion,tmp});

            adiabatic_invariant.push_back(tmp);

            if(Npeaks>=2){
                an_diff=std::abs(adiabatic_invariant[Npeaks-1]-adiabatic_invariant[Npeaks-2]);
                if(adiabatic_invariant[Npeaks-1]>adiabatic_invariant[Npeaks-2]){
                    an_diff=an_diff/adiabatic_invariant[Npeaks-1];
                }else{
                    an_diff=an_diff/adiabatic_invariant[Npeaks-2];
                }

                if(an_diff<convergence_lim){
                    N_convergence++;
                }
            }
        }

        if(N_convergence>=N_convergence_max){
            gamma=cosmo.s(axionEOM.T_stop)/cosmo.s(T)*std::exp(3*(axionEOM.t_stop-System.tn));
            
            relic=h_hub*h_hub/rho_crit*cosmo.s(T0)/cosmo.s(T)/gamma*0.5*
                    std::sqrt(axionMass.ma2(T0,1)*axionMass.ma2(T,1))*
                    theta*theta*anharmonicFactor(theta);
            


            break;
        }

    }

    pointSize=points.size();
    peakSize=peaks.size();
};








#endif