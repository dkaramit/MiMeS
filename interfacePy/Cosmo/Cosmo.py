from  ctypes import CDLL, c_longdouble, c_double

from src.misc_dir.path import _PATH_
from src.misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libCosmo.so')

CppFunc=func.heff,func.geff,func.dgeffdT,func.dheffdT,func.dh,func.rhoR,func.Hubble,func.s  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble,
    #specify the return type
    f.restype = cdouble
###############################################

heff=func.heff
geff=func.geff
dgeffdT=func.dgeffdT
dheffdT=func.dheffdT
dh=func.dh
rhoR=func.rhoR
Hubble=func.Hubble
s=func.s

func.getT0.argtypes =None
func.getT0.restype =cdouble
func.geth_hub.argtypes =None
func.geth_hub.restype =cdouble
func.getrho_crit.argtypes =None
func.getrho_crit.restype =cdouble
func.getrelicDM.argtypes =None
func.getrelicDM.restype =cdouble

T0=func.getT0()
rho_crit=func.getrho_crit()
h_hub=func.geth_hub()
relicDM_obs=func.getrelicDM()
mP=1.22e19


if __name__=="__main__":
    pass