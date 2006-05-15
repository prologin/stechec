# -*- Makefile -*-

DIRS	 	= formation font i18n image sound team video
EXTRA_DIST	= $(DIRS)

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
