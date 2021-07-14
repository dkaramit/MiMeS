###-----------------------------------------------------------------------------###
###-------------------Example of  interfacePy.ScanScript.Scan-------------------###
###-----------------------------------------------------------------------------###

#------------------------------------Note:------------------------------------#
###########-Scan: scans for the entire theta_i and f_a one provides-###########
#-----------------------------------------------------------------------------#

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../../src'))

from interfacePy.ScanScript import Scan 


from numpy import logspace,linspace

scan=Scan(
    cpus=8,
    table_fa= logspace(10,20,150),
    table_theta_i=linspace(0.2,2,50),
    umax=500,
    TSTOP=1e-4,
    ratio_ini=1e3,
    N_convergence_max=5,
    convergence_lim=1e-2,
    inputFile="../../InputExamples/RDinput.dat",#"../../InputExamples/NSCinput.dat"
    PathToCppExecutable=r"../../Cpp/Axion/Axion.run",
    break_after=60*60*3,
    break_time=60,
    break_command=''
)


scan.run()