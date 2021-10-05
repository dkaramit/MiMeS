from ..Cosmo import Cosmo
from ..AxionMass import AxionMass
from numpy import sqrt,loadtxt,vectorize,array,exp



cosmo=Cosmo()

def relic(Tosc,theta_osc,ma2,gamma=1.):
    '''
    The axion relic abundance using the WKB approximation.
    Tosc: the oscillation temperature
    theta_osc:the  angle at T_osc (usually one uses theta_osc = theta_ini)
    gamma: entropy ratio betweem Tosc and today (gamma = S(T0)/S(Tosc)) 
    '''

    correction=(3/4)**(0.5)#this factor gives more acurate form of WKB result
    return   cosmo.s(cosmo.T0)/cosmo.s(Tosc)/gamma*0.5*sqrt(ma2(0,1)*ma2(Tosc,1))*theta_osc**2*cosmo.h_hub**2/cosmo.rho_crit *correction




def getPoints(T_start,ratio_ini,fa,ma2,inputFile):
    '''find the points you need for Tosc, gamma, and gamma osc 
    T_start: some initial temperature (this just help to start searching for an appropriate Tini)
    ratio_ini: 3H/ma at Tini (it has to be close to 1 for the theta_osc approximation to work). This is used to find Tini
    fa: PQ scale
    inputFile: a file that contains u,T,logH forthe cosmology of interest
    
    this function returns gamma_osc, gamma, Tosc, Tini, and ratio_ini (this is close to the inut's value, but corresponts to the closest point in inputFile)
    '''
    _=loadtxt(inputFile)
    cosmology=_[_[:,1]<T_start]
    u=cosmology[:,0]
    T=cosmology[:,1]
    logH=cosmology[:,2]
    ma=vectorize(lambda T:ma2(T,fa)**0.5)
    ratio=3*exp(logH)/ma(T)

    ch=ratio<=ratio_ini
    tmp=cosmology[ch][0]
    Tini=tmp[1]
    uini=tmp[0]
    logHini=tmp[2]
    ratio_ini=3*exp(logHini)/ma(Tini)
    
    ch=ratio<=1
    tmp=cosmology[ch][0]
    Tosc=tmp[1]
    uosc=tmp[0]
    
    
    return cosmo.s(Tosc)/cosmo.s(Tini)*exp(3*(uosc-uini)),cosmo.s(cosmology[-1][1])/cosmo.s(Tosc)*exp(3*(cosmology[-1][0]-uosc)),Tosc,Tini,ratio_ini         
