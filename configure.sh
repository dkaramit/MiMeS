#!/bin/bash
srcPath="src"


##------clone ODE solver and iinterpolation from my github repos------##
sh $srcPath/clone_Ros.sh
sh $srcPath/clone_Spline.sh


mkdir "lib" || true
mkdir "exec" || true
mkdir "$srcPath/misc_dir" || true