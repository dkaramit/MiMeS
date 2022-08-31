TEX=pdflatex
BIB=bibtex

FLG=-synctex=1 

figs = $(wildcard figs/*.pdf)

#make the pdf
all:  pres_2022.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
pres_2022.pdf:  pres_2022.tex $(figs) 
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 

	#remove all output except the pdf file
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.toc 
	rm -f *.blg 
	rm -f *.snm
	rm -f *.nav

clean:
	rm -f pres_2022.pdf 
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
	rm -f *.toc 
	rm -f *.snm
	rm -f *.nav
	rm -f *.synctex.gz
