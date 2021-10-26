# <code>MiMeS</code>
Misalignment Mechanism Solver

The documentation is available on [arXiv](https://arxiv.org/abs/2110.12253).


If you wish to just use <code>MiMeS</code>, clone the [stabe](https://github.com/dkaramit/MiMeS/tree/stable) branch:
<pre><code>git clone -b stable https://github.com/dkaramit/MiMeS.git</code></pre>


Another option is the master branch, which is where developement mostly happens, including the implementation of the new versions of [<code>NaBBODES</code>](https://github.com/dkaramit/NaBBODES.git) and [<code>SimpleSplines</code>](https://github.com/dkaramit/SimpleSplines.git) (both are being developed independently by myself). This means that the code in the master branch may not always work. 

However, if you would like to try and run the master branch, you need to run
<pre><code>git clone https://github.com/dkaramit/MiMeS.git
cd MiMeS
git submodule init
git submodule update --remote</code></pre>


In order to begin using <code>MiMes</code>, you need a machine that can run <code>bash</code> and a version of <code>g++</code> that supports the <code>C++17</code> standard.

Once you have downloaded <code>MiMeS</code>, just run
<pre><code>cd MiMeS
bash configure.sh
make</code></pre>

This will compile the shared libraries you'll need in order to call <code>MiMeS</code> from <code>python</code>. Also, it compiles some examples written in <code>C++</code>. There are several examples <code>C++</code>  <code>python</code> (including <code>python</code> in <code>jupyter</code> notebooks) that can be found in <code>MiMeS/UserSpace</code>. So feel free to check them out. Detailed description of what <code>MiMeS</code> does can be found in the [documentation](https://arxiv.org/abs/2110.12253). However, for those brave enough, almost every line of code is commented, in order to be able to follow the logic yourselves.

Enjoy,

Dimitris


