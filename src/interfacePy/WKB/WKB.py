from ..Cosmo import Hubble,s,T0, rho_crit, h_hub,heff,geff
from ..AxionMass import ma2
from numpy import sqrt



def relic(Tosc,theta_osc,gamma=1.):
    '''
    The axion relic abundance using the WKB approximation.
    Tosc: the oscillation temperature
    theta_osc:the  angle at T_osc (usually one uses theta_osc = theta_ini)
    gamma: entropy ratio betweem Tosc and today (gamma = S(T0)/S(Tosc)) 
    '''

    correction=(3/4)**(0.5)#this factor gives more acurate form of WKB result
    return   s(T0)/s(Tosc)/gamma*0.5*sqrt(ma2(0,1)*ma2(Tosc,1))*theta_osc**2*h_hub**2/rho_crit *correction



def theta_osc(Tini,ratio_ini,Tosc,theta_ini,gamma_osc=1):
    '''
    The first approximation for theta_osc.
    Tini=initial temperature . In this approximation it has to be as close to T_osc as possible, 
          while dtheta/dt ~= 0.
    ratio_ini: 3H/ma at T=Tini.
    Tosc: the oscillation temperature
    theta_ini:the angle at Tini
    gamma_osc: entropy ratio betweem Tini and Tosc (gamma_osc = S(Tosc)/S(Tini)) 
    '''
    return theta_ini*(1- (ratio_ini/3.)**(-2.)*( 1-Tini/Tosc*( heff(Tini)/heff(Tosc)*gamma_osc )**(1/3.) )**2)#work in progress