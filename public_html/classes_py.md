## Classes in ```python```

All ```python``` classes can be used similarly to ```C++```. Obviously, since there are no tempates and pointers in ```python```, the syntax is a bit different. Regarding the template arguments, they are chosen at compile-time using some macros, which can be adjusted from the ```Definitions.mk``` file inside the root directory of ```MiMeS```. Furthermore, in the corresponding ```Axion``` class, instead of pointer to ```AxionMass``` instance we just pass the instance by value. 

In order to be able to load the various modules, the following has to be included at the top of the script
<pre><code class="prettyprint">
	from sys import path as sysPath
	sysPath.append('path_to_src')
</code></pre>
where ```'path_to_src'``` is the relative or absolute path to ```MiMeS/src```.

### The ```Cosmo``` class
The ```Cosmo``` class is used to  interpolate various quantities of the plasma.[Planck Collaboration](https://inspirehep.net/literature/1682902).
The class can be imported by running 
<pre><code class="prettyprint">
	from interfacePy.Cosmo import Cosmo
</code></pre>
Its constructor is
<pre><code class="prettyprint">
	Cosmo(cosmo_PATH, minT=0, maxT=mP)
</code></pre>
The argument <code class="prettyprint">cosmo_PATH</code> is the path (a string) of a data file that contains $T$ (in GeV), $h_{eff}$, $g_{eff}$, with accenting $T$. The second and third arguments, <code class="prettyprint">minT</code> and <code class="prettyprint">maxT</code>, are minimum and maximum interpolation temperatures, with the interpolation being between the closest temperatures in the data file. Moreover, beyond these limits, both $h_{eff}$ and $g_{eff}$ are assumed to be constants. It is important to note that the class creates a <code class="prettyprint">void</code> pointer that gets recasted to <code class="prettyprint">mimes::Cosmo< LD ></code> in order to call the various member functions. This means that once an instance of <code class="prettyprint">Cosmo</code> is no longer needed, it must be deleted, in order to free the memory that it occupies. An instance, say <code class="prettyprint">cosmo</code>, is deleted using
<pre><code class="prettyprint">
	del cosmo
</code></pre>

Interpolation of the RDOF, allows us to define various quantities related to the plasma. These quantities are given as the member functions:

<ol>
	<li> <code class="prettyprint">Cosmo.heff(T)</code>: $h_{eff}$ as a function of $T$.</li> 
	<li> <code class="prettyprint">Cosmo.geff(T)</code>: $g_{eff}$ as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.dheffdT(T)</code>: $dh_{eff}/dT$ as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.dgeffdT(T)</code>: $dg_{eff}/dT$ as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.dh(T)</code>: $\delta_h = 1 + \frac{1}{3} \frac{d\log h_{eff}}{d\log T}$ as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.s(T)</code>: The entropy density of the plasma as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.rhoR(T)</code>: The energy density of the plasma as a function of $T$.</li>
	<li> <code class="prettyprint">Cosmo.Hubble(T)</code>: The Hubble parameter assuming radiation dominated expansion as a function of $T$.</li>
</ol>

Moreover, there are several cosmological quantities are given as members variables:[^ref]

<ol>
	<li> <code class="prettyprint">Cosmo.T0</code>: CMB temperature today in GeV.</li>
	<li> <code class="prettyprint">Cosmo.h_hub</code>: Dimensionless Hubble constant.</li>
	<li> <code class="prettyprint">Cosmo.rho_crit</code>: Critical density in GeV$^3$.</li>
	<li> <code class="prettyprint">Cosmo.relicDM_obs</code>: Central value of the measured DM relic abundance.</li>
	<li> <code class="prettyprint">Cosmo.mP</code>: Planck mass in GeV.</li>
</ol>
Note that these values can be directly imported from the module, without declaring an instance of the class, as 
<pre><code class="prettyprint">
	from interfacePy.Cosmo import T0, h_hub, rho_crit, relicDM_obs, mP
</pre></code>


[^ref]: Taken from [PRD](https://inspirehep.net/literature/1812251) and the [Planck Collaboration](https://inspirehep.net/literature/1682902).

### The ```AxionMass``` class
The <code class="prettyprint">AxionMass</code> class is defined in the module with the same name that can be found in the directory <code class="prettyprint">MiMeS/src/interfacePy/AxionMass</code>. This class is responsible for the definition of the axion mass. This module loads the corresponding shared library from <code class="prettyprint">MiMeS/lib/libma.so</code>, which is created by compiling <code class="prettyprint"> MiMeS/src/AxionMass/AxionMass.cpp</code> using <code class="prettyprint">make lib/libma.so</code>. Its usage is described in the <a href="Examples.html">Examples</a>. Moreover, this class is used in the same way as \cppin{mimes::AxionMass<LD>}. However, we should append in this section the definition of its member functions in <code class="prettyprint">python</code>.

The constructor is
<pre><code class="prettyprint">
	AxionMass(*args)
</pre></code>
and can be used in two different ways. 

First, one can pass three arguments, as
<pre><code class="prettyprint">
	AxionMass(chi_PAT, minT, maxT)
</pre></code>
The first argument is the path to a data file that contains two columns; $T$ (in GeV) and $chi$ (in GeV$^4$), with increasing $T$. The arguments ```minT``` and ```maxT``` are the interpolation limits. These limits are used in order to stop the interpolation in the closest temperatures in ```chi_PATH```. That is the actual interpolation limits are $T_{min}\geq$```minT``` and $T_{max}\leq$```maxT```. Beyond these limits, by default, the axion mass is assumed to be constant. However, this can be changed by using the member functions
<pre><code class="prettyprint">
	AxionMass.set_ma2_MIN(ma2_MIN)
	AxionMass.set_ma2_MAX(ma2_MAX)
</pre></code>

Here, ```ma2_MIN(T,fa)``` and ```ma2_MAX(T,fa)```, are functions (not any callable object), should take as arguments ```T``` and ```fa```, and return the axion mass squared beyond the interpolation limits. In order to ensure that the axion mass is continuous, usually we need $T_{min}$, $T_{max}$, $\chi(T_{min})$, and $\chi(T_{max})$. These values can be obtained using the member functions

<ol> 
	<li> ```AxionMass.getTMin()```: This function returns the minimum interpolation temperature, $T_{min}$. </li>
	<li> ```AxionMass.getTMax()```: This function returns the maximum interpolation temperature, $T_{max}$.</li>
	<li> ```AxionMass.getChiMin()```: This function returns $\chi(T_{min})$.</li>
	<li> ```AxionMass.getChiMax()```: This function returns $\chi(T_{max})$.</li>
</ol> 

An alternative way to define the axion mass is via the constructor
<pre><code class="prettyprint">
	AxionMass(ma2)
</pre></code>
Here, ```ma2(T,fa)``` is a function (not any callable object) that takes $T$ (in GeV) and $f_a$, and returns  $m_a^2(T)$ (in GeV). As in the other ```python``` classes, once the instances of this class are no longer needed, they must be deleted using the destructor, ```del```. 


The member function that returns $m^2_a(T)$ is
<pre><code class="prettyprint">
	AxionMassma2(T,fa)
</pre></code>
We should note that another ```ma2``` can be changed using the following member function:
<pre><code class="prettyprint">
	AxionMass.set_ma2(ma2)
</pre></code>
Again, ```ma2(T,fa)``` is a function (not any callable object) that takes $T$ (in GeV) and $f_a$, and returns  $m_a^2(T)$ (in GeV).

### The ```AxionMass``` class
The class ```Axion```, solves the axion EOM. This class is defined in ```MiMeS/interfacePy/Axion/Axion.py```, and imports the corresponding shared library. This library is compiled by running ```make lib/Axion_py.so```, and its source file is ```MiMeS/src/Axion/Axion-py.cpp```. As in the previous classes, its usage is similar to the ```C++``` version.

Its constructor is 
<pre><code class="prettyprint">
	Axion(theta_i, fa, umax, TSTOP, 
			ratio_ini, unsigned int N_convergence_max, convergence_lim, 
			inputFile, axionMass, initial_step_size=1e-2, 
			minimum_step_size=1e-8, maximum_step_size=1e-2, absolute_tolerance=1e-8, 
			relative_tolerance=1e-8, beta=0.9, fac_max=1.2, fac_min=0.8, 
			unsigned int maximum_No_steps=10000000)
</pre></code>
Again, the various arguments are discussed in the <a href="classes_cpp.html">```C++``` case</a>. Notice one important difference between this and the ```C++``` verison of this class; the instance of ```AxionMass```, ```axionMass```, is passed by value. However, internally, the constructor converts this instance to a pointer, which is then passed to the underlying ```C``` function responsible for creating the relevant instance.

The member function responsible for solving the EOM is
<pre><code class="prettyprint">
	Axion.solveAxion()
</pre></code>

Once this function is finished, the following member variables are available

<ol> 
	<li> ```Axion.T_osc```: the oscillation temperature, $T_{osc}$, in GeV.</li>
	<li> ```Axion.a_osc```: $\dfrac{a}{a_i}$ at the oscillation temperature.</li>
	<li> ```Axion.theta_osc```: $theta_{osc}$, i.e. $\theta$ at $T_{osc}$.</li>
	<li>  ```Axion.gamma```: the entropy injection between the last peak ($T=T_{peak}$) and today ($T=T_0$), $\gamma$ (entropy injection factor from $T_i$).</li>
	<li> ```Axion.relic``` The relic abundance of the axion.</li>
</ol> 

The evolution of $a/a_i, \ T, \ \theta, \ \zeta, \rho_a$ at the integration steps, is not automatically accessible to user, but they can be made so using
<pre><code class="prettyprint">
	Axion.getPoints()
</pre></code>
Then, the following member variables are filled
<ol> 
	<li> ```Axion.a```: The scale factor over its initial value, $\dfrac{a}{a_i}$.</li>
	<li> ```Axion.T```: The temperature in GeV.</li>
	<li> ```Axion.theta```: The axion angle, $\theta$.</li>
	<li> ```Axion.zeta```: The derivative of $\theta$, $\zeta \equiv \dfrac{d \theta}{d \log (a/a_i)}$.</li>
	<li> ```Axion.rho_axion```: The axion energy density in GeV$^4$.</li>
</ol> 
Moreover, the function
<pre><code class="prettyprint">
	Axion.getPeaks()
</pre></code>
fills the (```numpy```) arrays ```Axion.a_peak```, ```Axion.T_peak```, ```Axion.theta_peak```, ```Axion.zeta_peak```, ```Axion.rho_axion_peak```, and ```Axion.adiabatic_invariant``` with the quantities $a/a_i, \ T, \ \theta, \ \zeta, \rho_a, \ J$, at the peaks of the oscillation. These points are computed using linear interpolation between two integration points with a change in the sign of $\zeta$.


The local integration errors for $\theta$ and $\zeta$ are stored in	```Axion.dtheta``` and ```Axion.dzeta```, after the following function is run
<pre><code class="prettyprint">
	Axion.getErrors()
</pre></code>

Another initial condition, $\theta_i$, can be used without declaring a new instance using
<pre><code class="prettyprint">
	Axion.setTheta_i(theta_i)
</pre></code>
We should note that running this function all variables are cleared. 

As in the previous ```py``` classes, once an instance of the ```Axion``` class is no longer needed, it needs to be deleted, by calling the destructor, ```del```. 

**Important difference between the ```C++``` version** Since the axion mass is passed by value in the constructor, a change of the  ```AxionMass``` instance has no effect on the ```Axion``` instance that uses it. Therefore, if the definition of the axion mass changes, one has to declare a new instance of the ```Axion``` class. The new instance can be named using the name of the previous one, if the latter is deleted by running its destructor.
