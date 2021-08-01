#!/bin/bash
source Paths.mk
source Definitions.mk

# change permissions to all .sh files
for file in $(find . -type f -regex ".*\.sh");do
  echo "allow $file to be executed" 
  chmod +x $file
done


# since FormatFile.sh can change directories in future versions, let's find it first 
formatF=$(find . -type f -regex ".*FormatFile.sh")
for dataFile in $cosmoDat $axMDat $anFDat;do
  if [ -f "$dataFile" ]; then 
    echo "format $dataFile" 
    eval $formatF $dataFile 
  else 
    echo  "$dataFile does not exist! Make sure that you provide valid paths in Definitions.mk"
    exit 
  fi
done

# ---------these are needed for python and c++---------------- #
echo "make lib directory"
mkdir "lib" 2> /dev/null

echo "make exec directory"
mkdir "exec" 2> /dev/null

echo "make src/misc_dir directory"
mkdir "src/misc_dir" 2> /dev/null

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

echo "write type in $PathTypePy"
echo "from ctypes import c_"$LONGpy"double as cdouble" > $PathTypePy

echo "define _PATH_=$PWD in $PathHeadPy"
echo "_PATH_=\"$PWD\" "> $PathHeadPy

echo "write paths in $PathHead"

rm -f $PathHead
touch $PathHead
echo "#ifndef PATHS_HEAD">>$PathHead
echo "#define PATHS_HEAD">>$PathHead
echo "#include<string>">>$PathHead
echo "">>$PathHead

[  -z "$axMDat" ] || echo "constexpr const static auto chi_PATH = \"$PWD/$axMDat\";">>$PathHead
[  -z "$cosmoDat" ] || echo "constexpr const static auto cosmo_PATH = \"$PWD/$cosmoDat\";">>$PathHead
[  -z "$anFDat" ] || echo "constexpr const static auto anharmonic_PATH = \"$PWD/$anFDat\";">>$PathHead

echo "constexpr const static auto PWD = \"$PWD\"; 

#endif
">>$PathHead

echo "Run \"bash configure.sh License\" in order to read the License, or read the file named LICENSE."
if test "$1" = "License" 
then 
  echo -e "License:"
  cat LICENSE

  echo ""
  echo ""
  echo ""
fi


echo  -e "\033[1;5;31m 
  __  __   _   __  __           _____ 
 |  \/  | (_) |  \/  |         / ____|
 | \  / |  _  | \  / |   ___  | (___  
 | |\/| | | | | |\/| |  / _ \  \___ \ 
 | |  | | | | | |  | | |  __/  ____) |
 |_|  |_| |_| |_|  |_|  \___| |_____/
"


echo  -e "\033[0;97m 
You can run \"make\" to compile everything. After that, you will find several examples in UserSpace, 
and you can run the executables in exec/ in order to see if the code actually works.
"

tput smul
[ -z "$axMDat" ] && echo -e "NOTE: Data file for axion mass is not given. Please make sure to provide a function in src/static.hpp."
tput rmul