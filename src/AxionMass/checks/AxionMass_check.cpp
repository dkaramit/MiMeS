#include<iostream>
#include<fstream>


#include"src/AxionMass/AxionMass.hpp"
#include"src/Cosmo/Cosmo.hpp"
#include"src/misc_dir/mimes_paths.hpp"


/*uncomment to print difference between interpolation and data*/
#define printDiff

/*uncomment to print interpolation for different values of theta_max*/
// #define printVal


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif



int main(){
    mimes::AxionMass<LD> axM(chi_PATH,0,mimes::Cosmo<LD>::mP);
    
    /*set ma2 for T>TMax*/
    LD TMax=axM.getTMax();    
    LD chiMax=axM.getChiMax();    
    axM.set_ma2_MAX( [&chiMax,&TMax](LD T, LD fa){return chiMax/fa/fa*std::pow(T/TMax,-8.16);});  
    
    /*set ma2 for T<TMin*/
    LD TMin=axM.getTMin();  
    LD chiMin=axM.getChiMin();    
    axM.set_ma2_MIN( [&chiMin,&TMin](LD T, LD fa){return chiMin/fa/fa;});


    // std::function<LD(LD,LD)> ma2 = [](LD T,LD fa){
    //     LD TQCD=150*1e-3;
    //     LD ma20=3.1575e-05/fa/fa;
    //     if(T<=TQCD){return ma20;}
    //     else{return ma20*std::pow((TQCD/T),8.16);}
    // };
    // mimes::AxionMass<LD> axM(ma2);

    #ifdef printDiff
    
        LD T,chi;
        std::cout<<"difference between interpolation and data"<<std::endl;
        std::cout<<"Line No.\tT[GeV]\tdiff"<<std::endl;
        
        std::ifstream data_file(chi_PATH);
        int i=0;
        while(true){
            data_file>>T; 
            data_file>>chi;


            std::cout<<++i<<"\t"<<T<<"\t"<<chi<<"\t"<<axM.ma2(T,1)<<"\t";
            
            std::cout<<std::abs((chi-axM.ma2(T,1))/chi)<<"\n";

            if(data_file.eof()){data_file.close();break;}

        }
    
    #endif

    #ifdef printVal
    {
        LD logT;
        std::cout<<"print chi for 1e-3 GeV <=T<=1e4 GeV"<<std::endl;
        std::cout<<"T[GeV]\tchi(T)[GeV^4]"<<std::endl;
        logT=-3;
        int N=150;
        for(int i=0; i<N; ++i){
            std::cout<<std::pow(10,logT)<<"\t"<<axM.ma2(std::pow(10,logT),1)<<"\n";
            logT=logT+7./(N-1.);
        }
    }    
    #endif
    
    
      
    return 0;
}