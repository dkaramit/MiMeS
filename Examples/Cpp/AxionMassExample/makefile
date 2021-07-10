rootDir=../../../#the root directory of MiMeS


#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work


LONG=$(shell cat $(rootDir)/.prep.long) 
PATH_INCLUDE=  -I"$(rootDir)"

FLG= -$(OPT) -std=$(STD) -DLONG=$(LONG) $(PATH_INCLUDE) 

DataFiles=$(wildcard $(rootDir)src/data/*.dat)
SPLINE_Headers=$(wildcard $(rootDir)src/Interpolation/*.hpp)

PathHead=$(rootDir)src/misc_dir/path.hpp

AxionMass_Headers=$(wildcard $(rootDir)src/AxionMass/AxionMass.hpp)

all: AxionMassExample.run

AxionMassExample.run: AxionMassExample.cpp makefile $(AxionMass_Headers) $(DataFiles) $(PathHead) $(SPLINE_Headers)

	$(CC) -o "AxionMassExample.run" "AxionMassExample.cpp" $(FLG)


make clean:
	rm -f CosmoExample.run