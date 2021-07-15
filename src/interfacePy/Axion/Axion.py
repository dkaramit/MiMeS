from ctypes import  CDLL, c_longdouble, c_double, c_char_p, c_void_p, c_int, POINTER
from numpy import array as np_array

from time import time

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

cint=c_int
void_p=c_void_p
char_p=c_char_p

#load the library
axionLib = CDLL(_PATH_+'/lib/Axion_py.so')

# set the argumet and return types of the different functions
axionLib.INIT.argtypes= cdouble, cdouble, cdouble, cdouble, cdouble, cint, cdouble, char_p, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble, cint
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
    '''
    class that solves the axion eom and stores the resulting evolution of different quantities (eg the angle),
    as well as the relic. 

    Under the hood, the constructor gets a new (pointer to an) instance of 
    mimes::Axion and casts it to void*. Then every member function of this class casts this void* to 
    mimes::Axion* and calls the corresponding mimes::Axion member from the shared library.

    However, the only thing one needs to remember is to delete any instance of this class
    once the calculations are done. 
    '''
    def __init__(self, theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max,convergence_lim,inputFile,
                initial_step_size=1e-2, minimum_step_size=1e-8, maximum_step_size=1e-2, 
                absolute_tolerance=1e-8, relative_tolerance=1e-8,
                beta=0.9, fac_max=1.2, fac_min=0.8, maximum_No_steps=int(1e7)):
        '''
        The constructor of the Axion class. 
        
        theta_i: initial angle
        fa: PQ scale in GeV (the temperature dependent mass is defined as m_a^2(T) = \chi(T)/f^2)
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM, 
        in order to make the interpolations start at this point)
        
        N_convergence_max and convergence_lim: integration stops when the relative difference 
        between two consecutive peaks is less than convergence_lim for N_convergence_max 
        consecutive peaks

        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH

        -----------Optional arguments------------------------
        initial_stepsize: initial step the solver takes. 

        maximum_stepsize: This limits the sepsize to an upper limit. 
        minimum_stepsize: This limits the sepsize to a lower limit. 
        
        absolute_tolerance: absolute tolerance of the RK solver

        relative_tolerance: relative tolerance of the RK solver
        Note:
        Generally, both absolute and relative tolerances should be 1e-8. 
        In some cases, however, one may need more accurate result (eg if f_a is extremely high, 
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the  
        tolerances are below 1e-8, long doubles *must* be used.

        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min. 
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
        tweak them in order to avoid instabilities.

        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished. 
        '''
        
        self.theta_i, self.fa=theta_i, fa
        
        self.voidAx=void_p()
        _file_=char_p(bytes(inputFile, encoding='utf-8'))

        self.voidAx=axionLib.INIT(theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max, convergence_lim, _file_,
        initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance, beta,
        fac_max, fac_min, maximum_No_steps)

        self.a_peak=[]
        self.T_peak=[]
        self.theta_peak=[]
        self.zeta_peak=[]
        self.adiabatic_invariant=[]
        self.rho_axion_peak=[]

        self.a=[]
        self.T=[]
        self.theta=[]
        self.zeta=[]
        self.rho_axion=[]

    def __del__(self):
        '''
        Overloaded del.
        Also deallocates self.voidAx. 
        '''
        axionLib.DEL(self.voidAx)
        del self.voidAx

        del self.T_osc
        del self.a_osc
        del self.theta_osc
        del self.relic

        del self.a_peak
        del self.T_peak
        del self.theta_peak
        del self.zeta_peak
        del self.adiabatic_invariant
        del self.rho_axion_peak

        del self.a
        del self.T
        del self.theta
        del self.zeta
        del self.rho_axion

        
    def solve(self):
        '''
        Solve the Axion eom. 
        
        Running this function we get:  
        T_osc [GeV] (self.T_osc), a_osc/a_i (self.a_osc), \theta_osc (self.theta_osc), 
        and \Omega h^2 (in self.relic).


        This function also returns the it took to run it. 
        '''
        time0=time()

        axionLib.MAKE(self.voidAx)#run the solver
        
        #get the results
        points=(cdouble * 4)()
        axionLib.getResults(points,self.voidAx)

        self.T_osc=points[0]
        self.a_osc=points[1]
        self.theta_osc=points[2]
        self.relic=points[3]

        #return the time it took to run
        return time()-time0

    def getPeaks(self):
        '''
        This function stores the peaks of the oscillation
        in the variables (with selfexplanatory names):
        self.a_peak
        self.T_peak
        self.theta_peak
        self.zeta_peak
        self.adiabatic_invariant
        self.rho_axion_peak
        '''

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
        '''
        This function stores the points of integration
        in the variables (with selfexplanatory names):
        self.a
        self.T
        self.theta
        self.zeta
        self.rho_axion
        '''

        pointSize=axionLib.getPointSize(self.voidAx)
        Arr = cdouble * pointSize 
        
        self.a=Arr()
        self.T=Arr()
        self.theta=Arr()
        self.zeta=Arr()
        self.rho_axion=Arr()

        axionLib.getPoints(self.a,self.T,self.theta,self.zeta,self.rho_axion,self.voidAx)


        self.a=np_array(list(self.a))
        self.T=np_array(list(self.T))
        self.theta=np_array(list(self.theta))
        self.zeta=np_array(list(self.zeta))
        self.rho_axion=np_array(list(self.rho_axion))
