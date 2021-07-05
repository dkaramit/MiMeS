#include<iostream>
#include "LU.hpp"
#include<cmath>

using std::cout;
using std::endl;

#define LD double

// #define indmax // run ind_max test

// #define swap //run index_swap test

// #define perm //run permutation test

// #define lup //run LUP test

// #define _rand //run random tests of Solve_LU

// #define inv_test // random tests of Inverse_LU




#ifdef _rand
    #include <cstdlib>
#endif

int main(){


    #ifdef indmax
        LD x[]={2,1,-1,2,50};
        cout<<ind_max<LD>(x,5)<<endl;
    #endif


    
    #ifdef swap
        LD x[]={2,1,-1,2,50};
        index_swap<LD>(x,4,1);
        for( LD i : x ){ cout<<i<<endl;}
    #endif
    

    #ifdef perm
        LD A[]={1,2,5,8,3};
        int P[]={2,4,0,3,1};

        LD Ap[5];

        apply_permutations_vector<LD>(A,P,5,Ap);
        for( int i =0 ; i<5 ; i++){ cout<<A[i]<<" ";}
        cout<<endl;
        for( int i =0 ; i<5 ; i++){ cout<<Ap[i]<<" ";}
        cout<<endl;
    #endif




    #ifdef lup
    const int N=5;
    LD M[N][N]={
    { 0,  2,  2 , 3 , 5},
    {-3, -1,  1 , 5 , 9},
    { 1, -1,  1 , 4 , 7},
    { 1, -1,  1 , 0 , 2},
    { 1, -1,  1 , 0 , 3}
    };


    int P[N];
    LD L[N][N], U[N][N];

    LUP<N,LD>(M,L,U,P);

    for( LD i : P ){ cout<< i<<' ';}
    cout<<endl;
    cout<<endl;
    cout<<endl;

    for (int i = 0; i < N; i++) {
    
    for (int j = 0; j < N; j++) {
        cout<< U[i][j]<<"\t";

    }
    cout<<endl;
    }
    cout<<endl;
    cout<<endl;

    for (int i = 0; i < N; i++) {
    
    for (int j = 0; j < N; j++) {
        cout<< L[i][j]<<"\t";

    }
    cout<<endl;
    }
    #endif


    #ifdef _rand
        /* 
            Random tests for Solve_LU. Basically run "runs" tests of Solve_LU with N number 
            of equations (with random coefficients of magnitude up to 100), and if 
            (M*x-b)/b > 1e-11, print it.
        */    
        int runs=10000;
        LD err[runs];


        const int N=10;
        LD M[N][N], b[N];
        LD U[N][N], L[N][N] , x[N];
        int P[N];
        
        LD tmp[N];
        
        LD tmpE;

        for(int _r=0; _r<runs ; _r++){
            for (int i = 0; i < N; i++) { for (int j = 0; j < N; j++)  {
                M[i][j]= ( rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  } 
                b[i]= (rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  
            } 
        
            LUP<N,LD>(M,L,U,P);
            Solve_LU<N,LD>(L,U,P,b,x);

            err[_r]=0;
            for (int i = 0; i < N; i++){
                dot<N,LD>(M,x,tmp);
                tmpE= fabs((tmp[i] - b[i])/b[i]) ;
                if(tmpE>err[_r] ) {err[_r] = tmpE ;}

            }
        
        
        }

        for(LD _err: err){  
            if(_err>1e-11){ cout<<_err<<endl;} 
        }
        

    #endif



    #ifdef inv_test 
    /*
    run tests for Inverse_LU. Basically run "runs" random tests of Inverse_LU of N*N matrix 
    (with random entries of magnitude up to 100), and if 
    (M*M^{-1}-1)>1e-12, print it.
    */
    
    
    const int N=10;
    LD M[N][N];
    LD Unit[N][N]={0};

    int P[N];
    LD L[N][N], U[N][N],invM[N][N], R[N][N], tmp;
    
    for(int _run=0 ; _run<10000; ++_run)
    {
        for (int i = 0; i < N; i++) 
        { 
            // Unit is initialized as zero matrix. So put 1 at Unit[i][i].
            Unit[i][i]=1;
            for (int j = 0; j < N; j++)  
            {        
                // fil M with random numbers
                M[i][j]= ( rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  

            }
        } 

        // LUP decomposition of M
        LUP<N,LD>(M,L,U,P);
        // Given LUP decomposition you can calculate the inverse. 
        Inverse_LU<N,LD>(L,U,P,invM);

        // calculate M*M^{-1}
        dot<N,LD>(M,invM,R);


        // print if M*M^{-1} - 1 > 10^{-10}
        for(int i=0; i<N; ++i){
            for(int j=0; j<N; ++j){
                tmp=fabs(R[i][j]-Unit[i][j]);
                if(tmp>1e-12){ cout<< tmp << endl;}
            }
        }
    }
    #endif



    return 0;
}