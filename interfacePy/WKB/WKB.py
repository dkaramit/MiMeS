from ..Cosmo import Hubble,s,T0, rho_crit, h_hub
from ..AxionMass import ma2
from numpy import sqrt



def relic(Tosc,theta_osc,gamma=1.):
    '''
    The axion relic abundance using the WKB approximation.
    Tosc: the oscillation temperature
    theta_osc:the  angle at T_osc (usually one uses theta_osc = theta_ini)
    gamma: entropy ratio betweem Tosc and today (gamma = S(Tosc)/S(T0)) 
    '''

    correction=(3/4)**(0.5)#this factor gives more acurate form of WKB result
    return   s(T0)/s(Tosc)/gamma*0.5*sqrt(ma2(0,1)*ma2(Tosc,1))*theta_osc**2*h_hub**2/rho_crit *correction
