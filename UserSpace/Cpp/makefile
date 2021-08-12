#make the examples
all: anF axion axM cosmo 

anF:
	cd AnharmonicFactor && $(MAKE)

axion:
	cd Axion && $(MAKE)

axM:
	cd AxionMass && $(MAKE)

cosmo:
	cd Cosmo && $(MAKE)

clean:
	rm -f AnharmonicFactor/*.run
	rm -f Axion/*.run
	rm -f AxionMass/*.run
	rm -f Cosmo/*.run
