# -*- Makefile -*-

prolo2009datadir = $(pkgdatadir)/prolo2009
prolo2009fontdir = $(pkgdatadir)/prolo2009/font
prolo2009graphicsdir = $(pkgdatadir)/prolo2009/graphics

dist_prolo2009data_DATA =			\
	prolo2009.yml

dist_prolo2009font_DATA =		\
	font/Vera.ttf

dist_prolo2009graphics_DATA =	\
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
