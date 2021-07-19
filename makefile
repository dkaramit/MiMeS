-include Definitions.mk 

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

Ros_Headers= $(wildcard src/NaBBODES/Rosenbrock/*.hpp) $(wildcard src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard src/NaBBODES/RKF/*.hpp) 
SPLINE_Headers=$(wildcard src/SimpleSplines/*.hpp)


Cosmo_Headers= $(wildcard src/Cosmo/Cosmo.hpp)
Cosmo_Cpp= $(wildcard src/Cosmo/Cosmo.cpp) 

AxionMass_Headers= $(wildcard src/AxionMass/AxionMass.hpp)
AxionAnF_Headers= $(wildcard src/AnharmonicFactor/AnharmonicFactor.hpp) 

AxionMass_Cpp= $(wildcard src/AxionMass/AxionMass.cpp)
AxionAnF_Cpp=   $(wildcard src/AnharmonicFactor/AnharmonicFactor.cpp) 

AxionSolve_Headers= $(wildcard src/Axion/AxionSolve.hpp) 
AxionEOM_Headers= $(wildcard src/Axion/AxionEOM.hpp) 

Static_Funcs= $(wildcard src/static.hpp) 

all: lib exec examples

lib: lib/libCosmo.so lib/libma.so lib/libanfac.so lib/Axion_py.so
	
exec: check

#shared libraries that can be used from python
lib/libCosmo.so: $(Cosmo_Cpp)  $(PathTypePy)\
				 $(Cosmo_Headers) $(SPLINE_Headers) $(Static_Funcs)
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libma.so: $(AxionMass_Cpp)  $(PathTypePy)\
				 $(AxionMass_Headers) $(SPLINE_Headers) $(Static_Funcs)
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libanfac.so: $(AxionAnF_Cpp)  $(PathTypePy)\
				 $(AxionAnF_Headers) $(SPLINE_Headers) $(Static_Funcs)
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy) 

#shared library for the evolution of the axion that can be used from python
Axion_py=$(wildcard src/Axion/Axion-py.cpp)
lib/Axion_py.so: $(Axion_py)  $(PathTypePy)\
				 $(Cosmo_Headers) $(AxionAnF_Headers) $(AxionMass_Headers)\
				 $(AxionEOM_Headers) $(AxionSolve_Headers)\
				 $(Ros_Headers) $(RKF_Headers)  $(SPLINE_Headers)\
				 $(Static_Funcs) 
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy) -DMETHOD=$(METHOD) -Dsolver=$(Solver)


# make the examples in Examples/Cpp
examples: 
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



##--------------------------------make checks----------------------------------------##
check: exec/AxionEOM_check.run exec/AxionSolve_check.run exec/AnharmonicFactor_check.run exec/AxionMass_check.run exec/Cosmo_check.run

Cosmo_check_cpp=$(wildcard src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(Cosmo_check_cpp)  $(Cosmo_Headers) $(DataFiles) $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG)


AnFac_check_cpp=$(wildcard src/AnharmonicFactor/checks/AnharmonicFactor_check.cpp)
# check anharmonic factor interpolation
exec/AnharmonicFactor_check.run: $(AnFac_check_cpp)  $(AxionAnF_Headers)\
								 $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG)

AxM_check_cpp=$(wildcard src/AxionMass/checks/AxionMass_check.cpp)
# check axion mass interpolation
exec/AxionMass_check.run: $(AxM_check_cpp)  $(AxionMass_Headers)\
						  $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG)


AxionEOM_check_cpp=$(wildcard src/Axion/checks/AxionEOM_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionEOM_check.run: $(AxionEOM_check_cpp)  $(AxionEOM_Headers)\
						 $(Cosmo_Headers) $(AxionAnF_Headers) $(AxionMass_Headers)\
						 $(DataFiles) $(Static_Funcs) $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG) 

AxionSolve_check_cpp=$(wildcard src/Axion/checks/AxionSolve_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionSolve_check.run: $(AxionSolve_check_cpp)  $(AxionSolve_Headers)\
						   $(Cosmo_Headers) $(AxionAnF_Headers) $(AxionMass_Headers)\
						   $(DataFiles) $(Static_Funcs)\
						   $(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG) -DMETHOD=$(METHOD) -Dsolver=$(Solver)


# produce the documentation pdf
doc:
	cd Documentation/Tex && $(MAKE)