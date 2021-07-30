from  ctypes import CDLL, c_longdouble, c_double

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libma.so')

CppFunc=func.ma2,func.dma2dT  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble, cdouble 
    #specify the return type
    f.restype = cdouble 
###############################################
def ma2(T,fa):
    '''
    The squared mass of the axion at temperature T and PQ scale fa (both in GeV)
    usage: ma2(T,fa)
    '''
    return func.ma2(T,fa)



def dma2dT(T,fa):
    '''
    \dfrac{dm_a^2}{dT} at temperature T and PQ scale fa (both in GeV)
    usage: dma2dT(T,fa)
    '''

    return func.dma2dT(T,fa)



if __name__=="__main__":
    pass