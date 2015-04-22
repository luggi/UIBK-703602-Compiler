TEX = pdflatex

DOC_NAME = firstfollow_deriviation
DOC_SOURCES = $(DOC_NAME).tex
DOCUMENTATION = $(DOC_NAME).pdf

$(DOCUMENTATION): $(DOC_SOURCES)
	$(TEX) $<

clean:
	*.pdf *.aux *.log

