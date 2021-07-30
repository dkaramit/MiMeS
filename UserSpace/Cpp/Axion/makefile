-include Definitions.mk

PATH_INCLUDE= -I$(rootDir)

#c++ std
STD=c++17

FLG= -$(OPT) -std=$(STD) -lm $(PATH_INCLUDE) -Wall -DLONG=$(LONG) -DMETHOD=$(METHOD) -DSolver=$(Solver)

Ros_Headers= $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard $(rootDir)src/NaBBODES/RKF/*.hpp) 
SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

Axion_Headers= $(wildcard $(rootDir)src/Axion/*.hpp)  

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)
AxionMisc_Headers= $(wildcard $(rootDir)src/AxionMass/AxionMass.hpp) $(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.hpp) 

Static_Headers= $(wildcard $(rootDir)src/static.hpp) 
PathHead=$(rootDir)src/misc_dir/path.hpp

all: Axion.run

Axion.run: Axion.cpp\
					$(Axion_Headers) $(AxionMisc_Headers) $(Cosmo_Headers) $(PathHead) $(Static_Headers)\
					$(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG) 


clean:
	rm -f Axion.run