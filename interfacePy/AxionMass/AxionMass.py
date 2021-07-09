from  ctypes import CDLL, c_longdouble, c_double

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libma.so')

CppFunc=func.ma2,func.ma2_approx,func.dma2dT,func.dma2dT_approx  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble, cdouble 
    #specify the return type
    f.restype = cdouble 
###############################################
'''
The squared mass of the axion at temperature T and PQ scale fa (both in GeV)
usage: ma2(T,fa)
'''
ma2=func.ma2

'''
The approximate squared mass of the axion at temperature T and PQ scale fa (both in GeV)
usage: ma2_approx(T,fa)
'''
ma2_approx=func.ma2_approx

'''
\dfrac{dm_a^2}{dT} at temperature T and PQ scale fa (both in GeV)
usage: dma2dT(T,fa)
'''

dma2dT=func.dma2dT

'''
Approximate \dfrac{dm_a^2}{dT} at temperature T and PQ scale fa (both in GeV)
usage: dma2dT_approx(T,fa)
'''
dma2dT_approx=func.dma2dT_approx


if __name__=="__main__":
    pass