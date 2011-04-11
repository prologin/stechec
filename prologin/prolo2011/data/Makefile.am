# -*- Makefile -*-

prolo2011datadir = $(pkgdatadir)/prolo2011
prolo2011graphicsdir = $(pkgdatadir)/prolo2011/graphics
prolo2011mapsdir = $(pkgdatadir)/prolo2011/maps

dist_prolo2011data_DATA =			\
	prolo2011.yml

dist_prolo2011graphics_DATA = graphics/*
dist_prolo2011maps_DATA = maps/*

dist-hook:
	rm -rf `find $(distdir) -name .svn`
