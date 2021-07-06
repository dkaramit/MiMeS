TEX=pdflatex
BIB=bibtex

figs = $(wildcard *.pdf)

#make the pdf
all:  axion_analytical.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
axion_analytical.pdf:  $(figs) axion_analytical.tex refs.bib
	$(TEX) "axion_analytical.tex" 
	$(BIB) "axion_analytical" || true
	$(TEX) "axion_analytical.tex" 
	$(TEX) -synctex=1  "axion_analytical.tex"
	rm *.aux || true
	rm *.log || true
	rm *.out || true
	rm *.blg || true


#remove all output except the pdf file
clean:
	rm "axion_analytical.pdf" || true
	rm *.synctex.gz || true
	rm *.aux || true
	rm *.log || true
	rm *.out || true
	rm *.bbl || true
	rm *.blg || true
	
