# -*- Makefile -*-

prolo2006datadir = $(pkgdatadir)/prolo2006
prolo2006fontdir = $(pkgdatadir)/prolo2006/font

dist_prolo2006data_DATA =		\
	prolo2006.yml			\
	simple.map			\
	test.map			\
	street.map

dist_prolo2006font_DATA =		\
	font/Vera.ttf

EXTRA_DIST = texture_2D

dist-hook:
	rm -rf `find $(distdir) -name .svn`

install-data-hook:
	-cp -r $(srcdir)/texture_2D $(DESTDIR)$(prolo2006datadir)

uninstall-hook:
	-chmod -R u+w $(DESTDIR)$(prolo2006datadir)/texture_2D
	rm -rf $(DESTDIR)$(prolo2006datadir)/texture_2D
