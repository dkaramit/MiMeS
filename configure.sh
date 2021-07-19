#!/bin/bash
source Definitions.mk


# ---------these are needed for python and c++---------------- #
mkdir "lib" 2> /dev/null
mkdir "exec" 2> /dev/null
mkdir "src/misc_dir" 2> /dev/null

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

echo "from ctypes import c_$(LONGpy)double as cdouble" > $PathTypePy


echo "#ifndef PATHS_HEAD
#define PATHS_HEAD

#define cosmo_PATH \"$PWD/$cosmoDat\" 
#define chi_PATH \"$PWD/$axMDat\" 
#define anharmonic_PATH \"$PWD/$anFDat\" 
#define PWD \"$PWD\" 

#endif
">$PathHead

echo "_PATH_=\"$PWD\" "> $PathHeadPy

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
