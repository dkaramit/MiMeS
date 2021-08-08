from numpy import logspace, pi


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))
#load the module
from interfacePy.AxionMass import AxionMass


axionMass = AxionMass(r'../../src/data/chi.dat',0,1e5)

# def ma2(T,fa):
#     TQCD=150*1e-3;
#     ma20=3.1575e-05/fa/fa;
#     if T<=TQCD:
#         return ma20;
#     return ma20*pow((TQCD/T),8.16)

# axionMassFunc = AxionMass(ma2)


print(axionMassFunc.ma2(2,1))
print(axionMass.ma2(2,1))
del axionMass 
del axionMassFunc 