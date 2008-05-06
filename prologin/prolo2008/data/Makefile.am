# -*- Makefile -*-

prolo2008datadir = $(pkgdatadir)/prolo2008
prolo2008fontdir = $(pkgdatadir)/prolo2008/font
prolo2008graphicsdir = $(pkgdatadir)/prolo2008/graphics

dist_prolo2008data_DATA =			\
	prolo2008.yml

dist_prolo2008font_DATA =		\
	font/Vera.ttf

dist_prolo2008graphics_DATA =	\
	graphics/ground.png

dist-hook:
	rm -rf `find $(distdir) -name .svn`
