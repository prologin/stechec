# -*-Makefile-*-

#
# FIXME: remove this directory, move into bb5/ and prologin/
#
dist_pkgdata_DATA =				\
	config_prolo.xml			\
	config.dtd

dist_sysconf_DATA = stechec.yml

install-data-local:
	chmod 600 $(DESTDIR)$(sysconfdir)/stechec.yml
