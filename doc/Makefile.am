# -*- Makefile -*-

SRC			= propal.tex stechec.tex
DOCREF			= doxygen_ref.tar.gz
SRC_EXTRA		= 			\
	$(srcdir)/styles/tbt.sty		\
	$(srcdir)/styles/acro.sty		\
	$(srcdir)/bib/tbt.bib

PDF			= $(SRC:.tex=.pdf)
MAINTAINERCLEANFILES	= $(PDF) $(DOCREF)
EXTRA_DIST		= $(SRC) $(PDF) $(DOCREF) $(SRC_EXTRA) doxygen_main.hh
docdir			= $(datadir)/doc/${PACKAGE}
doc_DATA		= $(PDF) $(DOCREF)

TEXI2DVI_FLAGS		= --tidy --build-dir=pdf --batch -I $(srcdir)/figs -I $(srcdir)/styles -I $(srcdir)/bib

%.pdf : %.tex $(SRC_EXTRA)
	$(top_srcdir)/build-aux/texi2dvi --pdf $(TEXI2DVI_FLAGS) -o $@ $<


#
# Rules to run doxygen on our code.
#
$(DOCREF): $(srcdir)/doc.doxy.in $(srcdir)/doxygen_main.hh
	if test -d $(srcdir)/.svn -a -x `which svn`; then \
	  REV=`LANG=C svn info $(srcdir) | sed -ne 's/^Revision: *\([0-9]\+\)/r\1/p'`; \
	  sed -e "s/%%Revision%%/$$REV/" < doc.doxy > doc.doxy.rev; \
	  $(DOXYGEN) doc.doxy.rev; \
	elif test "`svk info $(top_srcdir) 2> /dev/null`" != ""; then \
	  REV=`svk info $(top_srcdir) 2> /dev/null | sed -ne 's/^Mirrored From: .*, Rev. *\([0-9]\+\)/r\1/p'`; \
	  sed -e "s/%%Revision%%/$$REV/" < doc.doxy > doc.doxy.rev; \
	  $(DOXYGEN) doc.doxy.rev; \
	fi
	if ! test -d doxygen_html; then \
	  sed -e "s/%%Revision%%//" < doc.doxy > doc.doxy.rev; \
	  $(DOXYGEN) doc.doxy.rev; \
	fi
	@rm -f doc.doxy.rev
	tar czf $@ doxygen_html

clean-local::
	rm -rf pdf doxygen_html
