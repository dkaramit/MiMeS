from numpy import linspace, pi


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))
#load the module
from interfacePy.AnharmonicFactor import anharmonicFactor #load the anharmonicFactor function form the AnharmonicFactor module


for theta_maX in linspace(0,pi,50):
    print(theta_maX,anharmonicFactor(theta_maX))


# plot data alongsode the interpolation (if there are large differences there is something wrong)
if False:
    import matplotlib.pyplot as plt
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.99, right=0.9,wspace=0.0,hspace=0.25)
    sub = fig.add_subplot(1,1,1)
    
    
    Y=[]
    T=linspace(0,pi,50)
    for t in T:
        Y.append(anharmonicFactor(t))
    sub.plot(T,Y,linestyle='-',linewidth=2,alpha=1,c='xkcd:black')
    
    
    sub.set_xlabel(r'$\theta_{\rm max}$')
    sub.xaxis.set_label_coords(0.5, -0.1) 
    sub.set_ylabel(r'$ \dfrac{2\sqrt{2}}{\pi \ \theta_{\rm max}^2} \ \int_{-\theta_{\rm max}}^{\theta_{\rm max}} d\theta \ \sqrt{ \cos(\theta) - \cos(\theta_{\rm max}) }$')
#     sub.set_ylabel(r'$ f\left( \theta_{\rm max} \right)$')
    sub.yaxis.set_label_coords(-0.1,0.5) 
    
    fig.savefig('anharmonic_factor_examplePlot.pdf',bbox_inches='tight')