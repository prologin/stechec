# -*-Makefile-*-

ACLOCAL_AMFLAGS	= -I build-aux

SUBDIRS		= 				\
	build-aux				\
	conf					\
	stechec					\
	$(BUILD_bb5)				\
	$(BUILD_prologin)

EXTRA_DIST	= README.stechec debian stechec.spec

dist-hook:
	rm -rf `find $(distdir)/debian -name .svn`

# FIXME: move to stechec.
if GENERATE_DOC
SUBDIRS		+= doc
endif

CONFIGURE_DEPENDENCIES =			\
	$(srcdir)/build-aux/sdl.m4		\
	$(srcdir)/build-aux/pthreads.m4		\
	$(srcdir)/build-aux/stechec-deps.m4
