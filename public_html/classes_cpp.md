## Classes in ```C++```
### The ```Cosmo``` class

The ```mimes::Cosmo< LD >``` class is used to  interpolate various quantities of the plasma. It is defined inside the header file ```MiMeS/src/Cosmo/Cosmo.hpp```. The constructor is

<pre><code class="prettyprint">
	template< class LD >
	mimes::Cosmo< LD >(std::string cosmo_PATH, LD minT=0, LD maxT=mimes::Cosmo< LD >::mP)
</code></pre>
The argument ```cosmo_PATH``` is the path of the data file that contains $T$ (in $GeV$), $h_{eff}$, $g_{eff}$, with increasing $T$. The parameters ```minT``` and ```maxT``` are minimum and maximum interpolation temperatures. These temperatures are just limits, and the action interpolation is done between the closest temperatures in the data file. Moreover, beyond the interpolation temperatures, both $h_{eff}$ and $g_{eff}$ are assumed to be constants.   

Interpolation of the RDOF, allows us to define various quantities related to the plasma; that is. These quantities are given as the member functions:

<ol>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::heff(LD T)</code>: $h_{eff}$ as a function of $T$.</li> 
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::geff(LD T)</code>: $g_{eff}$ as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::dheffdT(LD T)</code>: $dh_{eff}/dT$ as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::dgeffdT(LD T)</code>: $dg_{eff}/dT$ as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::dh(LD T)</code>: $\delta_h = 1 + \frac{1}{3} \frac{d\log h_{eff}}{d\log T}$ as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::s(LD T)</code>: The entropy density of the plasma as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::rhoR(LD T)</code>: The energy density of the plasma as a function of $T$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::Cosmo< LD >::Hubble(LD T)</code>: The Hubble parameter assuming radiation dominated expansion as a function of $T$.</li>
</ol>

Moreover, there are several cosmological quantities are given as members variables:[^ref]

<ol>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::T0</code>: CMB temperature today in $GeV$.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::h_hub</code>: Dimensionless Hubble constant.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::rho_crit</code>: Critical density in $GeV^3$.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::relicDM_obs</code>: Central value of the measured DM relic abundance.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::mP</code>: Planck mass in $GeV$.</li>
</ol>


### The ```AxionMass``` class
The ```mimes::AxionMass<LD>``` class is responsible for the definition of the axion mass. The header file of this class is ```\mimes/src/AxionMass/AxionMass.hpp```. 

The class has two constructors. The first one is
<pre><code class="prettyprint">
	template< class LD >
	mimes::AxionMass<LD>(std::string chi_PATH, LD minT=0, LD maxT=mimes::Cosmo::mP)
</code></pre>

The first argument, ```chi\_PATH```, is the path to a data file that contains two columns; $T$ (in $GeV$) and $\chi$ (in $GeV^4$), with increasing $T$. The arguments
```minT``` and ```maxT``` are the interpolation limits. These limits are used in order to stop the interpolation in the closest temperatures that exist in the data 
file. That is the actual interpolation limits are $T_{\min}\geq$```minT``` and $T_{\max}\leq$```maxT```. Beyond these limits, by default, the axion mass is assumed to 
be constant. However, this can be changed by using the member functions

<pre><code class="prettyprint">
	void set_ma2_MIN(std::function<LD(LD,LD)> ma2_MIN)
	void set_ma2_MAX(std::function<LD(LD,LD)> ma2_MAX)
</code></pre>

Here, ```ma2_MIN``` and ```ma2_MAX``` are functors that define the axion mass squared beyond the interpolation limits. In order to ensure that the axion mass is 
continuous, usually we need $T_{\min}$, $T_{\max}$, $\chi(T_{ min})$, and $\chi(T_{ max})$. These values can be obtained using the member functions

<ol>
	<li> <code class="prettyprint">template< class LD > LD mimes::AxionMass<LD>::getTMin()</code>: This function returns the minimum interpolation temperature, $T_{ min}$.</li> 
	<li> <code class="prettyprint">template< class LD > LD mimes::AxionMass<LD>::getTMax()</code>: This function returns the maximum interpolation temperature, $T_{ max}$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::AxionMass<LD>::getChiMin()</code>: This function returns $\chi(T_{ min})$.</li>
	<li> <code class="prettyprint">template< class LD > LD mimes::AxionMass<LD>::getChiMax()</code>: This function returns $\chi(T_{ max})$.</li>
</ol>
An alternative way to define the axion mass is via the constructor
<pre><code class="prettyprint">
	template< class LD >
	mimes::AxionMass<LD>(std::function<LD(LD,LD)> ma2)
</code></pre>
Here, the only argument is the axion mass squared, $\tilde{m}_a(T)$, defined as a callable object.

Once an instance of the class is defined, we can get $\tilde{m}^2_a(T)$ using the member function
<pre><code class="prettyprint">
	template< class LD >	LD mimes::AxionMass<LD>::ma2(LD T, LD fa)
</code></pre>

We should note that ```ma2``` is a public ```std::function<LD(LD,LD)>``` member variable. Therefore, it can be assigned using the assignment operator. However,  in order to change its definition, we can also use the following member function:

<pre><code class="prettyprint">
	template< class LD > void mimes::AxionMass<LD>::set_ma2(std::function<LD(LD,LD)> ma2)
</code></pre>

[^ref]: Taken from [PRD](https://inspirehep.net/literature/1812251) and the [Planck Collaboration](https://inspirehep.net/literature/1682902).


### The ```Axion``` class

The ```mimes::Axion<LD,Solver,Method>``` class is the class that combines all the others, and actually solves the axion EOM. Its header file is ```\mimes/src/Axion/AxionSolve.hpp``` and its constructor is
<pre><code class="prettyprint">
 	template<class LD, const int Solver, class Method>
	mimes::Axion< LD, Solver, Method >(LD theta_i, LD fa, LD umax, LD TSTOP, 
					LD ratio_ini, unsigned int N_convergence_max, LD convergence_lim, 
					std::string inputFile, AxionMass<LD> *axionMass, LD initial_step_size=1e-2, 
					LD minimum_step_size=1e-8, LD maximum_step_size=1e-2, LD absolute_tolerance=1e-8, 
					LD relative_tolerance=1e-8, LD beta=0.9, LD fac_max=1.2, LD fac_min=0.8, 
					unsigned int maximum_No_steps=10000000)
</code></pre>
The various arguments are :