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