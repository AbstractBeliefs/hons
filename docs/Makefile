TEXFILES := $(wildcard *.tex)
REPORTS = $(TEXFILES:.tex=.pdf)

DIAFILES := $(wildcard *.dia)
DIAIMAGES = $(DIAFILES:.dia=.png)

reports: $(REPORTS)

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

%.png: %.dia
	dia $< -e $@

clean:
	rm -f *.pdf
	rm -f *.log
	rm -f *.aux
	rm -f *.out

.PHONY: reports clean
