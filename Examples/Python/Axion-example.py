from numpy import array as np_array

from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

#load the module
from interfacePy.Axion import Axion 
from interfacePy.Cosmo import Hubble

theta_i, fa=0.9445, 1e12

tmax=500
TSTOP=1e-4
ratio_ini=1e2

N_convergence_max, convergence_lim=5, 1e-2 #this is fine, but you can experiment a bit. 

#radiation dominated example
inputFile="../InputExamples/RDinput.dat" 

# non-standard cosmological example. 
# the NSC parameters (using the notation of 2012.07202) are:
# T_end=1e-1 (GeV), c=2, T_ini=1e7 (GeV), and r=1e-3
# inputFile="../InputExamples/NSCinput.dat" 


# Axion instance
ax=Axion(theta_i, fa, tmax, TSTOP, ratio_ini, N_convergence_max, convergence_lim, inputFile)

# solve the EOM (this only gives you the relic, T_osc, theta_osc, and a_osc)
ax.solve()


print(theta_i, fa, ax.theta_osc, ax.T_osc ,ax.relic)




if True:
    ax.getPeaks()#this gives you the peaks of the oscillation
    ax.getPoints()#this gives you all the points of integration

    import matplotlib.pyplot as plt

    #########-----\dot \theta-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    #this plot shows the peaks of the oscillation
    sub.plot(ax.T_peak,ax.theta_peak,linestyle=':',marker='+',color='xkcd:blue',linewidth=2)

    #this plot shows all the points
    sub.plot(ax.T,ax.theta,linestyle='-',linewidth=2,alpha=1,c='xkcd:black')

    
    sub.set_yscale('linear')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\theta$')
    sub.yaxis.set_label_coords(-0.1,0.5) 

    sub.axhline(ax.theta_osc,linestyle=':',color='xkcd:red',linewidth=1.5)
    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
   
    fig.savefig('theta-T_examplePlot.pdf',bbox_inches='tight')


    #########-----\dot \theta-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    _H_=np_array([Hubble(T) for T in ax.T])
    sub.plot(ax.T,_H_*ax.zeta,linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    _H_=np_array([Hubble(T) for T in ax.T_peak])
    sub.plot(ax.T_peak,_H_*ax.zeta_peak,linestyle=':',marker='+',color='xkcd:blue',linewidth=2)
    
    sub.set_yscale('linear')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\dfrac{d \theta}{ dt } = H \ \zeta $')
    sub.yaxis.set_label_coords(-0.1,0.5) 

    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
    fig.savefig('dot_theta-T_examplePlot.pdf',bbox_inches='tight')

    #########-----rho_a-----#########
    
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    sub.plot(ax.T,ax.rho_axion,linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    sub.plot(ax.T_peak,ax.rho_axion_peak,linestyle=':',linewidth=2,alpha=1,c='xkcd:blue')

    sub.set_yscale('log')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\rho_{a}(T) ~[{\rm GeV}^{4}]$')
    sub.yaxis.set_label_coords(-0.1,0.5) 
    
    
    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
    fig.savefig('rho_a-T_examplePlot.pdf',bbox_inches='tight')





#don't forget the destructor
del ax