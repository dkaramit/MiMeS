#include<iomanip>
#include"MiMeS.hpp"

using numeric = long double;//make life easier if you want to change to double

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
                    inputFile, &axionMass, 1e-2, 1e-8, 1e-2, 1e-10, 1e-10, 0.85, 1.5, 0.85,
                    int(1e7) );
    /*solve the EOM!*/
    ax.solveAxion();

    std::cout<<std::setprecision(5)
    <<"theta_i="<<ax.theta_i<<std::setw(25)<<"f_a="<<ax.fa<<" GeV\n"
    <<"theta_osc~="<<ax.theta_osc <<std::setw(20)
    <<"T_osc~="<<ax.T_osc<<"GeV \n"<<"Omega h^2="<<ax.relic<<"\n";
    
    return 0;
    }