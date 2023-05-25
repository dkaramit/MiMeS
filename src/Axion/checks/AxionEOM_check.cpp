#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/misc_dir/mimes_paths.hpp"
#include"src/Axion/AxionEOM.hpp"
#include"src/AxionMass/AxionMass.hpp"


#define readCheck
#define interpolationVSreadCheck
#define interpolationCheck
#define axionMassCheck
#define printVal




#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 
    //model parameters
    LD fa = 1e12;
    LD ratio_ini=100;
    std::string  inputFile=std::string(rootDir)+std::string("/UserSpace/InputExamples/RDinput.dat");

    mimes::AxionMass<LD> axionMass(chi_PATH,0,mimes::Cosmo<LD>::mP);
    /*set ma2 for T>TMax*/
    LD TMax=axionMass.getTMax();    
    LD chiMax=axionMass.getChiMax();    
    axionMass.set_ma2_MAX( [&chiMax,&TMax](LD T, LD fa){return chiMax/fa/fa*std::pow(T/TMax,-8.16);});  
    
    /*set ma2 for T<TMin*/
    LD TMin=axionMass.getTMin();  
    LD chiMin=axionMass.getChiMin();    
    axionMass.set_ma2_MIN( [&chiMin,&TMin](LD T, LD fa){return chiMin/fa/fa;});

    // std::function<LD(LD,LD)> ma2 = [](LD T,LD fa){
    //     LD TQCD=150*1e-3;
    //     LD ma20=3.1575e-05/fa/fa;
    //     if(T<=TQCD){return ma20;}
    //     else{return ma20*std::pow((TQCD/T),8.16);}
    // };
    // mimes::AxionMass<LD> axionMass(ma2);

    mimes::AxionEOM<LD> axionEOM(fa, ratio_ini, inputFile, &axionMass);
    axionEOM.makeInt();

    std::cout<<"T_osc~= "<<axionEOM.T_osc<<" GeV"<<std::endl;
    std::cout<<"a_osc~= "<<std::exp(axionEOM.u_osc)<<std::endl;

    // check if the file is read
    #ifdef readCheck
    {
        std::cout<<"------------check if the file is read-----------"<<std::endl;
        for(size_t i=0; i<axionEOM.u_tab.size(); ++i ){
            std::cout<<axionEOM.u_tab[i]<<"\t"<<axionEOM.T_tab[i]<<"\t"<<axionEOM.logH2_tab[i]<<"\n";
        }
    }
    #endif



    // check if interpolation gives the same numbers on the points from the file
    #ifdef interpolationVSreadCheck
    {
        LD t, T, logH2;
        std::cout<<"------------check if interpolation gives the same numbers on the points from the file-----------"<<std::endl;
        for(size_t i=0; i<axionEOM.u_tab.size(); ++i ){
            t=axionEOM.u_tab[i];
            std::cout<<t<<"\t";
            T=axionEOM.T_tab[i];
            std::cout<<std::abs((T - axionEOM.Temperature(t))/T) <<"\t";
            logH2=axionEOM.logH2_tab[i];
            std::cout<<std::abs((logH2 - axionEOM.logH2(t))/logH2)<<"\n";
        }
    }
    #endif

    // check if interpolation works
    #ifdef interpolationCheck
    {
        std::cout<<"------------check if interpolation works-----------"<<std::endl;
        LD t=0;
        int Npoints=500;
        for(auto i=0; i<Npoints; ++i ){
            std::cout<<t<<"\t";
            std::cout<<axionEOM.Temperature(t) <<"\t";
            std::cout<<axionEOM.logH2(t)<<"\t";
            std::cout<<axionEOM.dlogH2du(t)<<"\n";
            t+=axionEOM.u_stop/(Npoints-10) ;///the last few points should be the same as I make the interpolation return T_stop and logH2_stop if I go beyond t_stop 
        }
    }
    #endif

    #ifdef axionMassCheck
    {       
        std::cout<<"------------check the axion mass-----------"<<std::endl;
        LD T,chi;
        std::cout<<"difference between interpolation and data"<<std::endl;
        std::cout<<"Line No.\tT[GeV]\tdiff"<<std::endl;
        
        std::ifstream data_file(chi_PATH);
        int i=0;
        while(true){
            data_file>>T; 
            data_file>>chi;

            std::cout<<++i<<"\t"<<T<<"\t"<<std::abs((chi-axionEOM.axionMass->ma2(T,1))/chi)<<"\n";

            if(data_file.eof()){data_file.close();break;}

        }
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
            std::cout<<std::pow(10,logT)<<"\t"<<axionEOM.axionMass->ma2(std::pow(10,logT),1)<<"\n";
            logT=logT+7./(N-1.);
        }
    }    
    #endif


    return 0;
}
