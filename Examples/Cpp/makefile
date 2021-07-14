#make the examples


all: anharmonicFactor  axion  axionMass  aosmo 

anharmonicFactor:
	cd AnharmonicFactor && $(MAKE)

axion:
	cd Axion && $(MAKE)

axionMass:
	cd AxionMass && $(MAKE)

aosmo:
	cd Cosmo && $(MAKE)


clean:
	rm -f AnharmonicFactor/*.run
	rm -f Axion/*.run
	rm -f AxionMass/*.run
	rm -f Cosmo/*.run
