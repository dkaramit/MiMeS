from numpy import array as np_array
from numpy import abs as np_abs
from time import time
from sys import stderr

from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))

#load the module
from interfacePy.Axion import Axion 
from interfacePy.Cosmo import Hubble

theta_i, fa=0.94435, 1e12

umax=500
TSTOP=1e-4
ratio_ini=1e3

N_convergence_max, convergence_lim=5, 1e-2 #this is fine, but you can experiment a bit. 

#radiation dominated example
inputFile="../InputExamples/RDinput.dat" 

# Matter Domination example. 
# the NSC parameters (using the notation of 2012.07202) are:
# T_end=1e-2 (GeV), c=3, T_ini=1e12 (GeV), and r=1e-1
# inputFile="../InputExamples/MatterInput.dat" 

# Kination Domination example. 
# the NSC parameters (using the notation of 2012.07202) are:
# T_end=0, c=6, T_ini=1e3 (GeV), and r=1e10
# inputFile="../InputExamples/KinationInput.dat" 



# options for the solver
# These variables are optional. Yoou can use the Axion class without them.
initial_step_size=1e-2; #initial step the solver takes. 
minimum_step_size=1e-8; #This limits the sepsize to an upper limit. 
maximum_step_size=1e-2; #This limits the sepsize to a lower limit.
absolute_tolerance=1e-8; #absolute tolerance of the RK solver
relative_tolerance=1e-8; #relative tolerance of the RK solver
beta=0.9; #controls how agreesive the adaptation is. Generally, it should be around but less than 1.

#The stepsize does not increase more than fac_max, and less than fac_min. 
#This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
#tweak them in order to avoid instabilities.
fac_max=1.2; 
fac_min=0.8;
maximum_No_steps=int(1e7); #maximum steps the solver can take Quits if this number is reached even if integration is not finished.

_=time()
# Axion instance
ax=Axion(theta_i, fa, umax, TSTOP, ratio_ini, N_convergence_max, convergence_lim, inputFile,
        initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, 
        relative_tolerance, beta, fac_max, fac_min, maximum_No_steps)

# solve the EOM (this only gives you the relic, T_osc, theta_osc, and a_osc)
ax.solveAxion()

print(ax.theta_i, ax.fa, ax.theta_osc, ax.T_osc ,ax.relic)
print(round(time()-_,3),file=stderr)

# change to True in order to mkae the plots
if True:
    ax.getPeaks()#this gives you the peaks of the oscillation
    ax.getPoints()#this gives you all the points of integration
    ax.getErrors()#this gives you local errors of integration

    import matplotlib.pyplot as plt

    #########-----\theta-----#########
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


    #########-----\delta \theta-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    #this plot shows all the points
    sub.plot(ax.T,np_abs(ax.dtheta),linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    
    sub.set_yscale('log')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\delta \theta$')
    sub.yaxis.set_label_coords(-0.1,0.5) 

    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
   
    fig.savefig('dtheta-T_examplePlot.pdf',bbox_inches='tight')


    #########-----\dot \theta-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    sub.plot(ax.T,ax.zeta,linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    sub.plot(ax.T_peak,ax.zeta_peak,linestyle=':',marker='+',color='xkcd:blue',linewidth=2)
    
    sub.set_yscale('linear')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\zeta $')
    sub.yaxis.set_label_coords(-0.1,0.5) 

    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
    fig.savefig('zeta-T_examplePlot.pdf',bbox_inches='tight')


    #########-----\delta \zeta-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    sub.plot(ax.T,np_abs(ax.dzeta),linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    
    sub.set_yscale('log')
    sub.set_xscale('linear')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$\delta \zeta$')
    sub.yaxis.set_label_coords(-0.1,0.5) 

    sub.axvline(ax.T_osc,linestyle='--',color='xkcd:gray',linewidth=1.5)
    fig.savefig('dzeta-T_examplePlot.pdf',bbox_inches='tight')



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