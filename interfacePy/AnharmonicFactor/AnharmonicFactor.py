from  ctypes import CDLL, c_longdouble, c_double

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libanfac.so')


func._anharmonicFactor.argtypes = cdouble,
#specify the return type
func._anharmonicFactor.restype = cdouble
###############################################
###---since ff returns a function pointer, FuncPY is now what ff retured.---###
anharmonicFactor=func._anharmonicFactor


if __name__=="__main__":
    pass