# -*-Autconf-*

# Autconf init
AC_PREREQ(2.59)
AC_INIT([stechec], [3.2.3], [http://github.com/prologin/stechec-2012])

# Auxiliary files
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([
  Makefile
  build-aux/Makefile
])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.9.2 foreign dist-bzip2])
AM_CONFIG_HEADER([config.h])


# Modules to build, by default
modules="prolo2012"

# Existing modules/meta-modules
STECHEC_ADDMODULE([prologin])
STECHEC_ADDMODULE([ant])
STECHEC_ADDMODULE([prolo2005])
STECHEC_ADDMODULE([prolo2006])
STECHEC_ADDMODULE([prolo2007])
STECHEC_ADDMODULE([prolo2008])
STECHEC_ADDMODULE([prolo2009])
STECHEC_ADDMODULE([prolo2010])
STECHEC_ADDMODULE([summer2010])
STECHEC_ADDMODULE([prolo2011])
STECHEC_ADDMODULE([prolo2012])

# Modules dependencies
DEPENDS=""
DEPENDS_prologin="sdl,mysql"
DEPENDS_ant="prologin"
DEPENDS_prolo2005="prologin"
DEPENDS_prolo2006="prologin"
DEPENDS_prolo2007="prologin"
DEPENDS_prolo2008="prologin"
DEPENDS_prolo2009="prologin"
DEPENDS_prolo2010="prologin"
DEPENDS_summer2010="prologin"
DEPENDS_prolo2011="prologin,python"

# Can select which module(s) to build
AC_MSG_CHECKING([for modules to build])
AC_ARG_WITH([modules],
        [AS_HELP_STRING([--with-modules=mods],
                [build selected modules [$modules]])],
        [tbt_cv_with_modules="$withval"],
        [AC_CACHE_VAL([tbt_cv_with_modules],
                      [tbt_cv_with_modules="$modules"])])
AC_MSG_RESULT([$tbt_cv_with_modules])

# Configure dependency
STECHEC_CONFIG_DEPENDENCY


AC_ARG_ENABLE([debug],
        [AS_HELP_STRING([--enable-debug], [for debugging with gdb])],
        [echo "compiling in debug mode ... -g";
         CXXFLAGS="-g";
         CFLAGS="-g"])

# to follow debian policy, don't set rpath when prefix is /usr
AM_CONDITIONAL([USE_RPATH], [test "$prefix" != "/usr"])

#
# Look for common programs
#
AC_PROG_CC
AC_PROG_CXX

# Bash
AC_PATH_PROGS(BASH, bash)
BASH_SCRIPTS="${prefix}/lib/${PACKAGE}/bash"
AC_SUBST(BASH_SCRIPTS)

# Ruby. We need it for the generator.
AC_PATH_PROGS(RUBY, ruby)
AC_ARG_VAR([RUBY], [the ruby interpreter])
AC_SUBST([RUBY_INC], ["${libdir}/${PACKAGE}/ruby"])
if test "x$RUBY" = "x" ; then
   echo
   AC_MSG_ERROR([ruby not found in $PATH])
fi

# Help2man, generate man pages.
AC_CHECK_PROGS([HELP2MAN], [help2man], [$am_aux_dir/missing help2man])
AC_ARG_VAR([HELP2MAN], [automatically generate simple manual pages])

# Texi2dvi
AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])
AC_ARG_VAR([TEXI2DVI], [PDF documentation])

# Doxygen
AC_CHECK_PROGS([DOXYGEN], [doxygen], [$am_aux_dir/missing doxygen])
AC_ARG_VAR([DOXYGEN], [A C++ documentation system program])

# Documentation generation
AC_ARG_ENABLE([documentation],
    [AS_HELP_STRING([--enable-documentation], [disable documentation generation])],
    [documentation=${enableval}],
    [documentation=no]
)
AM_CONDITIONAL([GENERATE_DOC], [test "$documentation" = "yes"])

# Libtool
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Pkg-config
PKG_PROG_PKG_CONFIG

#
# Configure system specific stuff.
#
LIBDL="-ldl"
LT_NO_UNDEFINED=-no-undefined
case $host_os in
  *cygwin*)
    AC_DEFINE([HOSTTYPE_CYGWIN], [], [host type is cygwin])
    LIBDL=
  ;;

  linux*)
    AC_DEFINE([HOSTTYPE_LINUX], [], [host type is linux])
  ;;

  netbsd*)
    AC_DEFINE([HOSTTYPE_NETBSD], [], [host type is netbsd])
    LIBDL=
  ;;

  freebsd*)
    AC_DEFINE([HOSTTYPE_FREEBSD], [], [host type is netbsd])
    LIBDL=
  ;;

  darwin*)
    AC_DEFINE([HOSTTYPE_DARWIN], [], [host type is darwin])
    LT_NO_UNDEFINED=
  ;;

  *)
    cat <<EOF
******************************************************************
WARNING: your system, _${host_os}_, was never tested. It may or
may not work.

I would however really appreciate that you report any success (or
failure) at <victor@towbowltactics.com>.

Good luck !
******************************************************************
EOF
    sleep 2
  ;;
esac
AC_SUBST([LIBDL])
AC_SUBST([LT_NO_UNDEFINED])
CFLAGS="$CFLAGS -Wall -Wextra -pipe"
CXXFLAGS="$CXXFLAGS -Wall -Wextra -std=c++0x -pipe"

# Readline (optional)
HAVE_READLINE=no
case $DEPENDS in
     *readline*) STECHEC_CHECK_READLINE ;;
esac
AM_CONDITIONAL([HAVE_READLINE], [test x"$HAVE_READLINE" = xyes])

# SDL
HAVE_SDL=no
HAVE_PARAGUI=no
case $DEPENDS in
    *sdl*) TBT_CHECK_SDL([1.2.0]) ;;
esac
AM_CONDITIONAL([HAVE_SDL], [test x"$HAVE_SDL" = xyes])
AM_CONDITIONAL([HAVE_PARAGUI], [test x"$HAVE_PARAGUI" = xyes])

# QT
HAVE_QT=no
case $DEPENDS in
    *qt*) STECHEC_CHECK_QT ;;
esac
AM_CONDITIONAL([HAVE_QT], [test x"$HAVE_QT" = xyes])

# Mysql (really optionnal)
HAVE_MYSQL=no
case $DEPENDS in
    *mysql*) STECHEC_CHECK_MYSQL ;;
esac
AM_CONDITIONAL([HAVE_MYSQL], [test x"$HAVE_MYSQL" = xyes])

# Python 2.x
HAVE_PYTHON=no
case $DEPENDS in
    *python*) STECHEC_CHECK_PYTHON ;;
esac
AM_CONDITIONAL([HAVE_PYTHON], [test x"$HAVE_PYTHON" = xyes])

# Check for pthread.
TBT_CHECK_PTHREAD

#
# Stechec (base system) output files
#
AC_CONFIG_FILES([
  doc/Makefile
  doc/doc.doxy
  stechec/Makefile
  stechec/base_rules/Makefile
  stechec/client/Makefile
  stechec/scripts/Makefile
  stechec/scripts/conf.rb
  stechec/server/Makefile
  stechec/tools/Makefile
  stechec/tools/test/Makefile
])

#
# Prologin ouput files
#
if test x$BUILD_prologin != x; then

AC_CONFIG_FILES([
  prologin/Makefile
  prologin/common/Makefile
  prologin/master-node/Makefile
  prologin/master-node/paths.py
  prologin/master-node/master-node
  prologin/master-node/master-node.yml
  prologin/worker-node/Makefile
  prologin/worker-node/paths.py
  prologin/worker-node/worker-node
  prologin/worker-node/worker-node.yml
])

if test x$BUILD_ant != x; then
AC_CONFIG_FILES([
  prologin/ant/Makefile
  prologin/ant/rules/Makefile
  prologin/ant/visu/Makefile
  prologin/ant/test/Makefile
  prologin/ant/subject/Makefile
  prologin/ant/data/Makefile
])
fi

if test x$BUILD_prolo2005 != x; then
AC_CONFIG_FILES([
  prologin/prolo2005/Makefile
  prologin/prolo2005/subject/Makefile
  prologin/prolo2005/rules/Makefile
  prologin/prolo2005/test/Makefile
  prologin/prolo2005/editor/Makefile
  prologin/prolo2005/data/Makefile
])
fi

if test x$BUILD_prolo2006 != x; then
AC_CONFIG_FILES([
  prologin/prolo2006/Makefile
  prologin/prolo2006/subject/Makefile
  prologin/prolo2006/rules/Makefile
  prologin/prolo2006/data/Makefile
  prologin/prolo2006/test/Makefile
  prologin/prolo2006/visu/Makefile
  prologin/prolo2006/virchor/Makefile
])
fi

if test x$BUILD_prolo2007 != x; then
AC_CONFIG_FILES([
  prologin/prolo2007/Makefile
  prologin/prolo2007/rules/Makefile
  prologin/prolo2007/visu/Makefile
  prologin/prolo2007/subject/Makefile
  prologin/prolo2007/data/Makefile
])
fi

if test x$BUILD_prolo2008 != x; then
AC_CONFIG_FILES([
  prologin/prolo2008/Makefile
  prologin/prolo2008/rules/Makefile
  prologin/prolo2008/subject/Makefile
  prologin/prolo2008/data/Makefile
  prologin/prolo2008/test/Makefile
  prologin/prolo2008/editor/Makefile
  prologin/prolo2008/gui/Makefile
])
fi

if test x$BUILD_prolo2009 != x; then
AC_CONFIG_FILES([
  prologin/prolo2009/Makefile
  prologin/prolo2009/rules/Makefile
  prologin/prolo2009/subject/Makefile
  prologin/prolo2009/data/Makefile
  prologin/prolo2009/gui/Makefile
])
fi

if test x$BUILD_prolo2010 != x; then
AC_CONFIG_FILES([
  prologin/prolo2010/Makefile
  prologin/prolo2010/data/Makefile
  prologin/prolo2010/rules/Makefile
  prologin/prolo2010/jsview/Makefile
  prologin/prolo2010/asciiinter/Makefile
  prologin/prolo2010/gui/Makefile
])
fi

if test x$BUILD_summer2010 != x; then
AC_CONFIG_FILES([
  prologin/summer2010/Makefile
  prologin/summer2010/data/Makefile
  prologin/summer2010/rules/Makefile
  prologin/summer2010/gui/Makefile
  prologin/summer2010/jsdumper/Makefile
])
fi

if test x$BUILD_prolo2011 != x; then
AC_CONFIG_FILES([
  prologin/prolo2011/Makefile
  prologin/prolo2011/data/Makefile
  prologin/prolo2011/rules/Makefile
  prologin/prolo2011/gui/Makefile
  prologin/prolo2011/gui/paths.py
  prologin/prolo2011/dumper/Makefile
])
fi

fi # !BUILD_prologin

AC_OUTPUT

##
## Print summary
##

show_modules=`echo ${used_modules} | sed 's/prologin//g;s/^,\+//;s/,\+$//;s/,\+/,/g'`
echo "
 ---{ $PACKAGE_NAME $VERSION }---

   prefix:                  ${prefix}
   compiler:                ${CXX}
   cxxflags:                ${CXXFLAGS}

   Building modules:        ${show_modules}
   Building with readline:  ${HAVE_READLINE}
   Building with sdl:	    ${HAVE_SDL}
   Building with paragui:   ${HAVE_PARAGUI}
   Generating doc:          ${documentation}
"
