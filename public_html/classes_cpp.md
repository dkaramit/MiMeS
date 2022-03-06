## Classes in ```C++```
### The ```Cosmo``` class

The ```mimes::Cosmo< LD >``` class is used to  interpolate various quantities of the plasma. It is defined inside the header file ```MiMeS/src/Cosmo/Cosmo.hpp```. The constructor is

<pre><code class="prettyprint">
	template< class LD >
	mimes::Cosmo< LD >(std::string cosmo_PATH, LD minT=0, LD maxT=mimes::Cosmo< LD >::mP)
</code></pre>
The argument ```cosmo_PATH``` is the path of the data file that contains $T$ (in GeV), $h_{eff}$, $g_{eff}$, with increasing $T$. The parameters ```minT``` and ```maxT``` are minimum and maximum interpolation temperatures. These temperatures are just limits, and the action interpolation is done between the closest temperatures in the data file. Moreover, beyond the interpolation temperatures, both $h_{eff}$ and $g_{eff}$ are assumed to be constants.   

Interpolation of the RDOF, allows us to define various quantities related to the plasma. These quantities are given as the member functions:

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
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::T0</code>: CMB temperature today in GeV.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::h_hub</code>: Dimensionless Hubble constant.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::rho_crit</code>: Critical density in GeV$^3$.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::relicDM_obs</code>: Central value of the measured DM relic abundance.</li>
	<li> <code class="prettyprint">template< class LD > constexpr static LD mimes::Cosmo< LD >::mP</code>: Planck mass in GeV.</li>
</ol>

[^ref]: Taken from [PRD](https://inspirehep.net/literature/1812251) and the [Planck Collaboration](https://inspirehep.net/literature/1682902).

### The ```AxionMass``` class
The ```mimes::AxionMass<LD>``` class is responsible for the definition of the axion mass. The header file of this class is ```\mimes/src/AxionMass/AxionMass.hpp```. 

The class has two constructors. The first one is
<pre><code class="prettyprint">
	template< class LD >
	mimes::AxionMass<LD>(std::string chi_PATH, LD minT=0, LD maxT=mimes::Cosmo::mP)
</code></pre>

The first argument, ```chi_PATH```, is the path to a data file that contains two columns; $T$ (in GeV) and $\chi$ (in $GeV^4$), with increasing $T$. The arguments
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



### The ```Axion``` class

The ```mimes::Axion< LD,Solver,Method >``` class is the class that combines all the others, and actually solves the axion EOM. Its header file is ```\mimes/src/Axion/AxionSolve.hpp``` and its constructor is
<pre><code class="prettyprint">
 	template< class LD, const int Solver, class Method >
	mimes::Axion< LD, Solver, Method >(LD theta_i, LD fa, LD umax, LD TSTOP, 
					LD ratio_ini, unsigned int N_convergence_max, LD convergence_lim, 
					std::string inputFile, AxionMass<LD> *axionMass, LD initial_step_size=1e-2, 
					LD minimum_step_size=1e-8, LD maximum_step_size=1e-2, LD absolute_tolerance=1e-8, 
					LD relative_tolerance=1e-8, LD beta=0.9, LD fac_max=1.2, LD fac_min=0.8, 
					unsigned int maximum_No_steps=10000000)
</code></pre>
The various arguments are :
<ol>
	<li> <code class="prettyprint">theta_i</code>: Initial angle, $\theta_i$.</li> 
	<li> <code class="prettyprint"> fa</code>: The PQ scale.</li>
	<li> <code class="prettyprint"> umax </code>: If $u>$<code class="prettyprint"> umax</code> the integration stops (remember that $u=\log(a/a_i)$). Typically, this should be a large number ($\sim 1000$), in order to avoid stopping the integration before the axion begins to evolve  adiabatically.</li>    
	<li> <code class="prettyprint"> TSTOP</code>: If the temperature drops below this, integration stops. In most cases this should be around 
	$10^{-4}$ GeV, in order to be sure that any entropy injection has stopped before integration stops should not be violated).</li>
	<li> <code class="prettyprint"> ratio_ini</code>: Integration starts when $3H/ m_a(T) \approx $<code class="prettyprint"> ratio_ini</code> (the exact point depends on the file ``<code class="prettyprint"> inputFile</code>", which we will see later). </li>
	<li>  <code class="prettyprint"> N_convergence_max</code> and <code class="prettyprint"> convergence_lim</code>: Integration stops when the relative difference between two consecutive peaks is less than <code class="prettyprint"> convergence_lim</code> for <code class="prettyprint"> N_convergence_max</code> consecutive peaks. This is the point beyond which adiabatic evolution is assumed.</li>
	<li>  <code class="prettyprint"> inputFile </code>: Relative (or absolute) path to a file that describes the cosmology. the columns should be: $u$ $T$ (in GeV) $\log H$, sorted so that $u$ increases.[^FormatFile] It is important to remember that <code class="prettyprint"> MiMeS </code> assumes that the entropy injection has stopped before the lowest temperature given in <code class="prettyprint"> inputFile</code>. Since <code class="prettyprint"> MiMeS </code> is unable to guess the cosmology beyond what is given in this file, the user has to make sure that there are data between the initial temperature (which corresponds to <code class="prettyprint"> ratio_ini</code>), and <code class="prettyprint"> TSTOP</code>.</li>
	<li> <code class="prettyprint"> axionMass</code>: An instance of the <code class="prettyprint">mimes::AxionMass<LD></code> class, passed by pointer.</li> 
	<li> <code class="prettyprint"> initial_stepsize</code> (optional): Initial step the solver takes. </li>
	<li> <code class="prettyprint"> maximum_stepsize</code> (optional): This limits the step-size to an upper limit. </li>
	<li> <code class="prettyprint"> minimum_stepsize</code> (optional): This limits the step-size to a lower limit. </li>
	<li> <code class="prettyprint"> absolute_tolerance</code> (optional): Absolute tolerance of the RK solver.</li>
	<li> <code class="prettyprint"> relative_tolerance</code> (optional): Relative tolerance of the RK solver.
	Generally, both absolute and relative tolerances should be $10^{-8}$. 
	In some cases, however, one may need more accurate result (\eg if <code class="prettyprint">fa</code> is extremely high, 
	the oscillations happen violently, and the system destabilizes). In any case, if the tolerances are below $10^{-8}$, <code class="prettyprint">LD</code> should be <code class="prettyprint">long double</code>. <code class="prettyprint"> MiMeS </code> by default uses <code class="prettyprint">long double</code> variables, in order to change it see the options available in <a href="compile_time.html">Compile-time choices</a>.</li>
	<li> <code class="prettyprint"> beta</code> (optional): Controls how agreesive the adaptation is. Generally, it should be around but less than 1.</li>
	<li> <code class="prettyprint"> fac_max</code>,  <code class="prettyprint"> fac_min</code> (optional): The stepsize does not increase more than fac_max, and less than fac_min. This ensures a better stability. Ideally, <code class="prettyprint"> fac_max</code>$=\infty$ and <code class="prettyprint"> fac_min</code>$=0$, but in reality one must tweak them in order to avoid instabilities.</li>
	<li> <code class="prettyprint"> maximum_No_steps</code> (optional): Maximum steps the solver can take. Quits if this number is reached even if integration is not finished. </li>
</ol>

[^FormatFile]: One can run <code class="prettyprint">bash MiMeS/src/FormatFile.sh inputFile</code> in order to sort it and remove any unwanted duplicates.

The member function responsible for solving the EOM is
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	void mimes::Axion< LD, Solver, Method >::solveAxion()
</code></pre>
Once this function finishes, the results are stored in several member variables.

The quantities $a/a_i, \ T, \ \theta, \ \zeta, \rho_a$, at the integration  steps are stored in
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method > 
	std::vector< std::vector< LD > > mimes::Axion< LD, Solver, Method >::points
</code></pre>

The quantities $a/a_i, \ T, \ \theta, \ \zeta, \rho_a, \ J$, at the peaks of the oscillation are stored in 
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method > 
	std::vector< std::vector< LD > > mimes::Axion< LD, Solver, Method >::peaks
</code></pre>
Note that these points are computed using linear interpolation between two integration points with a change in the sign of $\zeta$.

The local integration errors for $\theta$ and $\zeta$ are stored in
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method > 
	std::vector< LD > mimes::Axion< LD, Solver, Method >::dtheta
	
	template< class LD, const int Solver, class Method > 
	std::vector< LD > mimes::Axion< LD, Solver, Method >::dzeta
</code></pre>

Moreover, the oscillation temperature, $T_{osc}$, and the corresponding values of $a/a_i$ and $\theta$ are given in
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	LD mimes::Axion< LD, Solver, Method >::T_osc
	
	template< class LD, const int Solver, class Method >
	LD mimes::Axion< LD, Solver, Method >::a_osc

	template< class LD, const int Solver, class Method >
	LD mimes::Axion< LD, Solver, Method >::theta_osc
</code></pre>

Also, the entropy injection between the last peak ($T=T_{peak}$) and today ($T=T_0$), $\gamma$ (the entropy increase factor from $T=T_i$), is given in 
<pre><code class="prettyprint">
	template<c lass LD, const int Solver, class Method >
	LD mimes::Axion< LD, Solver, Method >::gamma
</code></pre>
The relic abundance is stored in the following member variable 
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	LD mimes::Axion< LD, Solver, Method >::relic
</code></pre>

We can set another initial condition, $\theta_i$, using 
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	void mimes::Axion< LD, Solver, Method >::setTheta_i(LD theta_i)
</code></pre>
We should note that running this function all variables are cleared. So we lose all information about the last time <code class="prettyprint">axionSolve()</code> ran. 

In case the mass of the axion is changed, we also need to remake  the interpolation (\ie run <code class="prettyprint">mimes::AxionEOM::makeInt()</code>). This is done using
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	void mimes::Axion< LD, Solver, Method >::restart()
</code></pre>
Again, this function clears all member variables. So it should be used with caution.

Finally, there is static <code class="prettyprint">mimes::Cosmo< LD ></code> member variable
<pre><code class="prettyprint">
	template< class LD, const int Solver, class Method >
	static mimes::Cosmo< LD > mimes::Axion< LD, Solver, Method >::plasma
</code></pre>
This variable can be used without an instance of the <code class="prettyprint">mimes::Axion< LD,Solver,Method ></code> class.
