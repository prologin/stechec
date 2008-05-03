#
# Makefile for the Prologin 2008 Map Editor
#
# By Thomas Lecomte (th.lecomte@gmail.com)
#

TARGET  = editor

SRCS	= Gui.cpp	\
	  Editor.cpp	\
	  SpriteMgr.cpp

OBJS	= $(SRCS:%.cpp=%.o)

# Programs

CC	= g++

CFLAGS	= -W -Wall `sdl-config --cflags` -g

LDFLAGS	= `sdl-config --libs` -lSDL_image

# Rules

$(TARGET):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS)

all:		$(TARGET)

.cpp.o:
		$(CC) $(CFLAGS) -o $@ -c $<

clean:
		rm -f $(OBJS)

distclean:	clean
		rm -f $(TARGET)

re:		distclean all

.PHONY:		all clean distclean re

.SUFFIXES:	.cpp.o
