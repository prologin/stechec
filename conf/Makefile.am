# -*-Makefile-*-

dist_pkgdata_DATA =				\
	config_prolo.xml			\
	formation.xml				\
	config.dtd				\
	team.xsd				\
	formation.dtd

dist_sysconf_DATA = stechec.yml

install-data-local:
	chmod 600 $(DESTDIR)$(sysconfdir)/stechec.yml
