from numpy import logspace, pi


from sys import path as sysPath
from sys import path as sysPath
sysPath.append('../../src')

#load the module
from interfacePy.AxionMass import AxionMass


axionMass = AxionMass(r'../../src/data/chi.dat',0,1e5)

# this is the axion mass squared beyond the interpolation limits for the current data 
TMax=axionMass.getTMax() 
chiMax=axionMass.getChiMax()
TMin=axionMass.getTMin() 
chiMin=axionMass.getChiMin()

axionMass.set_ma2_MAX( lambda T,fa: chiMax/fa/fa*pow(T/TMax,-8.16) )
axionMass.set_ma2_MIN( lambda T,fa: chiMin/fa/fa )


def ma2(T,fa):
    TQCD=150*1e-3;
    ma20=3.1575e-05/fa/fa;
    if T<=TQCD:
        return ma20;
    return ma20*pow((TQCD/T),8.16)
axionMassFunc = AxionMass(ma2)


print(axionMassFunc.ma2(3,1))
print(axionMass.ma2(3,1))

print(axionMass.ma2(0,1))
print(axionMassFunc.ma2(0,1))

del axionMass 
del axionMassFunc 