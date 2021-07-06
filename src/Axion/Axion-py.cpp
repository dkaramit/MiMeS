#include<string>
#include "src/Axion/AxionSolve.hpp"


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

// use this to cast void* to Axion*
#define Cast(ax) static_cast<mimes::Axion<LD>*>(ax)

extern "C"{
    //constructor
    void* INIT(LD theta_i, LD fa, LD tmax, LD TSTOP, LD ratio_ini, unsigned int N_convergence_max, LD convergence_lim, char* inputFile){ 
        return new mimes::Axion<LD>(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim, static_cast<std::string>(inputFile));
    }
    // destructor to delete the void*
    void DEL(void *Ax){  delete static_cast<mimes::Axion<LD>*>(Ax); Ax=nullptr; }

    void MAKE(void * Ax){  Cast(Ax) -> solveAxion(); }
    unsigned int getPointSize(void * Ax){  return Cast(Ax) -> pointSize; }
    unsigned int getPeakSize(void * Ax){  return Cast(Ax) -> peakSize; }


    void getResults(LD *results, void *Ax){
        results[0]=Cast(Ax) ->T_osc;
        results[1]=Cast(Ax)->a_osc;
        results[2]=Cast(Ax) ->theta_osc;
        results[3]=Cast(Ax) ->relic;

    }

    void getPoints(LD *a, LD *T, LD *theta, LD *zeta,  LD *rho_a, void *Ax){
        unsigned int N = Cast(Ax) ->pointSize;
        for (unsigned int i = 0; i < N; i++){
          a[i]=Cast(Ax) ->points[i][0];
          T[i]=Cast(Ax) ->points[i][1];
          theta[i]=Cast(Ax) ->points[i][2];
          zeta[i]=Cast(Ax) ->points[i][3];
          rho_a[i]=Cast(Ax) ->points[i][4];
        }
    }



    void getPeaks(LD *a_peak, LD *T_peak, LD *theta_peak, LD*zeta_peak,  LD *rho_a_peak, LD *ad_inv_peak, void *Ax){
        unsigned int Npeaks = Cast(Ax) ->peakSize;
        for (unsigned int i = 0; i < Npeaks; i++){
          a_peak[i]=Cast(Ax) ->peaks[i][0];
          T_peak[i]=Cast(Ax) ->peaks[i][1];
          theta_peak[i]=Cast(Ax) ->peaks[i][2];
          zeta_peak[i]=Cast(Ax) ->peaks[i][3];
          rho_a_peak[i]=Cast(Ax) ->peaks[i][4];
          ad_inv_peak[i]=Cast(Ax) ->peaks[i][5];
        }
    }

}
