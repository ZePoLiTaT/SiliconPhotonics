#!/bin/bash

rm biblio.bib
cat bibtex/*.bib* >> biblio.bib

pdflatex report.tex
bibtex report.aux
pdflatex report.tex
pdflatex report.tex
pdflatex report.tex
evince report.pdf
