#!/bin/bash

rm biblio.bib
cat bibtex/*.bib* >> biblio.bib

latex report.tex
bibtex report.aux
latex report.tex
latex report.tex
pdflatex report.tex
evince report.pdf
