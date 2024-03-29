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


from numpy import logspace,linspace,log10,pi

scan=Scan(
    cpus=8,
    table_fa= logspace(10,19,150),
    table_theta_i=linspace(0.1,2,150),
    umax=500,
    TSTOP=1e-4,
    ratio_ini=1e3,
    N_convergence_max=5,
    convergence_lim=1e-2,
    inputFile="../../InputExamples/RDinput.dat", 
    # inputFile="../../InputExamples/MatterInput.dat", 
    # inputFile="../../InputExamples/KinationInput.dat",
    PathToCppExecutable=r"../../Cpp/Axion/Axion.run",
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