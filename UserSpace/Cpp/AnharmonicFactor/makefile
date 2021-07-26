-include Definitions.mk

PATH_INCLUDE= -I$(rootDir)

FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

AnFac_Headers=$(wildcard $(rootDir)src/AnharmonicFactor/AnharmonicFactor.hpp)

all: AnharmonicFactor.run

AnharmonicFactor.run: AnharmonicFactor.cpp $(AnFac_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG) 


clean:
	rm -f AnharmonicFactor.run