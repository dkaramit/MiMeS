-include Definitions.mk

PATH_INCLUDE= -I$(rootDir)

#c++ std
STD=c++17

FLG= -$(OPT) -std=$(STD) -lm $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

AnFac_Headers=$(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.hpp)
PathHead=$(rootDir)src/misc_dir/path.hpp

all: AnharmonicFactor.run

AnharmonicFactor.run: AnharmonicFactor.cpp $(AnFac_Headers) $(SPLINE_Headers) $(PathHead)

	$(CC) -o $@ $< $(FLG) 


clean:
	rm -f AnharmonicFactor.run