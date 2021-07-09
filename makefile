LONG=long#use long doubles. Slower, but more accurate (and safer for extreme cases, as we avoid roundoff errors). 

#---optimization options---#
OPT=O3 #this should be fast and safe

# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work


PATH_INCLUDE=  -I"./" #you can inlude more directories things here


FLG= -$(OPT) -std=$(STD) -DLONG=$(LONG) $(PATH_INCLUDE) 

DataFiles=$(wildcard src/data/*.dat)
Ros_Headers= $(wildcard src/Rosenbrock/*.hpp) $(wildcard src/Rosenbrock/LU/*.hpp)   
SPLINE_Headers=$(wildcard src/Interpolation/*.hpp)

Axion_Headers= $(wildcard src/Axion/*.hpp)   

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

Cosmo_Headers=$(wildcard src/Cosmo/Cosmo.cpp) $(wildcard src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard src/AxionMass/AxionMass.cpp) $(wildcard src/AxionMass/AxionMass.hpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.hpp) 

Static_Headers= $(wildcard src/static.hpp) 

all: lib exec examples

lib: lib/libCosmo.so lib/libma.so lib/libanfac.so  lib/Axion_py.so
	
exec: check


#shared libraries that can be used from python
lib/libCosmo.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(Cosmo_Headers) $(Static_Headers) makefile
	$(CC)  -fPIC "src/Cosmo/Cosmo.cpp" -shared -o "lib/libCosmo.so"  $(FLG) -Wall

lib/libma.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) $(Static_Headers) makefile
	$(CC)  -fPIC "src/AxionMass/AxionMass.cpp" -shared -o "lib/libma.so"  $(FLG) -Wall

lib/libanfac.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) $(Static_Headers) makefile
	$(CC)  -fPIC "src/AnharmonicFactor/AnharmonicFactor.cpp" -shared -o "lib/libanfac.so" $(FLG) -Wall
#######################################################################################################


Axion_py=$(wildcard src/Axion/Axion-py.cpp)
#shared library for the evolution of the axion that can be used from python
lib/Axion_py.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(Axion_py) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(Axion_Headers) makefile $(AxionMisc_Headers) $(Static_Headers) lib/libCosmo.so lib/libma.so lib/libanfac.so 
	 $(CC)  -fPIC "src/Axion/Axion-py.cpp" -shared -o "lib/Axion_py.so"  $(FLG) -DMETHOD=RODASPR2  -I"src/Axion"



##---copy paths where useful data cabe found in header files. 
##---These paths can be used from c++ files to load the data  
$(PathHead):  $(DataFiles) makefile
	@echo "#ifndef PATHS_HEAD\n#define PATHS_HEAD\n"> "$(PathHead)"

	@echo "#define cosmo_PATH \"$(PWD)/src/data/eos2020.dat\" ">> "$(PathHead)"
	@echo "#define chi_PATH \"$(PWD)/src/data/chi.dat\" ">> "$(PathHead)"
	@echo "#define anharmonic_PATH \"$(PWD)/src/data/anharmonic_factor.dat\" ">> "$(PathHead)"
	@echo "#define PWD \"$(PWD)\" ">> "$(PathHead)"

	@echo "\n#endif">> "$(PathHead)"

#python files contain the current path so that python files can find it easily 
$(PathHeadPy):  $(DataFiles) makefile
	@echo "_PATH_=\"$(PWD)\" "> "$(PathHeadPy)"

#python files that define either double or long double ctypes. 
#This is extremely useful, since it allows both c++ and shared libraries used by python to have the same type definitions. 
$(PathTypePy):  $(DataFiles) makefile
	@echo "from ctypes import c_$(LONG)double as cdouble"> "$(PathTypePy)"

# make the examples in Examples/Cpp
examples: $(PathHead)
	cd Examples/Cpp && $(MAKE)


#cleans whatever make all created
clean:
	rm -rf $(wildcard lib/*)
	rm -rf $(wildcard exec/*)
	rm -rf $(wildcard src/misc_dir/*)
	cd Examples/Cpp && $(MAKE) clean


#deletes directories that configure.sh made
deepClean: clean

	rm -rf lib
	rm -rf exec
	rm -rf src/misc_dir
	rm -rf src/Interpolation
	rm -rf src/Rosenbrock



##--------------------------------make checks----------------------------------------##
check: exec/AxionEOM_check.run exec/AxionSolve_check.run exec/AnharmonicFactor_check.run exec/AxionMass_check.run exec/Cosmo_check.run

Cosmo_cpp=$(wildcard src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(PathHead)  $(Cosmo_cpp) $(DataFiles) $(SPLINE_Headers) makefile
	$(CC)  -fPIC "src/Cosmo/checks/Cosmo_check.cpp"  -o "exec/Cosmo_check.run" $(FLG) -Wall


AnFac_cpp=$(wildcard src/AnharmonicFactor/checks/AnharmonicFactor_check.cpp)
# check anharmonic factor interpolation
exec/AnharmonicFactor_check.run: $(PathHead)  $(AnFac_cpp) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers)  makefile
	$(CC)  -fPIC "src/AnharmonicFactor/checks/AnharmonicFactor_check.cpp"  -o "exec/AnharmonicFactor_check.run" $(FLG) -Wall

AxM_cpp=$(wildcard src/AxionMass/checks/AxionMass_check.cpp)
# check axion mass interpolation
exec/AxionMass_check.run: $(PathHead)  $(AxM_cpp) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers)  makefile
	$(CC)  -fPIC "src/AxionMass/checks/AxionMass_check.cpp"  -o "exec/AxionMass_check.run" $(FLG) -Wall


AxionEOM_cpp=$(wildcard src/Axion/checks/AxionEOM_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionEOM_check.run: $(Axion_Headers) $(PathHead) $(AxionEOM_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers) $(Static_Headers)
	$(CC) -o "exec/AxionEOM_check.run" "src/Axion/checks/AxionEOM_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"src/Axion"

AxionSolve_cpp=$(wildcard src/Axion/checks/AxionSolve_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionSolve_check.run: $(Axion_Headers) $(PathHead) $(AxionSolve_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers) $(Static_Headers)
	$(CC) -o "exec/AxionSolve_check.run" "src/Axion/checks/AxionSolve_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"src/Axion"
