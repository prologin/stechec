# -*- Makefile -*-

bb5datadir = $(pkgdatadir)/bb5

DIRS	 	= formation font i18n image sound video
EXTRA_DIST	= $(DIRS)

dist_bb5data_DATA =				\
	bb5.yml

dist-hook:
	rm -rf `find $(distdir) -name .svn`

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	-for i in $(DIRS); do \
	  cp -r $(srcdir)/$$i $(DESTDIR)$(pkgdatadir); \
	done

uninstall-hook:
	for i in $(DIRS); do \
	  chmod -R u+w $(DESTDIR)$(pkgdatadir)/$$i; \
	  rm -rf $(DESTDIR)$(pkgdatadir)/$$i; \
	done
