from  ctypes import CDLL, c_longdouble, c_double

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libanfac.so')


func._anharmonicFactor.argtypes = cdouble,
#specify the return type
func._anharmonicFactor.restype = cdouble
###############################################

#anharmonicFactor: returns the anharmonic factor at a theta_max
anharmonicFactor=func._anharmonicFactor


if __name__=="__main__":
    pass