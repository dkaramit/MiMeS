
-include Definitions.mk

#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE= -I$(rootDir)
FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -std=$(STD) -lstdc++ -Wall -DLONG=$(LONG)


SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

AxionMass_Headers=$(wildcard $(rootDir)src/AxionMass/AxionMass.hpp)

all: AxionMass.run

AxionMass.run: AxionMass.cpp $(AxionMass_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG)


clean:
	rm -f AxionMass.run