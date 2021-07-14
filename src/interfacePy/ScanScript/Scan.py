# Scan for axion in radiation dominated Uinverse.
# 
#------------------Note:------------------#
# Scan scans for the entire theta_i and f_a one provides.
#-----------------------------------------#


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../'))
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))
from misc_dir.path import _PATH_


from numpy import array, float64, savetxt


from subprocess import check_output as subprocess_check_output
from sys import stdout as sys_stdout
from time import time
from datetime import datetime
from datetime import timedelta

from os import remove as os_remove
from os import system as os_system
from pathlib import Path


parallelScan=_PATH_+r"/src/interfacePy/ScanScript/parallel_scan"


class Scan:
    def __init__(self,cpus,table_fa,table_theta_i,umax,TSTOP,ratio_ini,N_convergence_max,convergence_lim,inputFile,
                PathToCppExecutable, break_after=0,break_time=60,break_command=''):
        '''
        scan for different values of fa (in table_fa) table_theta_i. The result file is timecoded 
        (so it would be difficult to write over it), and the columns correspond to:
        theta_i fa [GeV] theta_osc T_osc [GeV] relic (Omega h^2) 
        
        
        Comment: If the scan exits before it finishes, it will continue from the point it stopped.
        In order to start from the beginning, delete the file "count.dat".
        
        
        cpus: number of points to run simultaneously (No of cpus available). 
        table_fa: table of fa to scan
        table_theta_i: table of theta_i to scan

        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        ratio_ini: integration starts when 3H/m_a<~ratio_ini (this is passed to AxionEOM, 
        in order to make the interpolations start at this point)

        N_convergence_max and convergence_lim: integration stops when the relative difference 
        between two consecutive peaks is less than convergence_lim for N_convergence_max 
        consecutive peaks
        
        inputFile: file that describes the cosmology. the columns should be: u T[GeV] logH     
        
        PathToCppExecutable: path to an executable that takes "theta_i  fa  umax  TSTOP  ratio_ini  N_convergence_max convergence_lim inputFile"
        and prints "theta_i fa theta_osc T_osc relic"


        break_after,break_time: take a break after break_after seconds for break_time seconds
        break_command: before it takes a break, run this system command (this may be a script to send the results
        to an e-mail, or back them up)
        '''
        
        self.cpus=cpus
        self.Table_fa=table_fa
        self.table_theta_i=table_theta_i
        
        self.umax=umax
        self.TSTOP=TSTOP
        self.ratio_ini=ratio_ini
        self.Npeaks=N_convergence_max
        self.conv=convergence_lim
        self.inputFile=inputFile
        
        self.PathToCppExecutable=PathToCppExecutable

        self.break_after=break_after        
        self.break_time=break_time
        self.break_command=break_command
        
        
        self.FileDate=datetime.now()
        self.FileName = "{}".format(self.FileDate.strftime('%d-%m-%Y_%H-%M-%S')) 
        self._p = Path(self.FileName+'.dat')
        
        
        self.in_file="in.xtx"
        self.count_file="count.xtx"
    def run_batch(self):
        '''
        run a batch.
        '''
        # get the result     
        points=subprocess_check_output( [parallelScan,  self.PathToCppExecutable, str(self.cpus),self.in_file]).decode(sys_stdout.encoding)
        points=points.split('\n')
        

        points=array([array(i.split(),float64)  for i in points[:-1] ])
        points=array(sorted(points, key=lambda arr: arr[0]))

        File= self._p.open('ab')            
        savetxt(File,points)
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

            file=open(self.in_file , 'w')
            for theta_i in self.table_theta_i :
                file.write( '{0} {1} {2} {3} {4} {5} {6} {7} \n'.format(theta_i, fa, self.umax, self.TSTOP, self.ratio_ini, self.Npeaks, self.conv, self.inputFile) )
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
    