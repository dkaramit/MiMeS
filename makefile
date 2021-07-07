
CC=g++ #compiler. I use g++, and haven't tested anything else. 

LONG=long#use long doubles. Slower, but more accurate. 

#---optimization options---#
OPT=O3 #this is fast and safe
# OPT=Ofast #this is fast but can cause issues (I haven't observed any, though)

#c++ std lib
STD=c++17


SPLINE_FLG= -I"src/Interpolation"
PATH_INCLUDE=  -I"./" -I"./src" -I"src/misc_dir" 

ROS_FLG=-I"src/Rosenbrock" 

FLG= -$(OPT) -std=$(STD) -DLONG=$(LONG) $(PATH_INCLUDE) $(ROS_FLG) $(RKF_FLG) $(SPLINE_FLG)  

DataFiles=$(wildcard src/data/*.dat)
Ros_Headers= $(wildcard src/Rosenbrock/*.hpp) $(wildcard src/Rosenbrock/LU/*.hpp)   
SPLINE_Headers=$(wildcard src/Interpolation/*.hpp)

Axion_Headers= $(wildcard src/Axion/*.hpp)   

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

Cosmo_Headers=$(wildcard src/Cosmo/Cosmo.cpp) $(wildcard src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard src/AxionMass/AxionMass.cpp) $(wildcard src/AxionMass/AxionMass.hpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard src/AnharmonicFactor/AnharmonicFactor.hpp) 

all: mkmisc lib exec

lib: mklib lib/libCosmo.so lib/libma.so lib/libanfac.so  lib/Axion_py.so
	
exec: mkexec exec/Axion.run

mklib:
	mkdir "lib" || true
mkexec:
	mkdir "exec" || true
mkmisc:
	mkdir "src/misc_dir" || true

#shared libraries that can be used from python
lib/libCosmo.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(Cosmo_Headers) makefile
	$(CC)  -fPIC "src/Cosmo/Cosmo.cpp" -shared -o "lib/libCosmo.so"  $(FLG) -Wall

lib/libma.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers)  makefile
	$(CC)  -fPIC "src/AxionMass/AxionMass.cpp" -shared -o "lib/libma.so"  $(FLG) -Wall

lib/libanfac.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) makefile
	$(CC)  -fPIC "src/AnharmonicFactor/AnharmonicFactor.cpp" -shared -o "lib/libanfac.so" $(FLG) -Wall
#######################################################################################################


# evolution of the axion 
Axion_cpp=$(wildcard src/Axion/Axion.cpp)
exec/Axion.run: $(PathHead) $(Axion_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(Axion_Headers) makefile $(AxionMisc_Headers) $(NSC_Headers)
	$(CC) -o "exec/Axion.run" "src/Axion/Axion.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"src/Axion"

Axion_py=$(wildcard src/Axion/Axion-py.cpp)
#shared library for the evolution of the axion that can be used from python
lib/Axion_py.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(Axion_py) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(Axion_Headers) makefile $(AxionMisc_Headers) $(NSC_Headers) lib/libCosmo.so lib/libma.so lib/libanfac.so 
	 $(CC)  -fPIC "src/Axion/Axion-py.cpp" -shared -o "lib/Axion_py.so"  $(FLG) -DMETHOD=RODASPR2  -I"Axion"



##---copy paths where useful data cabe found in header files. These pathe can be used from c++ files to load the data  
$(PathHead):  $(DataFiles) makefile
	echo "#define h_PATH \"src/data/eos2020.dat\" "> "$(PathHead)"
	echo "#define chi_PATH \"src/data/chi.dat\" ">> "$(PathHead)"
	echo "#define anharmonic_PATH \"src/data/anharmonic_factor.dat\" ">> "$(PathHead)"

#python files that contain the currest path so that python files can find it easily 
$(PathHeadPy):  $(DataFiles) makefile
	echo "_PATH_=\"$(Path)\" "> "$(PathHeadPy)"

#python files that define either double or long double ctypes. 
#This is extremely useful, since it allows both c++ and shared libraries used by python to have the same type definitions. 
$(PathTypePy):  $(DataFiles) makefile
	echo "from ctypes import c_longdouble, c_double"> "$(PathTypePy)"
	echo "cdouble= c_$(LONG)double ">> "$(PathTypePy)" 


#cleans whatever make all created
clean: 

	rm -r $(wildcard lib/*) ||true
	rm -r $(wildcard exec/*) ||true
	rm -r $(wildcard src/misc_dir/*) ||true



##--------------------------------make checks----------------------------------------##
check: exec/AxionEOM_check.run exec/AxionSolve_check.run

AxionEOM_cpp=$(wildcard src/Axion/checks/AxionEOM_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionEOM_check.run: $(Axion_Headers) $(PathHead) $(AxionEOM_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers)
	$(CC) -o "exec/AxionEOM_check.run" "src/Axion/checks/AxionEOM_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"src/Axion"

AxionSolve_cpp=$(wildcard src/Axion/checks/AxionSolve_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionSolve_check.run: $(Axion_Headers) $(PathHead) $(AxionSolve_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers)
	$(CC) -o "exec/AxionSolve_check.run" "src/Axion/checks/AxionSolve_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"src/Axion"

##--------------------------------clone code from github repos----------------------------------------##
clone: clone_Ros clone_Spline

clone_Ros:
	bash -c "cd src;  ./clone_Ros.sh"

clone_Spline:
	bash -c "cd src;  ./clone_Spline.sh"
