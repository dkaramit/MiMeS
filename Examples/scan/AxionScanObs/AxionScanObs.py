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
    cpus=8,
    table_fa= logspace(10,20,150),
    len_theta=250,
    umax=500,
    TSTOP=1e-4,
    ratio_ini=1e3,
    N_convergence_max=5,
    convergence_lim=1e-2,
    inputFile="../../InputExamples/RDinput.dat",#"../../InputExamples/NSCinput.dat"
    PathToCppExecutable=r"../../Cpp/Axion/Axion.run",
    relic_obs=relicDM_obs,
    relic_err_up=0.01,
    relic_err_low=0.01,
    break_after=60*60*3,
    break_time=60,
    break_command=''
)


scan.run()