# -*- Makefile -*-

prolo2008datadir = $(pkgdatadir)/prolo2008
prolo2008fontdir = $(pkgdatadir)/prolo2008/font
prolo2008graphicsdir = $(pkgdatadir)/prolo2008/graphics

dist_prolo2008data_DATA =			\
	prolo2008.yml

dist_prolo2008font_DATA =		\
	font/Vera.ttf

dist_prolo2008graphics_DATA =	\
	graphics/ground.png	\
	graphics/rock.png	\
	graphics/hamster_1.png	\
	graphics/hamster_2.png	\
	graphics/hamster_2_apple.png	\
	graphics/hamster_1_apple.png	\
	graphics/hole.png		\
	graphics/apple.png		\
	graphics/pin.png		\
	graphics/hole_horiz1.png	\
	graphics/hole_horiz2.png	\
	graphics/hole_vert1.png		\
	graphics/hole_vert2.png		\
	graphics/font.ttf

dist-hook:
	rm -rf `find $(distdir) -name .svn`
