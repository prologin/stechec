# -*- Makefile -*-

bb5datadir = $(pkgdatadir)/bb5

DIRS	 	= font i18n image sound video
EXTRA_DIST	= $(DIRS)

nobase_dist_bb5data_DATA =			\
	bb5.yml					\
	default_config.xml			\
	team/amazon.xml				\
	formation/default.xml			\
	schema/team.xsd				\
	schema/formation.dtd

dist-hook:
	rm -rf `find $(distdir) -name .svn`

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	-for i in $(DIRS); do \
	  cp -r $(srcdir)/$$i $(DESTDIR)$(pkgdatadir); \
          rm -rf `find $(DESTDIR)$(pkgdatadir)/$$i -name .svn`; \
	done

uninstall-hook:
	for i in $(DIRS); do \
	  chmod -R u+w $(DESTDIR)$(pkgdatadir)/$$i; \
	  rm -rf $(DESTDIR)$(pkgdatadir)/$$i; \
	done
