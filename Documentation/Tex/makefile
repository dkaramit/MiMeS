TEX=pdflatex
BIB=bibtex

figs = $(wildcard figs/*.pdf)

#make the pdf
all:  ../MiMeS.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
../MiMeS.pdf:  $(figs) MiMeS.tex refs.bib makefile
	$(TEX) MiMeS.tex 
	$(BIB) MiMeS || true
	$(TEX) MiMeS.tex
	mv MiMeS.pdf ../

	#remove all output except the pdf file
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
clean:
	rm -f ../MiMeS.pdf 
	
