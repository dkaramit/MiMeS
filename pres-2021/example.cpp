    // use chi_PATH to interpolate the axion mass.
    mimes::AxionMass<long double> axionMass(chi_PATH,0,mimes::Cosmo<long double>::mP);

    /*set ?$\maT^2$? for ?$T\geq T_{\rm max}$?*/
    long double TMax=axionMass.getTMax(), chiMax=axionMass.getChiMax();    

    axionMass.set_ma2_MAX(
        [&chiMax,&TMax](long double T, long double fa){
            return chiMax/fa/fa*std::pow(T/TMax,-8.16);
        }
    );  

    /*set ?$\maT^2$? for ?$T\leq T_{\rm min}$?*/
    long double TMin=axionMass.getTMin(), chiMin=axionMass.getChiMin();    

    axionMass.set_ma2_MIN( 
        [&chiMin,&TMin](long double T, long double fa){
            return chiMin/fa/fa;
        }
    );		

    /*this path contains the cosmology*/
    std::string inputFile = std::string(rootDir)+
        std::string("/UserSpace/InputExamples/MatterInput.dat");

    /*declare an instance of Axion*/
    mimes::Axion<long double, 1, RODASPR2<long double> > ax(
        0.1, 1e16, 500, 1e-4, 1e3, 15, 1e-2, inputFile, &axionMass, 
        1e-1, 1e-8, 1e-1, 1e-11, 1e-11, 0.9, 1.2, 0.8, int(1e7)
        );

    /*solve the EOM!*/
    ax.solveAxion();
    