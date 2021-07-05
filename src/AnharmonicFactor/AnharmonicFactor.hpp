#ifndef Anharmonic_head
#define Anharmonic_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "Interpolation.hpp"
#include "path.hpp"

template<class LD>
class AnharmonicFactor {
    using VecLD=std::vector<LD>;
    protected:
    CubicSpline<LD> anharmonic_factor;
    VecLD theta_tab, ad_tab;
    LD max_Theta;

    public:

    AnharmonicFactor(std::string path){    
        
        unsigned int N;
        LD tmp1,tmp2;
        std::ifstream data_file(path);

        while (not data_file.eof()){
            data_file>>tmp1;
            data_file>>tmp2;
            theta_tab.push_back(tmp1);  
            ad_tab.push_back(tmp2);         
        }
        N=theta_tab.size();
        max_Theta=theta_tab[N-1];

        anharmonic_factor=CubicSpline<LD>(&theta_tab,&ad_tab);
    }

    LD mod(LD x, LD y) {
        return x - (int)(x/y) * y;
    }

    LD operator()(LD theta_max){
        // the adiabatic invariant for theta_max (ie at a point with dtheta/dt=0)

        //theta_max is positive by definition
        if(theta_max<0){theta_max*=-1;}
    
        //if theta_max>pi, then wrap it to pi, since theta_max is defined to be smaller (or equal) 
        // than pi. 
        if (theta_max>M_PI){
            LD _tmp=mod(theta_max, M_PI);

            if(_tmp==0){theta_max=M_PI;}
            else{theta_max= _tmp;}
        }
        
        // if theta_max is between the maximum tabulated theta_max and \pi, 
        // you can just use theta_max=max_Theta, since max_Theta \approx pi. 
        // This may happen if one uses a more accurate  value for \pi than the one used in
        // the tabulated results.
        if(theta_max>=max_Theta){return anharmonic_factor(max_Theta);}

     
        
        return anharmonic_factor(theta_max);
    }

};


#ifndef LONG
    #define LONG 
#endif

#ifndef LLD
    #define LLD LONG double
#endif



static AnharmonicFactor<LLD> _an_fac(anharmonic_PATH);

LLD _anharmonic_factor(LLD theta_max){
    return _an_fac(theta_max);
}


#endif