#!/bin/bash
srcPath="src"
PathHead="src/misc_dir/path.hpp"
PathHeadPy="src/misc_dir/path.py"
PathTypePy="src/misc_dir/type.py"



mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
# ---------these are needed for python and c++---------------- #
mkdir "$srcPath/misc_dir" 2> /dev/null

echo "#ifndef PATHS_HEAD
#define PATHS_HEAD

#define cosmo_PATH \"/media/200Gb/work/work_PD/MiMeS/src/data/eos2020.dat\" 
#define chi_PATH \"/media/200Gb/work/work_PD/MiMeS/src/data/chi.dat\" 
#define anharmonic_PATH \"/media/200Gb/work/work_PD/MiMeS/src/data/anharmonic_factor.dat\" 
#define PWD \"/media/200Gb/work/work_PD/MiMeS\" 

#endif
">$PathHead

echo "_PATH_=\"$PWD\" "> $PathHeadPy




##------clone ODE solver and iinterpolation from my github repos------##
if [ -d "$srcPath/Rosenbrock" ]
then
    echo "Rosenbrock exists. Nothing to do here."
else
    bash $srcPath/clone_Ros.sh
fi

if [ -d "$srcPath/Interpolation" ]
then
    echo "Spline exists. Nothing to do here."
else
    bash $srcPath/clone_Spline.sh
fi


# cloc --match-f='(\.cpp|\.hpp|\.py)' ./ | awk '(NR>5)'
RED='\033[0;31m'
BLUE='\033[0;34m'

echo -e "${RED}License:"
cat LICENSE

printf "\n\n\n"

echo  -e "${BLUE}Welcome to\n 
  __  __   _   __  __           _____ 
 |  \/  | (_) |  \/  |         / ____|
 | \  / |  _  | \  / |   ___  | (___  
 | |\/| | | | | |\/| |  / _ \  \___ \ 
 | |  | | | | | |  | | |  __/  ____) |
 |_|  |_| |_| |_|  |_|  \___| |_____/ 

"
echo  -e "\033[0;97m 
You can run \"make\" to compile everything. After that, you will find several examples in Examples/, 
and you can run the executables in exec/ in order to see if the code actually works.
"