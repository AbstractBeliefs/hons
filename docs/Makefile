TEXFILES := $(wildcard *.tex)
REPORTS = $(TEXFILES:.tex=.pdf)

reports: $(REPORTS)

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.pdf
	rm -f *.log
	rm -f *.aux
	rm -f *.out

.PHONY: reports clean
