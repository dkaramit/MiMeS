# <code>MiMeS</code>
Misalignment Mechanism Solver

The documentation is available on [arXiv](https://arxiv.org/abs/2110.12253).[^doc]  


The master branch is the one where developement mostly happens, including the implementation of the new versions of [<code>NaBBODES</code>](https://github.com/dkaramit/NaBBODES.git) and [<code>SimpleSplines</code>](https://github.com/dkaramit/SimpleSplines.git) (both are being developed independently by myself). This means that the code in this branch may not always work. If you wish to just use <code>MiMeS</code>, clone the [stabe](https://github.com/dkaramit/MiMeS/tree/stable) branch:
<pre><code>git clone -b stable https://github.com/dkaramit/MiMeS.git</code></pre>

If you would like to try and run the master branch, you need to run
<pre><code>git clone https://github.com/dkaramit/MiMeS.git
cd MiMeS
git submodule init
git submodule update --remote</code></pre>


In order to begin using <code>MiMes</code>, you need a machine that can run <code>bash</code> and a version of <code>g++</code> that supports the <code>C++17</code> standard.

Once you have downloaded <code>MiMeS</code>, just run
<pre><code>cd MiMeS
bash configure.sh
make</code></pre>

This will compile the shared libraries you'll need in order to call <code>MiMeS</code> from <code>python</code>. Also, it compiles some examples written in <code>C++</code>. There are several examples <code>C++</code>  <code>python</code> (including <code>python</code> in <code>jupyter</code> notebooks) that can be found in <code>MiMeS/UserSpace</code>. Although the [documentation](https://arxiv.org/abs/2110.12253) is detailed enough, the code is also carefully commented. This mean that one can also "read" the code in order to follow how <code>MiMeS</code> works.


If you use <code>MiMeS</code>, cite it as

@article{Karamitros:2021nxi,
    author = "Karamitros, Dimitrios",
    title = "{MiMeS: Misalignment Mechanism Solver}",
    eprint = "2110.12253",
    archivePrefix = "arXiv",
    primaryClass = "hep-ph",
    month = "10",
    year = "2021"
}




Enjoy,

Dimitris



[^doc]: You can get your own version of the documentation by running <code>make doc</code>, which creates <code>MiMeS/Documentation/Tex/MiMeS.pdf</code>.
