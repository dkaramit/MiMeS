from time import time; from sys import stderr #you need thes in order to print the time in stderr

#add the relative path for MiMeS/src
from sys import path as sysPath; sysPath.append('../src')	

from interfacePy.AxionMass import AxionMass #import the AxionMass class
from interfacePy.Axion import Axion #import the Axion class
from interfacePy.Cosmo import mP #import the Planck mass

def main():
    # AxionMass instance
    axionMass = AxionMass(r'../src/data/chi.dat',0,mP)

    # define ?$\maT^2$? for ?$T\leq T_{\rm min}$?
    TMin, chiMin=axionMass.getTMin(), axionMass.getChiMin()
    
    axionMass.set_ma2_MIN( lambda T,fa: chiMin/fa/fa )

    # define ?$\maT^2$? for ?$T\geq T_{\rm max}$?
    TMax, chiMax=axionMass.getTMax(), axionMass.getChiMax()

    axionMass.set_ma2_MAX( lambda T,fa: chiMax/fa/fa*pow(TMax/T,8.16))

    #in python it is more convenient to use relative paths
    inputFile="../UserSpace/InputExamples/MatterInput.dat"  

    ax = Axion(0.1, 1e16, 500, 1e-4, 1e3, 10, 1e-2, inputFile, axionMass, 
        1e-2, 1e-8, 1e-2, 1e-9, 1e-9, 0.9, 1.2, 0.8, int(1e7))

    ax.solveAxion()

    print(ax.relic)

    #once we are done we should run the destructor
    del ax
    del axionMass

if __name__ == '__main__':
    _=time()
    main()
    print(round(time()-_,3),file=stderr)