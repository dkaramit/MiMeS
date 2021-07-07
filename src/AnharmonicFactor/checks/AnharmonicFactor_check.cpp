#include<iostream>
#include<fstream>


#include"src/AnharmonicFactor/AnharmonicFactor.hpp"
#include"path.hpp"


/*uncomment to print difference between interpolation and data*/
// #define printDiff

/*uncomment to print interpolation for different values of theta_max*/
#define printVal


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(){
    mimes::AnharmonicFactor<LD> anF(anharmonic_PATH);

    #ifdef printDiff
    {
        LD theta_max,anharmonic_factor;
        std::cout<<"difference between interpolation and data"<<std::endl;
        std::cout<<"Line No.\t\\theta_max\tdiff"<<std::endl;
        
        std::ifstream data_file(anharmonic_PATH);
        int i=0;
        while(true){
            data_file>>theta_max; 
            data_file>>anharmonic_factor;
            std::cout<<++i<<"\t"<<theta_max<<"\t"<<std::abs((anharmonic_factor-anF(theta_max))/anF(theta_max))<<"\n";

            if(data_file.eof()){data_file.close();break;}

        }
    }
    #endif

    #ifdef printVal
    {
        LD theta_max;
        std::cout<<"print anharmonic factor for 0<=\\theta_max<=\\pi"<<std::endl;
        std::cout<<"\\theta_max\tf(\\theta_max)"<<std::endl;
        theta_max=0;
        int N=50;
        for(int i=0; i<N; ++i){
            std::cout<<theta_max<<"\t"<<anF(theta_max)<<"\n";
            theta_max=theta_max+M_PI/(N-1.);
        }
    }    
    #endif
    
    
      
    return 0;
}