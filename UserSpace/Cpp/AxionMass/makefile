-include Definitions.mk

PATH_INCLUDE= -I$(rootDir)

FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG)


SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

AxionMass_Headers=$(wildcard $(rootDir)src/AxionMass/AxionMass.hpp)

all: AxionMass.run

AxionMass.run: AxionMass.cpp $(AxionMass_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG)


clean:
	rm -f AxionMass.run