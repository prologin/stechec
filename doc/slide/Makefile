##
## Makefile
## Login   <cuissa_v@epita.fr>
##

## Environment Variable
CODE2BEAMER = ruby external/code2beamer.rb
RM   = $(shell which rm) -f
# put here basename of your final document
ALL  = presentation
# Source files
SRC = $(wildcard *.tex)
# put all you non .tex sources here (include bibliography, pictures, etc...)
SRC += 
# Syntax highlighting
CODE_FILE = $(wildcard code/*.code)
CODE_PDF = $(CODE_FILE:.code=.pdf)
CODE_NUMBER_LINE = "40"
IMG_DIR = "generated/"
## End Environment Variable


##
## Rules
##

all: pdf

.SUFFIXES: .code .pdf

bz2: distclean
	@echo -n Genrerate tarball...
	@rm -rf .presentation
	@mkdir .presentation
	@cp -rf * .presentation/
	@mv .presentation presentation
	@tar jcf presentation.tar.bz2 presentation
	@rm -rf presentation
	@echo OK

pdf: $(CODE_PDF)
	TEXINPUTS=.:styles//:external//:/usr/share/texmf//:/usr/share/texmf-tetex//:/sw/etc/texmf.local//:/sw/share/texmf-dist// ./latexmk.pl $(QUIETMK) -i -I -pdf $(ALL)

clean:
	$(RM) *.aux *.log *.out *.toc *~ *.bmt *.mtc*                   \
	*.bbl *.bak *.dep *.tmp *.blg *.haux *.htoc *.image.* *.[0-9] *.nav *.snm *.vrb
	$(RM) -rf generated/

distclean: clean
	$(RM) *.dvi *.ps *.pdf *.html *.info *.txt presentation.tar.bz2

.tex.pdf:
	./latexmk.pl $(QUIETMK) -i -I -pdf $<

# Syntax highligthing
.code.pdf: $<
	@$(CODE2BEAMER) -d $(IMG_DIR) -l $(CODE_NUMBER_LINE) $<;
