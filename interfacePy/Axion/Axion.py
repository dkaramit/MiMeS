from ctypes import  CDLL, c_longdouble, c_double, c_char_p, c_void_p, c_int, CFUNCTYPE, POINTER
from numpy import array as np_array

import time

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

cint=c_int
void_p=c_void_p
char_p=c_char_p

#load the library
axionLib = CDLL(_PATH_+'/lib/Axion_py.so')

axionLib.INIT.argtypes= None
axionLib.INIT.restype = CFUNCTYPE(void_p, cdouble, cdouble, cdouble, cdouble, cdouble, cint, cdouble, char_p)

axionLib.DEL.argtypes= None
axionLib.DEL.restype = CFUNCTYPE( None, void_p)


axionLib.MAKE.argtypes= None
axionLib.MAKE.restype = CFUNCTYPE(None, void_p)

axionLib.getSize.argtypes= None
axionLib.getSize.restype = CFUNCTYPE(cint, void_p)
axionLib.getPeakSize.argtypes= None
axionLib.getPeakSize.restype = CFUNCTYPE(cint, void_p)


axionLib.getResults.argtypes=POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble), void_p
axionLib.getResults.restype=None


class Axion:
    pass
    def __init__(self, theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile):
        self.theta_i, self.fa=theta_i, fa
        
        self.voidAx=void_p()
        _file_=char_p(inputFile)
        self.voidAx=axionLib.INIT()(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max, convergence_lim, _file_)

        self.t=[]
        self.T=[]
        self.theta=[]
        self.zeta=[]

    def delete(self):
        axionLib.DEL()(self.voidAx)
        del self.voidAx
        
    def solve(self,_timeit=True):
        time0=time.time()

        axionLib.MAKE()(self.voidAx)
        pointSize=axionLib.getSize()(self.voidAx)
        peakSize=axionLib.getPeakSize()(self.voidAx)
        
        if _timeit:
            print('time: ',time.time()-time0,' s')

        Arr = cdouble * pointSize 
        ArrP = cdouble * peakSize 
        points=(cdouble * 4)()

        self.a_ai=Arr()
        self.T=Arr()
        self.theta=Arr()
        self.zeta=Arr()
        self.rho_axion=Arr()
        
        self.a_peak=ArrP()
        self.T_peak=ArrP()
        self.theta_peak=ArrP()
        self.zeta_peak=ArrP()
        self.adiabatic_invariant=ArrP()
        self.rho_axion_peak=ArrP()

        axionLib.getResults(self.a_ai,self.T,self.theta,self.zeta,self.rho_axion,
        self.a_peak,self.T_peak,self.theta_peak,self.zeta_peak,self.rho_axion_peak,self.adiabatic_invariant,points,self.voidAx)

        self.adiabatic_invariant=np_array(list(self.adiabatic_invariant))
        self.a_ai=np_array(list(self.a_ai))
        self.a_peak=np_array(list(self.a_peak))
        self.T=np_array(list(self.T))
        self.T_peak=np_array(list(self.T_peak))
        self.theta=np_array(list(self.theta))
        self.theta_peak=np_array(list(self.theta_peak))
        self.zeta=np_array(list(self.zeta))
        self.zeta_peak=np_array(list(self.zeta_peak))
        self.rho_axion=np_array(list(self.rho_axion))
        self.rho_axion_peak=np_array(list(self.rho_axion_peak))

        self.T_osc=points[0]
        self.a_ini=points[1]
        self.theta_osc=points[2]
        self.relic=points[3]


        return time.time()-time0