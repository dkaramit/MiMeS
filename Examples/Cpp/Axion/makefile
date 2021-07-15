rootDir=../../../#the root directory of MiMeS


#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 

#######################################-Rosenbrock method-#######################################
# RODASPR2 is fairly accurate and fast enough (faster than the other two from NaBBODES), but one 
# can use the others or provide another Butcher tableu and use it.
METHOD=RODASPR2 
#METHOD=ROS34PW2
#METHOD=ROS3w



#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE= -I$(rootDir)
FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG) -DMETHOD=$(METHOD)

cosmoDat=$(shell cat .cosmoDat) 
axMDat=$(shell cat .axMDat)
anFDat=$(shell cat .anFDat)
DataFiles= $(cosmoDat) $(axMDat) $(anFDat)

DataFiles=$(wildcard $(rootDir)src/data/*.dat)
Ros_Headers= $(wildcard $(rootDir)src/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/Rosenbrock/LU/*.hpp)
SPLINE_Headers=$(wildcard $(rootDir)src/Interpolation/*.hpp)

Axion_Headers= $(wildcard $(rootDir)src/Axion/*.hpp)  

PathHead=$(rootDir)src/misc_dir/path.hpp

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.cpp) $(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard $(rootDir)src/AxionMass/AxionMass.cpp) $(wildcard $(rootDir)src/AxionMass/AxionMass.hpp) $(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.hpp) 

Static_Headers= $(wildcard $(rootDir)src/static.hpp) 

all: Axion.run

Axion.run: Axion.cpp makefile\
					$(Axion_Headers) $(AxionMisc_Headers) $(Cosmo_Headers)\
					$(DataFiles) $(PathHead) $(Static_Headers)\
					$(Ros_Headers) $(SPLINE_Headers)

	$(CC) -o Axion.run Axion.cpp $(FLG) 


make clean:
	rm -f Axion.run