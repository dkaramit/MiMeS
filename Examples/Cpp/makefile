#make the examples


all: AnharmonicFactor  Axion  AxionMass  Cosmo 

AnharmonicFactor:
	cd AnharmonicFactorExample && $(MAKE)

Axion:
	cd AxionExample && $(MAKE)

AxionMass:
	cd AxionMassExample && $(MAKE)

Cosmo:
	cd CosmoExample && $(MAKE)


clean:
	rm -f AnharmonicFactorExample/*.run
	rm -f AxionExample/*.run
	rm -f AxionMassExample/*.run
	rm -f CosmoExample/*.run
