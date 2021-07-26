#make the examples
all: anharmonicFactor  axion  axionMass  cosmo 

anharmonicFactor:
	cd AnharmonicFactor && $(MAKE)

axion:
	cd Axion && $(MAKE)

axionMass:
	cd AxionMass && $(MAKE)

cosmo:
	cd Cosmo && $(MAKE)

clean:
	rm -f AnharmonicFactor/*.run
	rm -f Axion/*.run
	rm -f AxionMass/*.run
	rm -f Cosmo/*.run
