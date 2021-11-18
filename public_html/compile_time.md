### Template arguments and compile-time options

You need to choose what numeric type to use. This is done by the template argument ```LD``` which should be ```double``` (fast) or ```long double``` (accurate).


You also need to tell ```MiMeS``` which integration strategy to use. This is done by choosing template arguments:

1. ```Solver``` can be set to ```1``` for Rosenbrock (semi-implicit Runge-Kutta). 
The ```Method``` argument in this case can be:
    a. ```RODASPR2<LD>``` (4th order).
    b. ```ROS34PW2<LD>``` (3rd order).
    c. ```ROS3W<LD>``` (2rd order, ***very*** bad).
2. ```Solver can be set to 2``` for explicit RK.
The ```Method``` argument can be:
    a. ```DormandPrinceRK45<LD>``` (7th order)
    b. ```CashKarpRK45<LD>``` (5th order, ***very*** bad).
    c. ```RK45<LD>``` (5th order, ***very*** bad).

### Definitions.mk

In order to call the ```python``` interface of ```MiMeS```, we need to first call ```make lib``` in the root directory of ```MiMeS```.

Before that, we can take some time to decide what the template arguments and compilation  options should be. In the 
file ```MiMeS/Definitions.mk```, you can change the variables:
<ol>
    <li> ```LONGpy=long``` will compile the library with ```long double``` numeric types. If ```LONGpy``` is left empty (i.e. ```LONGpy=```) the  library will be compiled with ```double``` numeric types.</li>
    <li> ```SOLVER``` and ```METHOD```, as in the template arguments.</li>
</ol>

Also, in the same file, you can change compilation options:
<ol>
    <li> Compiler:</li>
    <ol>
        <li> ```CC=g++``` in order to use the ```GNU``` \CPP compiler.</li>
        <li> ```CC=clang -lstdc++``` in order to use the ```clang``` \CPP compiler.</li>
    </ol>
        <li> Optimization level:</li>
    <ol>
        <li> ```OPT=O0```: No optimization. </li>
        <li> ```O=O1```, ```O2```, or ```O3```: all these perform mostly the same (read the compiler documentation for more information on the optimization).</li>
        <li> ```OPT=Ofast```: full optimization (fast, but dangerous). </li>
    </ol>
</ol>

### Paths.mk

MiMeS is written in order to be as general and as simple to use as possible. For this reason, there is a file in <code>MiMeS/Paths.mk</code> where the user can provide paths to the files needed. The file paths are given in terms of the variables <code>cosmoDat</code>, <code>axMDat</code>, and <code>anFDat</code>; corresponding to the data for the relativistic degrees of freedom (RDOF), axion mass, and the anharmonic factor. MiMeS, by default uses the RDOF tabulated in <a href="https://arxiv.org/abs/2005.03544">2005.03544</a>, the data for the axion mass as given in <a href="https://arxiv.org/abs/1606.07494">1606.07494</a>, and a few data points for the anharmonic factor, in order to avoid computing the same integral every time MiMeS is called. It is worth noting, that the axion mass data file path can be omitted if the user intends to define a function that does not need any data.

These paths are stored as strings in ```MiMeS/src/misc_dir/path.hpp``` when ```bash configure.sh``` is run. They can be changed by changing the following variables in ```MiMeS/Paths.mk```:

1. ```cosmoDat```: Relative path to data file with $T$ (in GeV), $h_{eff}$, $g_{eff}$.
1. ```axMDat```: Relative path to data file with $T$ (in ${GeV}$) and $\chi(T)$ (in ${GeV}^4$). This variable can be ommitted if the user indents to define all masses via functions. 
1. ```anFDat```: Relative path to data file with $\theta_{max}$, $f(\theta_{max})$.
	
It is advisable that if the paths change ```bash configure.sh``` and ```make``` should be run.	


***Important***: The axion mass is assumed to be a function of $f_a$ and $T$. Especially, when it is defined from a file, it is assumed to be of the form
$$
    \tilde{m}_{a}^{2}(T)=\dfrac{\chi(T)}{f_a^2} \;.
$$
Therefore, in a model where there is no $f_a$, the user may use $f_a=1$. 

