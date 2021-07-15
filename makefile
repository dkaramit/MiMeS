#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 

# use long doubles in python
LONGpy=long
# use doubles in python
# LONGpy=

#######################################-Rosenbrock method-#######################################
# RODASPR2 is fairly accurate and fast enough (faster than the other two from NaBBODES), but one 
# can use the others or provide another Butcher tableu and use it.
METHOD=RODASPR2 
#METHOD=ROS34PW2
#METHOD=ROS3w



################################-the data files are here. change them from List.txt-##############
cosmoDat=$(shell cat .cosmoDat) 
axMDat=$(shell cat .axMDat)
anFDat=$(shell cat .anFDat)

DataFiles= $(cosmoDat) $(axMDat) $(anFDat)


#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

#you can inlude more things here
PATH_INCLUDE= -I./

#compiler. I use g++, and haven't tested anything else.
CC=g++ 

#c++ std
STD=c++17


FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall

Ros_Headers= $(wildcard src/Rosenbrock/*.hpp) $(wildcard src/Rosenbrock/LU/*.hpp) 
SPLINE_Headers=$(wildcard src/Interpolation/*.hpp)

AxionSolve_Headers= $(wildcard src/Axion/AxionSolve.hpp) 
AxionEOM_Headers= $(wildcard src/Axion/AxionEOM.hpp) 

PathHead=src/misc_dir/path.hpp
PathTypePy=src/misc_dir/type.py

Cosmo_Headers=$(wildcard src/Cosmo/Cosmo.cpp) $(wildcard src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard src/AxionMass/AxionMass.cpp) $(wildcard src/AxionMass/AxionMass.hpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.hpp) 

Static_Headers= $(wildcard src/static.hpp) 

all: lib exec examples

lib: lib/libCosmo.so lib/libma.so lib/libanfac.so lib/Axion_py.so
	
exec: check

#shared libraries that can be used from python
lib/libCosmo.so: $(PathHead) $(PathTypePy) $(cosmoDat) $(SPLINE_Headers) $(Cosmo_Headers) $(Static_Headers) makefile
	$(CC) -o lib/libCosmo.so src/Cosmo/Cosmo.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libma.so: $(PathHead) $(PathTypePy) $(axMDat) $(SPLINE_Headers) $(AxionMisc_Headers) $(Static_Headers) makefile
	$(CC) -o lib/libma.so src/AxionMass/AxionMass.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libanfac.so: $(PathHead) $(PathTypePy) $(anFDat) $(SPLINE_Headers) $(AxionMisc_Headers) $(Static_Headers) makefile
	$(CC) -o lib/libanfac.so src/AnharmonicFactor/AnharmonicFactor.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy) 
#######################################################################################################


#shared library for the evolution of the axion that can be used from python
Axion_py=$(wildcard src/Axion/Axion-py.cpp)
lib/Axion_py.so: $(PathHead) $(PathTypePy) $(Axion_py) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(AxionEOM_Headers) $(AxionSolve_Headers) makefile $(AxionMisc_Headers) $(Static_Headers) 
	$(CC) -o lib/Axion_py.so src/Axion/Axion-py.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy) -DMETHOD=$(METHOD)


$(PathTypePy): makefile
	@echo "from ctypes import c_$(LONGpy)double as cdouble" > $(PathTypePy)


# make the examples in Examples/Cpp
examples: $(PathHead)
	cd Examples/Cpp && $(MAKE)


#cleans whatever make all created
clean:

	rm -rf $(wildcard lib/*)
	rm -rf $(wildcard exec/*)
	rm -rf $(wildcard Examples/Python/*_examplePlot.pdf)
	rm -rf $(PathTypePy)
	cd Examples/Cpp && $(MAKE) clean
	cd Documentation/Tex && $(MAKE) clean


#deletes directories that configure.sh made
deepClean: clean

	rm -f $(wildcard Examples/scan/*.xtx)
	rm -rf lib
	rm -rf exec
	rm -rf src/misc_dir
	rm -rf src/Interpolation
	rm -rf src/Rosenbrock



##--------------------------------make checks----------------------------------------##
check: exec/AxionEOM_check.run exec/AxionSolve_check.run exec/AnharmonicFactor_check.run exec/AxionMass_check.run exec/Cosmo_check.run

Cosmo_cpp=$(wildcard src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(PathHead) $(Cosmo_cpp) $(DataFiles) $(SPLINE_Headers) makefile
	$(CC) -o exec/Cosmo_check.run src/Cosmo/checks/Cosmo_check.cpp $(FLG) 


AnFac_cpp=$(wildcard src/AnharmonicFactor/checks/AnharmonicFactor_check.cpp)
# check anharmonic factor interpolation
exec/AnharmonicFactor_check.run: $(PathHead) $(AnFac_cpp) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) makefile
	$(CC) -o exec/AnharmonicFactor_check.run src/AnharmonicFactor/checks/AnharmonicFactor_check.cpp $(FLG) -DLONG=$(LONG)

AxM_cpp=$(wildcard src/AxionMass/checks/AxionMass_check.cpp)
# check axion mass interpolation
exec/AxionMass_check.run: $(PathHead) $(AxM_cpp) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) makefile
	$(CC) -o exec/AxionMass_check.run src/AxionMass/checks/AxionMass_check.cpp $(FLG) -DLONG=$(LONG)


AxionEOM_cpp=$(wildcard src/Axion/checks/AxionEOM_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionEOM_check.run: $(AxionEOM_Headers) $(PathHead) $(AxionEOM_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers) $(Static_Headers)
	$(CC) -o exec/AxionEOM_check.run src/Axion/checks/AxionEOM_check.cpp $(FLG) -DLONG=$(LONG) 

AxionSolve_cpp=$(wildcard src/Axion/checks/AxionSolve_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionSolve_check.run: $(AxionEOM_Headers) $(AxionSolve_Headers) $(PathHead) $(AxionSolve_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers) $(Static_Headers)
	$(CC) -o exec/AxionSolve_check.run src/Axion/checks/AxionSolve_check.cpp $(FLG) -DLONG=$(LONG) -DMETHOD=$(METHOD) 


# produce the documentation pdf
doc:
	cd Documentation/Tex && $(MAKE)