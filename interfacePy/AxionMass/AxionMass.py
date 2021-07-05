from  ctypes import CDLL, CFUNCTYPE, c_longdouble, c_double

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libma.so')

CppFunc=func.ma2,func.ma2_approx,func.dma2dT,func.dma2dT_approx  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = None
    #specify the return type
    f.restype = CFUNCTYPE(cdouble, cdouble, cdouble) 
###############################################
###---since ff returns a function pointer, FuncPY is now what ff retured.---###
ma2=func.ma2()
ma2_approx=func.ma2_approx()
dma2dT=func.dma2dT()
dma2dT_approx=func.dma2dT_approx()


if __name__=="__main__":
    pass