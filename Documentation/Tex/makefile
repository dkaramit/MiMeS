TEX=pdflatex
BIB=bibtex

FLG=-shell-escape

figs = $(wildcard figs/*.pdf)

#make the pdf
all:  ../MiMeS.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
../MiMeS.pdf:  $(figs) MiMeS.tex refs.bib
	$(TEX) $(FLG) MiMeS.tex 
	$(BIB) MiMeS || true
	$(TEX) $(FLG) MiMeS.tex
	mv MiMeS.pdf ../

	#remove all output except the pdf file
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
	rm -rf _minted-MiMeS
clean:
	rm -f ../MiMeS.pdf 
	
