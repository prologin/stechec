PDFLATEX	=	pdflatex

TMP		=	sujet.aux	\
			sujet.idx	\
			sujet.log	\
			sujet.toc

all: sujet.pdf

%.toc:	%.tex
	$(PDFLATEX) sujet.tex

%.pdf:	%.toc
	$(PDFLATEX) sujet.tex

clean:
	$(RM) $(TMP)

distclean: clean
	$(RM) sujet.pdf
