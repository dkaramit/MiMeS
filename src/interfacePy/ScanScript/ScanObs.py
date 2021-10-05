# Scan for axion in radiation dominated Uinverse.
# 
#------------------Note:------------------#
# The scan proceeds as follows:
#  The for each value of $f_a$, we find $\theta_i^{\rm approx}$ such that 
# $\Omega h^2 = 0.12$, assuming that $sin(\theta_i^{\rm approx}) \approx \theta_i^{\rm approx}$. 
# Then, we scan values of $\theta_i$ close to $\theta_i^{\rm approx}$. 
# This results in a range of values of $\theta_i$ for each $f_a$, 
# with $\Omega h^2$ close to the observed value.
#-----------------------------------------#


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../'))
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))
from misc_dir.path import _PATH_

from interfacePy.Axion import Axion 
from interfacePy.Cosmo import Cosmo,mP



from numpy import pi, sqrt, abs, array, float64, argmin, savetxt, linspace 


from subprocess import check_output as subprocess_check_output
from sys import stdout as sys_stdout
from time import time
from datetime import datetime
from datetime import timedelta

from os import remove as os_remove
from os import system as os_system
from pathlib import Path



parallelScan=_PATH_+r"/src/util/parallel_scan.sh"


class ScanObs:
    def __init__(self,cpus,table_fa,len_theta,umax,TSTOP,ratio_ini,N_convergence_max,convergence_lim,inputFile,axionMass,
                PathToCppExecutable, relic_obs,relic_err_up,relic_err_low,cosmo=Cosmo(_PATH_+r'/src/data/eos2020.dat',0,mP),
                break_after=5*60,break_time=5,break_command='',
                initial_step_size=1e-2, minimum_step_size=1e-8, maximum_step_size=1e-2, 
                absolute_tolerance=1e-8, relative_tolerance=1e-8,
                beta=0.9, fac_max=1.2, fac_min=0.8, maximum_No_steps=int(1e7)):
        '''
        scan for different values of fa (in table_fa) and find the theta_i closer to relic_obs.
        The result file is timecoded (so it would be difficult to write over it), and the columns correspond to
        theta_i fa [GeV] theta_osc T_osc [GeV] relic (Omega h^2) 
        
        Comment 1: The way it works is the following:
        1) we solve for theta_i=1e-5.
        2) based on this, rescale it in order to find an appropriate theta_i such that Omegah^2 = relic_obs
        3) use this rescaled theta_i as initial point, and scan for len_theta between 
            np.linspace(min([theta_i*0.85,1]),min([theta_i*1.2,pi]),len_theta)
        4) finally, the point with Omegah^2 closer to relic_obs is stored.
        
        Comment 2: If the scan exits before it finishes, it will continue from the point it stopped.
        In order to start from the beginning, delete the file "count.dat".
        
        
        cpus: number of points to run simultaneously (No of cpus available). 
        table_fa: table of fa to scan
        len_theta: number of points to search for theta closest to relic_obs. This search happens in 
        np.linspace(min([theta_i*0.85,1]),min([theta_i*1.2,pi]),self.len_theta), with theta_i is the angle
        that results in Omega h^2=relic_obs assuming theta_i<<1.

        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM, 
        in order to make the interpolations start at this point)

        N_convergence_max and convergence_lim: integration stops when the relative difference 
        between two consecutive peaks is less than convergence_lim for N_convergence_max 
        consecutive peaks
        
        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH     
        
        axionMass: instance of AxionMass

        PathToCppExecutable: path to an executable that takes "theta_i  fa  umax  TSTOP  ratio_ini  N_convergence_max convergence_lim inputFile"
        and prints "theta_i fa theta_osc T_osc relic"

        relic_obs: central value of the relic (example, relic_obs=0.12)
        relic_err_up: upper error bar around relic_obs (example, relic_err_up=0.001)
        relic_err_low: lower error bar around relic_obs (example, relic_err_low=0.001)

        cosmo: an instance of the Cosmo class (default Cosmo('src/data/eos2020.dat',0,1.22e19))
        break_after,break_time: take a break after break_after seconds for break_time seconds
        (optional) break_command: before it takes a break, run this system command (this may be a script to send the results
        to an e-mail, or back them up)

        -----------Optional arguments------------------------
        initial_stepsize: initial step the solver takes. 

        maximum_stepsize: This limits the sepsize to an upper limit. 
        minimum_stepsize: This limits the sepsize to a lower limit. 
        
        absolute_tolerance: absolute tolerance of the RK solver

        relative_tolerance: relative tolerance of the RK solver
        Note:
        Generally, both absolute and relative tolerances should be 1e-8. 
        In some cases, however, one may need more accurate result (eg if f_a is extremely high, 
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the  
        tolerances are below 1e-8, long doubles *must* be used.

        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min. 
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
        tweak them in order to avoid instabilities.

        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished. 
        '''
        
        self.cpus=cpus
        self.Table_fa=table_fa
        self.len_theta=len_theta
        
        self.umax=umax
        self.TSTOP=TSTOP
        self.ratio_ini=ratio_ini
        self.Npeaks=N_convergence_max
        self.conv=convergence_lim
        self.inputFile=inputFile
        self.axionMass=axionMass
        
        self.PathToCppExecutable=PathToCppExecutable

        self.relic_obs=relic_obs
        self.relic_err_up,self.relic_err_low = relic_err_up,relic_err_low

        self.h_hub=cosmo.h_hub
        self.T0=cosmo.T0
        self.rho_crit=cosmo.rho_crit
        self.s=cosmo.s

        self.break_after=break_after        
        self.break_time=break_time
        self.break_command=break_command
        
        self.initial_step_size=initial_step_size
        self.minimum_step_size=minimum_step_size
        self.maximum_step_size= maximum_step_size
        self.absolute_tolerance=absolute_tolerance
        self.relative_tolerance=relative_tolerance
        self.beta=beta
        self.fac_max=fac_max
        self.fac_min=fac_min
        self.maximum_No_steps=maximum_No_steps


        self.FileDate=datetime.now()
        self.FileName = "{}".format(self.FileDate.strftime('%d-%m-%Y_%H-%M-%S')) 
        self._p = Path(self.FileName+'.dat')
        
        
        self.in_file="in._mimes_"
        self.count_file="count._mimes_"
    def run_batch(self):
        '''
        run a batch.
        '''
        # get the result     
        points=subprocess_check_output( [parallelScan,  self.PathToCppExecutable, str(self.cpus),self.in_file]).decode(sys_stdout.encoding)
        points=points.split('\n')
        

        points=array([array(i.split(),float64)  for i in points[:-1] ])
        points=array(sorted(points, key=lambda arr: arr[0]))
        absDiff=argmin(abs(points[:,4]-self.relic_obs))
        
        _=points[absDiff]
        if _[4]<=self.relic_obs+self.relic_err_up and _[4]>=self.relic_obs-self.relic_err_low:  
            File= self._p.open('ab')            
            savetxt(File,array([_]))
            File.close()

    def run(self):

        try:
            with open(self.count_file,'r') as _:
                last_batch=int(_.read())
        except:
            last_batch=0
            
        Total_batches=len(self.Table_fa)-last_batch
            
        totalT=0
        meanT=0
        ETA=0
        batch=0
        
        sleepTimer=0
        for fa in self.Table_fa[last_batch:]:
            time0=time()

            ########################---find theta_i (assuming theta_i<<1) such that Omega h^=0.12---########################

            theta_small=1e-3
            ax=Axion(theta_small,fa,self.umax,self.TSTOP,
            self.ratio_ini,self.Npeaks,self.conv,self.inputFile, self.axionMass, 
            self.initial_step_size, self.minimum_step_size, self.maximum_step_size, 
            self.absolute_tolerance, self.relative_tolerance, self.beta,
            self.fac_max, self.fac_min, self.maximum_No_steps)

            ax.solveAxion()
            ax.getPeaks()
            
            T=ax.T_peak[-1]
            theta=ax.theta_peak[-1]

            relic=ax.relic
            theta_obs=sqrt(theta**2/relic*self.relic_obs)

            relic=self.s(self.T0)/self.s(T)*0.5*sqrt(self.axionMass.ma2(0,1)*self.axionMass.ma2(T,1))*theta_obs**2*self.h_hub**2/self.rho_crit

            theta_small_i=theta_small*theta_obs/theta
            
            if theta_small_i < theta_small:
                Table_theta_i = array([theta_small_i])
            else:
                if theta_small_i>pi:
                    theta_small_i=pi
                Table_theta_i=linspace(min([theta_small_i*0.85,1]),min([theta_small_i*1.2,pi]),self.len_theta)
                            
            del ax
            ########################---END---########################
            file=open(self.in_file , 'w')
            for theta_i in Table_theta_i :
                file.write( '{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13} {14} {15} {16} \n'.
                format(theta_i, fa, self.umax, self.TSTOP, self.ratio_ini, self.Npeaks, self.conv, self.inputFile,
                self.initial_step_size, self.minimum_step_size, self.maximum_step_size, self.absolute_tolerance, self.relative_tolerance, self.beta,
                self.fac_max, self.fac_min, self.maximum_No_steps) )
            file.close()
            
            self.run_batch()

            
            batch+=1
            totalT+=time()-time0
            meanT=totalT/batch

            if self.break_after>0:
                sleepTimer+=time()-time0
                if sleepTimer>self.break_after:
                    os_system(self.break_command)
                    print("taking a break")
                    time.sleep(self.break_time)
                    
                    sleepTimer=0
                
            ETA=meanT*(Total_batches-batch)

            print('======================================== \n',
            'Completed batches:  ',batch,' out of ', Total_batches ,'\n',
            'Running for:     {0:.50s}'.format( str(timedelta(seconds=totalT))  ), '\n', 
            'ETA:             {0:.50s}'.format(str(timedelta(seconds=ETA) )  ),'\n', 
            'Time per batch:  {0:.3f} sec'.format(meanT),
            '\n========================================' )
            
            
            with open(self.count_file,'w') as _:
                _.write(str(batch+last_batch))

        os_remove(self.in_file)
        os_remove(self.count_file)
        print('Done!')
    