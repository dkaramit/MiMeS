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
        
        checks="AnharmonicFactor_check  AxionEOM_check  AxionMass_check  AxionSolve_check  Cosmo_check"
        
        
        for file in $checks
        do
            make exec/$file.run
            ./exec/$file.run  > $file

            if [ "`diff $file src/checks/$file`" == "" ]
            then
                echo  -e "${BLUE}check passed!\033[0;97m"
            else
                echo "{RED}Something is wrong in $file.cpp, try to compile with LONG=long. If this does not work, sent an email to dkaramit@yahoo.com.\033[0;97m"
                exit
            fi

            rm ./exec/$file.run
            rm $file
        done
    fi
fi


echo -e "${RED}License:\n\n"
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
echo  -e "\033[0;97mYou can run \"make\" now!"