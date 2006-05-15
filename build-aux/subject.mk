# -*- Makefile -*-
#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Copyright (C) 2005, 2006 Prologin
#

PDF		= $(SRC:.tex=.pdf)
EXTRA_DIST 	= $(SRC) $(PDF)
CLEANFILES	= api.tex
DISTCLEANFILES  = $(PDF)
docdir		= $(datadir)/doc/$(PACKAGE)/subjects/
doc_DATA	= $(PDF)

TEXI2DVI_FLAGS		= 			\
	--tidy --build-dir=pdf --batch 		\
	-I $(srcdir) -I $(top_srcdir)/build-aux

%.pdf : %.tex $(SRC_EXTRA)
	$(top_srcdir)/build-aux/texi2dvi --pdf $(TEXI2DVI_FLAGS) -o $@ $<

clean-local::
	rm -rf pdf

api.tex :
	$(RUBY) 					\
	  -I$(top_srcdir)/stechec/scripts 		\
	  -I$(top_builddir)/stechec/scripts		\
	  $(top_srcdir)/stechec/scripts/generator.rb	\
	  apidoc $(YAML) tmp
	mv tmp/file.tex api.tex
	rm -rf tmp

$(PDF) : api.tex

SUFFIXES = .tex .pdf
