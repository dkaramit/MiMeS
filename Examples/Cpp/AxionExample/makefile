rootDir=../../../#the root directory of MiMeS

LONG=long#use long doubles. Slower, but more accurate (and safer for extreme cases, as we avoid roundoff errors). 

#---optimization options---#
OPT=O3 #this should be fast and safe

# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work



PATH_INCLUDE=  -I"$(rootDir)"

FLG= -$(OPT) -std=$(STD) -DLONG=$(LONG) $(PATH_INCLUDE) 

DataFiles=$(wildcard $(rootDir)src/data/*.dat)
Ros_Headers= $(wildcard $(rootDir)src/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/Rosenbrock/LU/*.hpp)   
SPLINE_Headers=$(wildcard $(rootDir)src/Interpolation/*.hpp)

Axion_Headers= $(wildcard $(rootDir)src/Axion/*.hpp)   

PathHead=$(rootDir)src/misc_dir/path.hpp

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.cpp) $(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard $(rootDir)src/AxionMass/AxionMass.cpp) $(wildcard $(rootDir)src/AxionMass/AxionMass.hpp) $(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.hpp) 

Static_Headers= $(wildcard $(rootDir)src/static.hpp) 




all: AxionExample.run

AxionExample.run: AxionExample.cpp makefile\
					$(Axion_Headers) $(AxionMisc_Headers) $(Cosmo_Headers)\
					$(DataFiles) $(PathHead) $(Static_Headers)\
					$(Ros_Headers) $(SPLINE_Headers)

	$(CC) -o "AxionExample.run" "AxionExample.cpp" $(FLG) -DMETHOD=RODASPR2 


make clean:
	rm -f AxionExample.run