# -*-Makefile-*-

ACLOCAL_AMFLAGS	= -I build-aux
SUBDIRS		= 				\
	build-aux				\
	conf					\
	stechec

EXTRA_DIST	= README.stechec

if BUILD_bb5
SUBDIRS		+= bb5
endif

if BUILD_prologin
SUBDIRS		+= prologin
endif

# FIXME: move to stechec.
SUBDIRS += doc

CONFIGURE_DEPENDENCIES =			\
	$(srcdir)/build-aux/sdl.m4		\
	$(srcdir)/build-aux/xerces.m4		\
	$(srcdir)/build-aux/pthreads.m4		\
	$(srcdir)/build-aux/stechec-deps.m4
