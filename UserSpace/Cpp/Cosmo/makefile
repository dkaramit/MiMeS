-include Definitions.mk

PATH_INCLUDE=-I$(rootDir)

FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

all: Cosmo.run

Cosmo.run: Cosmo.cpp $(Cosmo_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG)


clean:
	rm -f Cosmo.run