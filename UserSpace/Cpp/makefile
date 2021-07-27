#make the examples
all: AnharmonicFactor/AnharmonicFactor.run Axion/Axion.run AxionMass/AxionMass.run Cosmo/Cosmo.run 

AnharmonicFactor/AnharmonicFactor.run:
	cd AnharmonicFactor && $(MAKE)

Axion/Axion.run:
	cd Axion && $(MAKE)

AxionMass/AxionMass.run:
	cd AxionMass && $(MAKE)

Cosmo/Cosmo.run:
	cd Cosmo && $(MAKE)

clean:
	rm -f AnharmonicFactor/*.run
	rm -f Axion/*.run
	rm -f AxionMass/*.run
	rm -f Cosmo/*.run
