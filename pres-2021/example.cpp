#include"MiMeS.hpp"
#define numeric long double //makes life easier if you define a macro for this

int main(){
    mimes::util::Timer _timer_;//use this to time it!
    
    // use chi_PATH to interpolate the axion mass.
    mimes::AxionMass<numeric> axionMass(chi_PATH,0,mimes::Cosmo<numeric>::mP);

    /*set ?$\maT^2$? for ?$T\geq T_{\rm max}$?*/
    numeric TMax=axionMass.getTMax(), chiMax=axionMass.getChiMax();    

    axionMass.set_ma2_MAX(
        [&chiMax,&TMax](numeric T, numeric fa){ return chiMax/fa/fa*std::pow(T/TMax,-8.16);}
    );  

    /*set ?$\maT^2$? for ?$T\leq T_{\rm min}$?*/
    numeric TMin=axionMass.getTMin(), chiMin=axionMass.getChiMin();    

    axionMass.set_ma2_MIN( 
        [&chiMin,&TMin](numeric T, numeric fa){ return chiMin/fa/fa;}
    );		

    /*this path contains the cosmology*/
    std::string inputFile = std::string(rootDir)+
        std::string("/UserSpace/InputExamples/MatterInput.dat");

    /*declare an instance of Axion*/
    mimes::Axion<numeric, 1, RODASPR2<numeric> > ax(0.1, 1e16, 500, 1e-4, 1e3, 10, 1e-2, 
        inputFile, &axionMass, 1e-2, 1e-8, 1e-2, 1e-9, 1e-9, 0.9, 1.2, 0.8, int(1e7) );

    /*solve the EOM!*/
    ax.solveAxion();

    std::cout<<ax.relic<<"\n";
    return 0;
}