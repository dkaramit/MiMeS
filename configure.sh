#!/bin/bash
listVars=($(grep -v "^#" List.txt))

cosmoDat=${listVars[0]}
axMDat=${listVars[1]}
anFDat=${listVars[2]}

[ -f $cosmoDat ] || { echo "$cosmoDat file does not exit. You need to provide a valid path for it!" ;  exit 1; }
echo $cosmoDat > .cosmoDat

[ -f $axMDat ] || { echo "$axMDat file does not exit. You need to provide a valid path for it!" ; exit 1; }
echo $axMDat > .axMDat

[ -f $anFDat ] || { echo "$anFDat file does not exit. You need to provide a valid path for it!" ; exit 1; }
echo $anFDat > .anFDat


# ---------these are needed for python and c++---------------- #
mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
mkdir "src/misc_dir" 2> /dev/null

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py

echo "#ifndef PATHS_HEAD
#define PATHS_HEAD

#define cosmo_PATH \"$PWD/$cosmoDat\" 
#define chi_PATH \"$PWD/$axMDat\" 
#define anharmonic_PATH \"$PWD/$anFDat\" 
#define PWD \"$PWD\" 

#endif
">$PathHead

echo "_PATH_=\"$PWD\" "> $PathHeadPy


##------clone ODE solver and iinterpolation from my github repos------##
if [ -d "src/Rosenbrock" ]
then
    echo "Rosenbrock exists. Nothing to do here."
else
    curl -JLO https://github.com/dkaramit/NaBBODES/archive/refs/heads/stable.zip
    unzip NaBBODES-stable.zip 
    rm NaBBODES-stable.zip 

    cd NaBBODES-stable/Rosenbrock

    cd ../../

    mkdir src/Rosenbrock
    mkdir src/Rosenbrock/LU
    mkdir src/Rosenbrock/Jacobian

    mv  NaBBODES-stable/Rosenbrock/*.hpp                 src/Rosenbrock
    mv  NaBBODES-stable/Rosenbrock/Jacobian/Jacobian.hpp src/Rosenbrock/Jacobian
    mv  NaBBODES-stable/Rosenbrock/LU/*.hpp              src/Rosenbrock/LU

    rm -r NaBBODES-stable
fi

if [ -d "src/Interpolation" ]
then
    echo "Spline exists. Nothing to do here."
else
    curl -JLO https://github.com/dkaramit/SimpleSplines/archive/refs/heads/stable.zip

    unzip SimpleSplines-stable.zip 
    rm SimpleSplines-stable.zip 

    mkdir src/Interpolation
    mv  SimpleSplines-stable/*.hpp src/Interpolation

    rm -r SimpleSplines-stable
fi


# cloc --match-f='(\.cpp|\.hpp|\.py)' ./ | tail +6
echo -e "License:"
cat LICENSE

echo ""
echo ""
echo ""

echo  -e "\033[1;5;35m 
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
