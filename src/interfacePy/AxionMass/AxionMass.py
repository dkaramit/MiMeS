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

AxionMassLib.getTMin.argtypes=void_p,
AxionMassLib.getTMin.restype=cdouble
AxionMassLib.getTMax.argtypes=void_p,
AxionMassLib.getTMax.restype=cdouble
AxionMassLib.getChiMin.argtypes=void_p,
AxionMassLib.getChiMin.restype=cdouble
AxionMassLib.getChiMax.argtypes=void_p,
AxionMassLib.getChiMax.restype=cdouble

AxionMassLib.set_ma2_MAX.argtypes=CFUNCTYPE(cdouble, cdouble, cdouble),void_p
AxionMassLib.set_ma2_MAX.restype=None
AxionMassLib.set_ma2_MIN.argtypes=CFUNCTYPE(cdouble, cdouble, cdouble),void_p
AxionMassLib.set_ma2_MIN.restype=None



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

        self.c_ma2=CFUNCTYPE(cdouble, cdouble, cdouble)(lambda T,fa:0)
        self.c_ma2_MAX=CFUNCTYPE(cdouble, cdouble, cdouble)(lambda T,fa:0)
        self.c_ma2_MIN=CFUNCTYPE(cdouble, cdouble, cdouble)(lambda T,fa:0)

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
        del self.c_ma2_MAX
        del self.c_ma2_MIN

    def getTMin(self):
        return AxionMassLib.getTMin(self.voidAxM)
    def getTMax(self):
        return AxionMassLib.getTMax(self.voidAxM)
    def getChiMin(self):
        return AxionMassLib.getChiMin(self.voidAxM)
    def getChiMax(self):
        return AxionMassLib.getChiMax(self.voidAxM)

    def set_ma2_MAX(self,ma2_MAX):
        self.c_ma2_MAX= CFUNCTYPE(cdouble, cdouble, cdouble)(ma2_MAX)
        AxionMassLib.set_ma2_MAX(self.c_ma2_MAX,self.voidAxM)

    def set_ma2_MIN(self,ma2_MIN):
        self.c_ma2_MIN= CFUNCTYPE(cdouble, cdouble, cdouble)(ma2_MIN)
        AxionMassLib.set_ma2_MIN(self.c_ma2_MIN,self.voidAxM)


    def ma2(self,T,fa):
        return AxionMassLib.ma2(T,fa,self.pointer())



if __name__=="__main__":
    pass