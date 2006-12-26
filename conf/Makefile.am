# -*-Makefile-*-

dist_pkgdata_DATA =				\
	config_prolo.xml			\
	config_tbt.xml 				\
	formation.xml				\
	team.xml				\
	config.dtd				\
	team.xsd				\
	formation.dtd

dist_sysconf_DATA = stechec.yml

install-data-local:
	chmod 600 $(DESTDIR)$(sysconfdir)/stechec.yml
