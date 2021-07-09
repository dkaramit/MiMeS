#!/bin/bash

# cloc --match-f='(\.cpp|\.hpp|\.py)' ./ | awk '(NR>5)'
RED='\033[0;31m'
BLUE='\033[0;34m'

srcPath="src"

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

mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
mkdir "$srcPath/misc_dir" 2> /dev/null


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