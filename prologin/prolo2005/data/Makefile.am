# -*- Makefile -*-

prolo2005datadir = $(pkgdatadir)/prolo2005

dist_prolo2005data_DATA =		\
	$(wildcard *.map)	\
	prolo2005.yml

EXTRA_DIST = texture_2D

install-data-hook:
	-cp -r $(srcdir)/texture_2D $(DESTDIR)$(prolo2005datadir)

uninstall-hook:
	-chmod -R u+w $(DESTDIR)$(prolo2005datadir)/texture_2D
	rm -rf $(DESTDIR)$(prolo2005datadir)/texture_2D
