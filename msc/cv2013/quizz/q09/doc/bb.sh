#!/bin/bash

rm biblio.bib
cat bibtex/*.bib* >> biblio.bib

latex $1.tex
bibtex $1.aux
latex $1.tex
latex $1.tex
pdflatex $1.tex
evince $1.pdf
