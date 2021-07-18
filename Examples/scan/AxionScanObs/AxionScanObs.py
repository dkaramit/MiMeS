###-----------------------------------------------------------------------------###
###-------------------Example of interfacePy.ScanScript.ScanObs-----------------###
###-----------------------------------------------------------------------------###

#--------------------------------Note:--------------------------------#
###################-The scan proceeds as follows:-#####################
#-For each value of $f_a$, we find $\theta_i^{approx}$ such that------#
#-$Omega h^2 = 0.12$, assuming that $\theta_i^{approx} \ll 1$.--------#
#-Then, it scans values of $\theta_i$ close to $\theta_i^{approx}$.---# 
#-This results in a range of values of $\theta_i$ for each $f_a$, with# 
#-$\Omega h^2$ close to the observed value.---------------------------#
#---------------------------------------------------------------------#



from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../../src'))

from interfacePy.ScanScript import ScanObs 
from interfacePy.Cosmo import relicDM_obs 


from numpy import logspace

scan=ScanObs(
    cpus=16,
    table_fa= logspace(10,20,50),
    len_theta=1000,
    umax=500,
    TSTOP=1e-4,
    ratio_ini=1e3,
    N_convergence_max=5,
    convergence_lim=1e-2,
    inputFile="../../InputExamples/RDinput.dat",#"../../InputExamples/NSCinput.dat"
    PathToCppExecutable=r"../../Cpp/Axion/Axion.run",
    relic_obs=relicDM_obs,
    relic_err_up=0.001,
    relic_err_low=0.001,
    break_after=60*60*3,
    break_time=60,
    break_command='',
    initial_step_size=1e-2, 
    minimum_step_size=1e-8,
    maximum_step_size=1e-2,
    absolute_tolerance=1e-8,
    relative_tolerance=1e-8,
    beta=0.9,
    fac_max=1.2, 
    fac_min=0.8,
    maximum_No_steps=int(1e7)
)


scan.run()