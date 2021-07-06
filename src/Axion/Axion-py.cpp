#include<string>
#include "src/Axion/AxionSolve.hpp"


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif


typedef void* (*initType)(LD, LD, LD, LD, LD,  unsigned int, LD, char*);
typedef void (*solveType)(void*);
typedef unsigned int (*sizeType)(void*);


void* __cc__(LD theta_i, LD fa, LD tmax, LD TSTOP, LD ratio_ini, 
            unsigned int N_convergence_max, LD convergence_lim, char* inputFile){ 
    return new mimes::Axion<LD>(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim, static_cast<std::string>(inputFile));
}

// destructor to delete the void*
void delete_ptr(void * Ax){  delete static_cast<mimes::Axion<LD>*>(Ax) ; }

void solveAxion_eom(void * Ax){  static_cast<mimes::Axion<LD>*>(Ax) -> solveAxion(); }
unsigned int returnSize(void * Ax){  return static_cast<mimes::Axion<LD>*>(Ax) -> pointSize; }
unsigned int returnPeakSize(void * Ax){  return static_cast<mimes::Axion<LD>*>(Ax) -> peakSize; }



extern "C"{

    initType INIT () {return __cc__ ; }
    solveType DEL () {return delete_ptr ; }
    solveType MAKE(){return solveAxion_eom ;}
    sizeType getSize(){return returnSize ;}
    sizeType getPeakSize(){return returnPeakSize ;}

    void getResults(LD *a, LD *T, LD *theta, LD *zeta,  LD *rho_a,
                  LD *a_peak, LD *T_peak, LD *theta_peak, LD*zeta_peak,  LD *rho_a_peak,
                  LD *ad_inv_peak, LD *points, void *Ax){
        unsigned int N = static_cast<mimes::Axion<LD>*>(Ax) ->pointSize;
        for (unsigned int i = 0; i < N; i++){
          a[i]=static_cast<mimes::Axion<LD>*>(Ax) ->points[i][0];
          T[i]=static_cast<mimes::Axion<LD>*>(Ax) ->points[i][1];
          theta[i]=static_cast<mimes::Axion<LD>*>(Ax) ->points[i][2];
          zeta[i]=static_cast<mimes::Axion<LD>*>(Ax) ->points[i][3];
          rho_a[i]=static_cast<mimes::Axion<LD>*>(Ax) ->points[i][4];

        }
        unsigned int Npeaks = static_cast<mimes::Axion<LD>*>(Ax) ->peakSize;
        for (unsigned int i = 0; i < Npeaks; i++){
          a_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][0];
          T_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][1];
          theta_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][2];
          zeta_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][3];
          rho_a_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][4];
          ad_inv_peak[i]=static_cast<mimes::Axion<LD>*>(Ax) ->peaks[i][5];
        
        }
        points[0]=static_cast<mimes::Axion<LD>*>(Ax) ->T_osc;
        points[1]=static_cast<mimes::Axion<LD>*>(Ax)->a_osc;
        points[2]=static_cast<mimes::Axion<LD>*>(Ax) ->theta_osc;
        points[3]=static_cast<mimes::Axion<LD>*>(Ax) ->relic;
    }

}
