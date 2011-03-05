##=============================================================================
##
## Makefile
## Copyright (C) 2006  Julien Guertault
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
##
##=============================================================================
##
## Tools
##
## Makefile : made by Zavie (Julien Guertault)
##
##=============================================================================

NAME		=	outatime_bouze.so

AUTHOR		=	Zavie

VERSION		=	0.0.1

SYSTEM		=	`uname`

DIRECTORY	=	Outatime

BIN_DIRECTORY	=	bin

DOC_DIRECTORY	=	doc

SRC_DIRECTORY	=	src

ARCHIVE		=	tar -cvzf

CC		=	g++

ECHO		=	@echo

RM		=	rm -f

SRC		=	$(SRC_DIRECTORY)/3ds.cc			\
			$(SRC_DIRECTORY)/almanach.cc		\
			$(SRC_DIRECTORY)/building.cc		\
			$(SRC_DIRECTORY)/casino.cc		\
			$(SRC_DIRECTORY)/composite.cc		\
			$(SRC_DIRECTORY)/datacenter.cc		\
			$(SRC_DIRECTORY)/delorean.cc		\
			$(SRC_DIRECTORY)/element.cc		\
			$(SRC_DIRECTORY)/gem.cc			\
			$(SRC_DIRECTORY)/glutgui.cc		\
			$(SRC_DIRECTORY)/groundpart.cc		\
			$(SRC_DIRECTORY)/gui.cc			\
			$(SRC_DIRECTORY)/main.cc		\
			$(SRC_DIRECTORY)/matrix.cc		\
			$(SRC_DIRECTORY)/mesh.cc		\
			$(SRC_DIRECTORY)/moving.cc		\
			$(SRC_DIRECTORY)/ownable.cc		\
			$(SRC_DIRECTORY)/player.cc		\
			$(SRC_DIRECTORY)/rendering.cc		\
			$(SRC_DIRECTORY)/solid.cc		\
			$(SRC_DIRECTORY)/texturemanager.cc	\
			$(SRC_DIRECTORY)/vector3d.cc

DEBUG_FLAGS	=	-ggdb3

OPTIM_FLAGS	=	-Os			\
			-ffast-math		\
			-felide-constructors	\
			-fexpensive-optimizations\
			-funroll-loops		\
			-finline-functions

WARNING_FLAGS	=	-Wall			\
			-W

# CFLAGS		=	$(WARNING_FLAGS)	\
# 			$(OPTIM_FLAGS)

CFLAGS		=	$(DEBUG_FLAGS)		\
			$(WARNING_FLAGS)

#CFLAGS		=	$(DEBUG_FLAGS)		\
#			$(OPTIM_FLAGS)		\
#			$(WARNING_FLAGS)

OPENGL_IFLAGS	=	-I/usr/include		\
			-I/usr/include/GL	\
			-I/usr/X11R6/include	\
			-I/usr/X11R6/include/GL	\
			-I/dp/usr/include	\
			-I/dp/usr/include/GL	\
			-I/dp/usr/X11R6/include	\
			-I/dp/usr/X11R6/include/GL \
			-I/sw/include

top_srcdir	=	/home/zavie/dev/stechec

STECHEC_IFLAGS	=	-I$(top_srcdir)				\
			-I$(top_srcdir)/stechec/tools		\
		        -I$(top_srcdir)/stechec/base_rules	\
			-I$(top_srcdir)/stechec/client		\
			-I$(top_srcdir)/prologin/common		\
			-I$(top_srcdir)/prologin/gui_qt		\
			-I$(top_srcdir)/prologin/prolo2006/rules

IFLAGS		=	$(OPENGL_IFLAGS)	\
			$(STECHEC_IFLAGS)

OPENGL_LFLAGS	=	-lGL			\
			-lGLU			\
			-lglut

LFLAGS		=	-L/usr/lib		\
			-L/usr/X11R6/lib	\
			-L/sw/lib		\
			-lc			\
			-lm			\
			-lX11			\
			-ljpeg			\
			$(OPENGL_LFLAGS) -shared

OBJ		=	$(SRC:.cc=.o)

#HH		=	$(SRC_DIRECTORY)/

#------------------------------------------------------------------------------

all :		$(BIN_DIRECTORY)/$(NAME)

$(BIN_DIRECTORY)/$(NAME) :	$(OBJ)
				$(CC) $(LFLAGS) $(OBJ) -o $(BIN_DIRECTORY)/$(NAME)

.cc.o :
		$(CC) $(CFLAGS) $(IFLAGS) $< -c -o $@

clean :
		$(RM) $(OBJ)
		$(RM) *~ \#*\#
		$(RM) $(BIN_DIRECTORY)/*~ $(BIN_DIRECTORY)/\#*\#
		$(RM) $(DOC_DIRECTORY)/*~ $(DOC_DIRECTORY)/\#*\#
		$(RM) $(SRC_DIRECTORY)/*~ $(SRC_DIRECTORY)/\#*\#

distclean :	clean
		$(RM) $(BIN_DIRECTORY)/$(NAME)

separator :
		$(ECHO) "------------------------------------------"

re :		clean separator all

run :		all
		cd $(BIN_DIRECTORY) && ./$(NAME) && cd ..

tarball :	clean separator
		$(ECHO) "Archiving..."
		cd .. && $(ARCHIVE) $(AUTHOR)-$(NAME)-$(VERSION).tar.gz \
		$(DIRECTORY)
		$(ECHO) "Done !"

install :
		$(ECHO) "Not implemented yet."

love :
		$(ECHO) "Sorry, I have a headache..."

.PHONY :	all		\
		clean		\
		distclean	\
		separator	\
		re		\
		run		\
		tarball		\
		install		\
		love

##=============================================================================
