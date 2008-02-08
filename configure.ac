# -*-Autconf-*-

# Autconf init
AC_PREREQ(2.59)
AC_INIT([stechec], [3.2.3], [http://projects.nekeme.net/projects/tbt/newticket])

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
modules="prolo2008"

# Existing modules/meta-modules
STECHEC_ADDMODULE([bb5])
STECHEC_ADDMODULE([prologin])
STECHEC_ADDMODULE([ant])
STECHEC_ADDMODULE([prolo2005])
STECHEC_ADDMODULE([prolo2006])
STECHEC_ADDMODULE([prolo2007])
STECHEC_ADDMODULE([prolo2008])

# Modules dependencies
DEPENDS=""
DEPENDS_bb5="sdl,readline"
DEPENDS_prologin="sdl,mysql"
DEPENDS_ant="prologin"
DEPENDS_prolo2005="prologin"
DEPENDS_prolo2006="prologin"
DEPENDS_prolo2007="prologin"
DEPENDS_prolo2008="prologin"

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
        [echo "compiling in debug mode ... -ggdb3";
         CXXFLAGS="-ggdb3 -W -Wall";
         CFLAGS="-ggdb3 -W -Wall"])

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
    [AS_HELP_STRING([--disable-documentation], [disable documentation generation])],
    [documentation=${enableval}],
    [documentation=yes]
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
CFLAGS="$CFLAGS -pipe"
CXXFLAGS="$CXXFLAGS -pipe"

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

# Xerces (XML parser)
TBT_CHECK_XML

# Mysql (really optionnal)
HAVE_MYSQL=no
case $DEPENDS in
    *mysql*) STECHEC_CHECK_MYSQL ;;
esac
AM_CONDITIONAL([HAVE_MYSQL], [test x"$HAVE_MYSQL" = xyes])

# Check for pthread.
TBT_CHECK_PTHREAD

#
# Stechec (base system) output files
#
AC_CONFIG_FILES([
  doc/Makefile
  doc/doc.doxy
  conf/Makefile
  conf/config_prolo.xml
  stechec/Makefile
  stechec/base_rules/Makefile
  stechec/client/Makefile
  stechec/scripts/Makefile
  stechec/scripts/conf.rb
  stechec/server/Makefile
  stechec/tools/Makefile
  stechec/tools/test/Makefile
  stechec/tools/xml_test/Makefile
  stechec/sdl/Makefile
  stechec/sdl/test/Makefile
])

#
# BB5 output files
#
if test x$BUILD_bb5 != x; then
AC_CONFIG_FILES([
  bb5/Makefile
  bb5/ai/Makefile
  bb5/arbiter/Makefile
  bb5/client/Makefile
  bb5/common/Makefile
  bb5/console/Makefile
  bb5/data/Makefile
  bb5/data/default_config.xml
  bb5/data/team/amazon.xml
  bb5/data/team/chaos.xml
  bb5/data/formation/default.xml
  bb5/teamroster2/Makefile
  bb5/visu2d/Makefile
  bb5/rules_test/Makefile
  bb5/subject/Makefile
])
fi

#
# Prologin ouput files
#
if test x$BUILD_prologin != x; then

AC_CONFIG_FILES([
  prologin/Makefile
  prologin/web_meta/Makefile
  prologin/web_meta/stechec.yml
  prologin/common/Makefile
  prologin/tkrun/Makefile
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
