from  ctypes import CDLL, c_longdouble, c_double

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libCosmo.so')

CppFunc=func.heff,func.geff,func.dgeffdT,func.dheffdT,func.dh,func.rhoR,func.Hubble,func.s  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble,
    #specify the return type
    f.restype = cdouble
###############################################


func.getT0.argtypes =None
func.getT0.restype =cdouble
func.geth_hub.argtypes =None
func.geth_hub.restype =cdouble
func.getrho_crit.argtypes =None
func.getrho_crit.restype =cdouble
func.getrelicDM.argtypes =None
func.getrelicDM.restype =cdouble
func.getMP.argtypes =None
func.getMP.restype =cdouble



class Cosmo:
    '''Class that contains various cosmological parameters and functions.
    The available functions are:
    heff, geff, dgeffdT, dheffdT, dh, rhoR, Hubble, s. 
    All are functions of the temperature (in GeV), and correspond to the standard comsological scenario.
    The available variables are:
    T0: CMB temperature today in GeV
    rho_crit: critical density today in GeV^4
    h_hub: dimensionless hubble parameter
    relicDM_obs: central value of Omega h^2 according to Planck
    mP: Planck mass in GeV
    '''
    def __init__(self):
        # CMB temperature today in GeV
        self.T0=func.getT0()
        # critical density today in GeV^4
        self.rho_crit=func.getrho_crit()
        # dimensionless hubble parameter
        self.h_hub=func.geth_hub()
        # central value of Omega h^2 according to Planck 
        self.relicDM_obs=func.getrelicDM()
        # Planck mass in GeV
        self.mP=func.getMP()

    def heff(self,T):
        '''h_eff at temperature T [GeV]'''
        return func.heff(T)

    def geff(self,T):
        '''g_eff at temperature T [GeV] '''
        return func.geff(T)

    def dgeffdT(self,T):
        '''\\dfrac{dg_eff}{dT} at temperature T [GeV]'''
        return func.dgeffdT(T)

    def dheffdT(self,T):
        '''\\dfrac{dh_eff}{dT} at temperature T [GeV]'''
        return func.dheffdT(T)

    def dh(self,T):
        '''\\delta_h(T)=1+1/3 \frac{d log h_eff}{d log T} at temperature T [GeV]'''
        return func.dh(T)

    def rhoR(self,T):
        '''energy density of the plasma at temperature T [GeV]'''
        return func.rhoR(T)

    def Hubble(self,T): 
        '''H at temperature T [GeV]'''
        return func.Hubble(T)

    def s(self,T): 
        '''s (entropy density of the plasma) at temperature T [GeV]'''
        return func.s(T)


if __name__=="__main__":
    pass