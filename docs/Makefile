TEXFILES = $(wildcard *.tex)
REPORTS = $(TEXFILES:.tex=.pdf)

DIAPATH = generated_images
DIAFILES = $(wildcard *.dia)
DIAIMAGES = $(addprefix $(DIAPATH)/, $(DIAFILES:.dia=.png))

reports: $(REPORTS) $(DIAIMAGES)

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

$(DIAPATH)/%.png: %.dia
	dia $< -e $@

clean:
	rm -f *.pdf
	rm -f *.log
	rm -f *.aux
	rm -f *.out
	rm -f generated_images/*.png

.PHONY: reports clean
