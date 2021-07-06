# ---example of other path--- #
#Path = "/home/USR/MiMeS"

#set Path variable to working directory
Path=$(PWD)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

LONG=long#use long doubles. Slower (marginally), but more accurate. 
#Keep in mind that Hubble.hpp uses doubles, which can cause problems (n_bose for x>750 returns 0).

#---optimization options---#
OPT=O3 

#c++ std lib
STD=c++17


SPLINE_FLG= -I"$(Path)/src/Interpolation"
PATH_INCLUDE= -I"$(Path)"  -I"$(Path)/src" -I"$(Path)/src/misc_dir" -I"$(Path)/src/Cosmo" -I"$(Path)/src/AnharmonicFactor" -I"$(Path)/src/AxionMass"

ROS_FLG=-I"$(Path)/src/Rosenbrock" 


FLG= -$(OPT) -std=$(STD) -DLONG=$(LONG) $(PATH_INCLUDE) $(ROS_FLG) $(RKF_FLG) $(SPLINE_FLG)  

DataFiles=$(wildcard $(Path)/src/data/*.dat)
Ros_Headers= $(wildcard $(Path)/src/Rosenbrock/*.hpp) $(wildcard $(Path)/src/Rosenbrock/LU/*.hpp)   
SPLINE_Headers=$(wildcard $(Path)/src/Interpolation/*.hpp)

Axion_Headers= $(wildcard $(Path)/src/Axion/*.hpp)   

PathHead=src/misc_dir/path.hpp
PathHeadPy=src/misc_dir/path.py
PathTypePy=src/misc_dir/type.py

Cosmo_Headers=$(wildcard $(Path)/src/Cosmo/Cosmo.cpp) $(wildcard $(Path)/src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard $(Path)/src/AxionMass/AxionMass.cpp) $(wildcard $(Path)/src/AxionMass/AxionMass.hpp) $(wildcard $(Path)/src/AnharmonicFactor/AnharmonicFactor.cpp) $(wildcard $(Path)/src/AnharmonicFactor/AnharmonicFactor.hpp) 

all: mkmisc lib exec

lib: mklib lib/libCosmo.so lib/libma.so lib/libanfac.so  lib/Axion_py.so
	
exec: mkexec #exec/Axion.run

mklib:
	mkdir "$(Path)/lib" || true
mkexec:
	mkdir "$(Path)/exec" || true
mkmisc:
	mkdir "$(Path)/src/misc_dir" || true

#shared libraries that can be used from python
lib/libCosmo.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(Cosmo_Headers) makefile
	$(CC)  -fPIC "$(Path)/src/Cosmo/Cosmo.cpp" -shared -o "$(Path)/lib/libCosmo.so"  $(FLG) -Wall

lib/libma.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers)  makefile
	$(CC)  -fPIC "$(Path)/src/AxionMass/AxionMass.cpp" -shared -o "$(Path)/lib/libma.so"  $(FLG) -Wall

lib/libanfac.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(DataFiles) $(SPLINE_Headers) $(AxionMisc_Headers) makefile
	$(CC)  -fPIC "$(Path)/src/AnharmonicFactor/AnharmonicFactor.cpp" -shared -o "$(Path)/lib/libanfac.so" $(FLG) -Wall
#######################################################################################################


# evolution of the axion 
# exec/Axion.run: $(PathHead) Axion.cpp $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(Axion_Headers) makefile $(AxionMisc_Headers) $(NSC_Headers)
#	$(CC) -o "$(Path)/exec/Axion.run" "$(Path)/Axion.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"$(Path)/src/Axion"

Axion_py=$(wildcard $(Path)/src/Axion/Axion-py.cpp)
#shared library for the evolution of the axion that can be used from python
lib/Axion_py.so: $(PathHead)  $(PathHeadPy) $(PathTypePy) $(Axion_py) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) $(Axion_Headers) makefile $(AxionMisc_Headers) $(NSC_Headers) lib/libCosmo.so lib/libma.so lib/libanfac.so 
	 $(CC)  -fPIC "$(Path)/src/Axion/Axion-py.cpp" -shared -o "$(Path)/lib/Axion_py.so"  $(FLG) -DMETHOD=RODASPR2  -I"$(Path)/Axion"



##---copy paths where useful data cabe found in header files. These pathe can be used from c++ files to load the data  
$(PathHead):  $(DataFiles) makefile
	echo "#define h_PATH \"$(Path)/src/data/eos2020.dat\" "> "$(Path)/$(PathHead)"
	echo "#define chi_PATH \"$(Path)/src/data/chi.dat\" ">> "$(Path)/$(PathHead)"
	echo "#define anharmonic_PATH \"$(Path)/src/data/anharmonic_factor.dat\" ">> "$(Path)/$(PathHead)"

#python files that contain the currest path so that python files can find it easily 
$(PathHeadPy):  $(DataFiles) makefile
	echo "_PATH_=\"$(Path)\" "> "$(PathHeadPy)"

#python files that define either double or long double ctypes. 
#This is extremely useful, since it allows both c++ and shared libraries used by python to have the same type definitions. 
$(PathTypePy):  $(DataFiles) makefile
	echo "from ctypes import c_longdouble, c_double"> "$(Path)/$(PathTypePy)"
	echo "cdouble= c_$(LONG)double ">> "$(Path)/$(PathTypePy)" 


#cleans whatever make all created
clean: 

	rm $(wildcard lib/*) ||true
	rm $(wildcard exec/*) ||true
	rm $(wildcard src/misc_dir/*) ||true



##--------------------------------make checks----------------------------------------##
check: exec/AxionEOM_check.run exec/AxionSolve_check.run

AxionEOM_cpp=$(wildcard $(Path)/src/Axion/checks/AxionEOM_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionEOM_check.run: $(Axion_Headers) $(PathHead) $(AxionEOM_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers)
	$(CC) -o "$(Path)/exec/AxionEOM_check.run" "$(Path)/src/Axion/checks/AxionEOM_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"$(Path)/src/Axion"

AxionSolve_cpp=$(wildcard $(Path)/src/Axion/checks/AxionSolve_check.cpp)
# check interpolations of the Axion_eom class 
exec/AxionSolve_check.run: $(Axion_Headers) $(PathHead) $(AxionSolve_cpp) $(Ros_Headers) $(DataFiles) $(SPLINE_Headers) makefile $(AxionMisc_Headers)
	$(CC) -o "$(Path)/exec/AxionSolve_check.run" "$(Path)/src/Axion/checks/AxionSolve_check.cpp"   $(FLG)    -DMETHOD=RODASPR2  -I"$(Path)/src/Axion"

##--------------------------------clone code from github repos----------------------------------------##
clone: clone_Ros clone_Spline

clone_Ros:
	bash -c "cd src;  ./clone_Ros.sh"

clone_Spline:
	bash -c "cd src;  ./clone_Spline.sh"
