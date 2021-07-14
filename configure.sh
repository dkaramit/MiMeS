#!/bin/bash
listVars=(`grep -v "^#" List.txt`)

LONG=${listVars[0]}
METHOD=${listVars[1]}
cosmoDat=${listVars[2]}
axMDat=${listVars[3]}
anFDat=${listVars[4]}

[ -f $cosmoDat ] || { echo "$cosmoDat file does not exit. You need to provide a valid path for it!" ;  exit 1; }
[ -f $axMDat ] || { echo "$axMDat file does not exit. You need to provide a valid path for it!" ; exit 1; }
[ -f $anFDat ] || { echo "$anFDat file does not exit. You need to provide a valid path for it!" ; exit 1; }

if [ "$LONG" != "true" ] && [ "$LONG" != "false" ]; then
    echo "The first line in List.txt must be true or false."
    echo "true: in order to use long doubles (Slower, but safer for extreme cases, as we avoid roundoff errors)."
    echo "false: in order to use doubles."
    exit 1
fi

if [ $LONG == "true" ]; then
    echo "long">.prep.long
else
    echo " ">.prep.long
fi

echo $METHOD>.prep.method


LONG=`cat .prep.long`

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
# ---------these are needed for python and c++---------------- #
mkdir "src/misc_dir" 2> /dev/null

echo "#ifndef PATHS_HEAD
#define PATHS_HEAD

#define cosmo_PATH \"$PWD/$cosmoDat\" 
#define chi_PATH \"$PWD/$axMDat\" 
#define anharmonic_PATH \"$PWD/$anFDat\" 
#define PWD \"$PWD\" 

#endif
">$PathHead

echo "_PATH_=\"$PWD\" "> $PathHeadPy
echo "from ctypes import c_$LONG""double as cdouble"> $PathTypePy


##------clone ODE solver and iinterpolation from my github repos------##
if [ -d "src/Rosenbrock" ]
then
    echo "Rosenbrock exists. Nothing to do here."
else
    curl -JLO https://github.com/dkaramit/NaBBODES/archive/refs/heads/stable.zip
    unzip NaBBODES-stable.zip 
    rm NaBBODES-stable.zip 

    cd NaBBODES-stable/Rosenbrock

    mv METHOD.hpp METHOD.hpp 

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
