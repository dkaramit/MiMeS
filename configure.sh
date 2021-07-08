#!/bin/bash

# cloc --match-f='(\.cpp|\.hpp|\.py)' ./ | awk '(NR>5)'

srcPath="src"

##------clone ODE solver and iinterpolation from my github repos------##
if [ -d "$srcPath/Rosenbrock" ]
then
    echo "Rosenbrock exists. Nothing to do here."
else
    sh $srcPath/clone_Ros.sh
fi

if [ -d "$srcPath/Interpolation" ]
then
    echo "Spline exists. Nothing to do here."
else
    sh $srcPath/clone_Spline.sh
fi

mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
mkdir "$srcPath/misc_dir" 2> /dev/null

if [ "$#" -eq "1" ]
then
    if [ "$1" == "check" ]
    then
        make check
        ./exec/AnharmonicFactor_check.run  > AnharmonicFactor_check
        ./exec/AxionMass_check.run  > AxionMass_check
        ./exec/AxionSolve_check.run > AxionSolve_check
        ./exec/AxionEOM_check.run > AxionEOM_check
        ./exec/Cosmo_check.run > Cosmo_check   
        rm ./exec/*
        rm *_check
    fi
fi


RED='\033[0;31m'
BLUE='\033[0;34m'
echo -e "${RED}License:\n\n"
cat LICENSE

printf "\n\n\n"

echo -e "${BLUE}Welcome to\n 
  __  __   _   __  __           _____ 
 |  \/  | (_) |  \/  |         / ____|
 | \  / |  _  | \  / |   ___  | (___  
 | |\/| | | | | |\/| |  / _ \  \___ \ 
 | |  | | | | | |  | | |  __/  ____) |
 |_|  |_| |_| |_|  |_|  \___| |_____/ 

"
echo -e  "\033[0;97mYou can run \"make\" now!"