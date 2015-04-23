TEX = pdflatex

DOC_NAME = firstfollow_deriviation
DOC_SOURCES = $(DOC_NAME).tex
DOCUMENTATION = $(DOC_NAME).pdf

.PHONY: clean

$(DOCUMENTATION): $(DOC_SOURCES)
	$(TEX) $<

clean:
	rm *.pdf *.aux *.log

