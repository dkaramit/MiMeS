from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))

from interfacePy.Scan import Scan 
from interfacePy.Cosmo import relicDM_obs 


from numpy import logspace 

scan=Scan(
    cpus=8,
    table_fa=logspace(10,20,120),
    len_theta=200,
    umax=500,
    TSTOP=1e-4,
    ratio_ini=1e3,
    N_convergence_max=5,
    convergence_lim=1e-2,
    inputFile="../InputExamples/RDinput.dat",#"../InputExamples/NSCinput.dat"
    PathToCppExecutable=r"../Cpp/Axion/Axion.run",
    relic_obs=relicDM_obs,
    relic_err_up=0.01,
    relic_err_low=0.01,
    break_after=60*60*3,
    break_time=60,
    break_command=''
)


scan.run()