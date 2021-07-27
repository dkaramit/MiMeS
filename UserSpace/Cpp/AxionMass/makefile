-include Definitions.mk

PATH_INCLUDE= -I$(rootDir)

#c++ std
STD=c++17

FLG= -$(OPT) -std=$(STD) -lm $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

AxionMass_Headers=$(wildcard $(rootDir)src/AxionMass/AxionMass.hpp)
PathHead=$(rootDir)src/misc_dir/path.hpp

all: AxionMass.run

AxionMass.run: AxionMass.cpp $(AxionMass_Headers) $(SPLINE_Headers) $(PathHead)

	$(CC) -o $@ $< $(FLG)


clean:
	rm -f AxionMass.run