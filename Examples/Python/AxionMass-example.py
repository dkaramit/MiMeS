from numpy import logspace, pi


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))
#load the module
import interfacePy.AxionMass as AM

fa=1e12 # f_a= 10^{12} GeV

for T in logspace(-5,5,50):
    print(
        'T=',T,'GeV\t',
        'm_a^2=', AM.ma2(T,fa),'GeV^2\t', 
        '\dfrac{dm_a^2}{dT}=',AM.dma2dT(T,fa),'GeV^2\t')

# make some plots
if True:
    import matplotlib.pyplot as plt

    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    X=logspace(-2,1,500)
    Y=[]
    for t in X:
        Y.append(AM.ma2(t,1e12)**0.5)
    sub.plot(X,Y,linestyle='-',linewidth=1,alpha=1,c='xkcd:black')
    
    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$ m_a(T) ~[{\rm GeV}]$')
    sub.yaxis.set_label_coords(-0.1,0.5) 
    sub.set_yscale('log')
    sub.set_xscale('log')

    fig.savefig('ma2_examplePlot.pdf',bbox_inches='tight')
    

    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    X=logspace(-2,1,500)
    Y=[]
    for t in X:
        Y.append(AM.ma2_approx(t,1e12)**0.5)
    sub.plot(X,Y,linestyle='--',linewidth=2,alpha=1,c='xkcd:black',label=r"approx")
    
    Y=[]
    for t in X:
        Y.append(AM.ma2(t,1e12)**0.5)
    sub.plot(X,Y,linestyle='-',linewidth=1,alpha=1,c='xkcd:red',label=r"numeric")
    
    sub.legend(bbox_to_anchor=(0.99, 0.99),borderaxespad=0., 
           borderpad=0.05,ncol=1,loc='upper right',fontsize=14,framealpha=0)

    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$ m_{a}(T) ~[{\rm GeV}]$')
    sub.yaxis.set_label_coords(-0.1,0.5) 
    sub.set_yscale('log')
    sub.set_xscale('log')
    
    fig.savefig('ma2VSma2_approx_examplePlot.pdf',bbox_inches='tight')


    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.9, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    X=logspace(-3,2,1000)
    Y=[]
    for t in X:
        Y.append(AM.dma2dT(t,1))
    sub.plot(X,Y,linestyle='--',linewidth=2,alpha=1,c='xkcd:black',label=r"exact")
    
    Y=[]
    for t in X:
        Y.append(AM.dma2dT_approx(t,1))
    sub.plot(X,Y,linestyle='-',linewidth=1,alpha=1,c='xkcd:red',label=r"approx")
    
    sub.legend(bbox_to_anchor=(0.99, 0.99),borderaxespad=0., 
           borderpad=0.05,ncol=1,loc='upper right',fontsize=14,framealpha=0)

    sub.set_xlabel(r'$T ~[{\rm GeV}]$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$ \dfrac{dm^2_{a}}{dT} ~[{\rm GeV}]$')
    sub.yaxis.set_label_coords(-0.1,0.5) 
    sub.set_yscale('symlog')
    sub.set_xscale('log')

    
    fig.savefig('dma2dTVSdma2dT_approx_examplePlot.pdf',bbox_inches='tight')
