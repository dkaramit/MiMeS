from  ctypes import CDLL, CFUNCTYPE, c_longdouble, c_double

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libanfac.so')


func.anharmonic_factor.argtypes = None
#specify the return type
func.anharmonic_factor.restype = CFUNCTYPE(cdouble, cdouble) 
###############################################
###---since ff returns a function pointer, FuncPY is now what ff retured.---###
anharmonic_factor=func.anharmonic_factor()


if __name__=="__main__":
    pass