from ..Cosmo import T0, rho_crit, h_hub, Cosmo
from ..AxionMass import AxionMass
from numpy import sqrt,loadtxt,vectorize,array,exp

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_


def relic(Tosc,theta_osc,ma2,gamma=1.,cosmo=Cosmo(_PATH_+r'/src/data/eos2020.dat')):
    '''
    The axion relic abundance using the WKB approximation.
    Tosc: the oscillation temperature
    theta_osc:the  angle at T_osc (usually one uses theta_osc = theta_ini)
    gamma: entropy ratio betweem Tosc and today (gamma = S(T0)/S(Tosc)) 
    cosmo: instance of Cosmo
    '''

    s=cosmo.s
    correction=(3/4)**(0.5)#this factor gives more acurate form of WKB result
    return   s(T0)/s(Tosc)/gamma*0.5*sqrt(ma2(0,1)*ma2(Tosc,1))*theta_osc**2*h_hub**2/rho_crit *correction


def getPoints(fa,ma2,inputFile,cosmo=Cosmo(_PATH_+r'/src/data/eos2020.dat')):
    '''find the points you need for Tosc, gamma, and gamma osc 
    fa: PQ scale
    inputFile: a file that contains u,T,logH for the cosmology of interest
    cosmo: instance of Cosmo

    this function returns gamma, Tosc
    '''
    s=cosmo.s

    cosmology=loadtxt(inputFile) 
    u=cosmology[:,0]
    T=cosmology[:,1]
    logH=cosmology[:,2]
    ma=vectorize(lambda T:ma2(T,fa)**0.5)
    ratio=3*exp(logH)/ma(T)

    ch=ratio<=1
    tmp=cosmology[ch][0]
    Tosc=tmp[1]
    uosc=tmp[0]
    
    
    return s(cosmology[-1][1])/s(Tosc)*exp(3*(cosmology[-1][0]-uosc)),Tosc         