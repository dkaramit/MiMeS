from  ctypes import CDLL, c_longdouble, c_double, c_void_p, c_char_p, CFUNCTYPE

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

void_p=c_void_p
char_p=c_char_p


#load the library
AxionMassLib = CDLL(_PATH_+'/lib/libma.so')

AxionMassLib.INIT_interpolation.argtypes=char_p, cdouble, cdouble
AxionMassLib.INIT_interpolation.restype=void_p

AxionMassLib.INIT_function.argtypes=CFUNCTYPE(cdouble, cdouble, cdouble),
AxionMassLib.INIT_function.restype=void_p

AxionMassLib.DEL.argtypes= void_p,
AxionMassLib.DEL.restype = None

AxionMassLib.ma2.argtypes=cdouble, cdouble, void_p
AxionMassLib.ma2.restype=cdouble

class AxionMass:
    '''AxionMass class: This is wrapper for the mimes::AxionMass class.
    There member function available are:
        pointer(): returns the void pointer of the underlying AxionMass instance. This is useful because 
        the AxionMass class needs a void pointer to a mimes::AxionMass instance.

        ma2(T,fa): the axion mass squared (in GeV^2) at temperature T (in GeV) and fa (in GeV)
    '''
    def __init__(self,*args):
        '''
        The constructor of the class.
        If only one agrument is given, it is assumed that it is the function of the axion mass squared.
        If three arguments are given, the first should be a path of a datafile that contains T (in GeV)
        and chi (in GeV^4), with increasing T. The second and third arguments are the minimum and maximum 
        interpolation temperatures.   
        '''
        if len(args) == 1:
            self.c_ma2=CFUNCTYPE(cdouble, cdouble, cdouble)(args[0])
            self.voidAxM=AxionMassLib.INIT_function( self.c_ma2 )

        if len(args) == 3:
            path, minT,maxT=args[0],args[1],args[2]
            _file_=char_p(bytes(path, encoding='utf-8'))
            self.voidAxM=AxionMassLib.INIT_interpolation(_file_,minT,maxT)

    def pointer(self):
        return self.voidAxM

    def __del__(self):
        AxionMassLib.DEL(self.voidAxM)
        del self.voidAxM
        del self.c_ma2

    def ma2(self,T,fa):
        return AxionMassLib.ma2(T,fa,self.pointer())



if __name__=="__main__":
    pass