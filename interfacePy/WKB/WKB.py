from ..Cosmo import Hubble,s,T0, rho_crit, h_hub
from ..AxionMass import ma2
from numpy import sqrt



def relic(Tosc,theta_ini,gamma=1.):
    '''
    The axion relic abundance using the WKB approximation.
    Tosc: the oscillation temperature
    theta_ini:the initial angle
    gamma: entropy ratio betweem Tosc and today (gamma = S(Tosc)/S(T0)) 
    '''

    correction=(3/4)**(0.5)#this factor gives more acurate form of WKB result
    return   s(T0)/s(Tosc)/gamma*0.5*sqrt(ma2(0,1)*ma2(Tosc,1))*theta_ini**2*h_hub**2/rho_crit *correction
