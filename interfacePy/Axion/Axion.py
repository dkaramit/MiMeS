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

axionLib.INIT.argtypes= cdouble, cdouble, cdouble, cdouble, cdouble, cint, cdouble, char_p
axionLib.INIT.restype = void_p

axionLib.DEL.argtypes= void_p,
axionLib.DEL.restype = None


axionLib.MAKE.argtypes= void_p,
axionLib.MAKE.restype = None

axionLib.getPointSize.argtypes= void_p,
axionLib.getPointSize.restype = cint

axionLib.getPeakSize.argtypes= void_p,
axionLib.getPeakSize.restype = cint



axionLib.getResults.argtypes=POINTER(cdouble), void_p
axionLib.getResults.restype=None

axionLib.getPoints.argtypes=POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),void_p
axionLib.getPoints.restype=None

axionLib.getPeaks.argtypes=POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),void_p
axionLib.getPeaks.restype=None


class Axion:
    pass
    def __init__(self, theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile):
        self.theta_i, self.fa=theta_i, fa
        
        self.voidAx=void_p()
        _file_=char_p(bytes(inputFile, encoding='utf-8'))

        self.voidAx=axionLib.INIT(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max, convergence_lim, _file_)

        self.t=[]
        self.T=[]
        self.theta=[]
        self.zeta=[]

    def delete(self):
        axionLib.DEL(self.voidAx)
        del self.voidAx

    def __del__(self):
        self.delete()
        
    def solve(self,_timeit=True):
        time0=time.time()

        axionLib.MAKE(self.voidAx)
        
        if _timeit:
            print('time: ',time.time()-time0,' s')

        points=(cdouble * 4)()

        axionLib.getResults(points,self.voidAx)

        

        self.T_osc=points[0]
        self.a_ini=points[1]
        self.theta_osc=points[2]
        self.relic=points[3]


        return time.time()-time0

    def getPeaks(self):
        peakSize=axionLib.getPeakSize(self.voidAx)
        ArrP = cdouble * peakSize 
                
        self.a_peak=ArrP()
        self.T_peak=ArrP()
        self.theta_peak=ArrP()
        self.zeta_peak=ArrP()
        self.adiabatic_invariant=ArrP()
        self.rho_axion_peak=ArrP()

        axionLib.getPeaks(self.a_peak,self.T_peak,self.theta_peak,self.zeta_peak,self.rho_axion_peak,self.adiabatic_invariant,self.voidAx)

        self.a_peak=np_array(list(self.a_peak))
        self.theta_peak=np_array(list(self.theta_peak))
        self.T_peak=np_array(list(self.T_peak))
        self.zeta_peak=np_array(list(self.zeta_peak))
        self.rho_axion_peak=np_array(list(self.rho_axion_peak))
        self.adiabatic_invariant=np_array(list(self.adiabatic_invariant))

    def getPoints(self):
        pointSize=axionLib.getPointSize(self.voidAx)
        Arr = cdouble * pointSize 
        
        self.a_ai=Arr()
        self.T=Arr()
        self.theta=Arr()
        self.zeta=Arr()
        self.rho_axion=Arr()

        axionLib.getPoints(self.a_ai,self.T,self.theta,self.zeta,self.rho_axion,self.voidAx)


        self.a_ai=np_array(list(self.a_ai))
        self.T=np_array(list(self.T))
        self.theta=np_array(list(self.theta))
        self.zeta=np_array(list(self.zeta))
        self.rho_axion=np_array(list(self.rho_axion))
