# -*- Makefile -*-

summer2010datadir = $(pkgdatadir)/summer2010
summer2010graphicsdir = $(pkgdatadir)/summer2010/graphics

dist_summer2010data_DATA =			\
	summer2010.yml

dist_summer2010graphics_DATA = graphics/*

dist-hook:
	rm -rf `find $(distdir) -name .svn`
