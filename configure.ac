# -*-Autconf-*-
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# ++                           T B T                                ++
# ++                        configure.ac                            ++
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# Autconf init
AC_PREREQ(2.59)
AC_INIT([stechec], [3.0], [https://projects.nekeme.net/projects/tbt/newticket])

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
modules="bb5"

# Existing modules/meta-modules
STECHEC_ADDMODULE([bb5])
STECHEC_ADDMODULE([prologin])
STECHEC_ADDMODULE([ant])
STECHEC_ADDMODULE([prolo2005])
STECHEC_ADDMODULE([prolo2006])

# Modules dependencies
DEPENDS=""
DEPENDS_bb5="sdl,readline"
DEPENDS_prologin="sdl,mysql"
DEPENDS_ant="prologin,qt"
DEPENDS_prolo2005="prologin,qt"
DEPENDS_prolo2006="prologin,qt"

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

# Doxygen
AC_CHECK_PROGS([DOXYGEN], [doxygen], [$am_aux_dir/missing doxygen])
AC_ARG_VAR([DOXYGEN], [A C++ documentation system program])

# Libtool
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

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
case $DEPENDS in
     *readline*) STECHEC_CHECK_READLINE() ;;
esac

# SDL
case $DEPENDS in
    *sdl*) TBT_CHECK_SDL([1.2.0]) ;;
    *) AM_CONDITIONAL([HAVE_SDL], [false])
       AM_CONDITIONAL([HAVE_PARAGUI], [false])
     ;;
esac

# QT
case $DEPENDS in
    *qt*) TBT_CHECK_QT ;;
    *) AM_CONDITIONAL([HAVE_QT], [false]) ;;
esac

# Xerces (XML parser)
TBT_CHECK_XML

# Mysql (really optionnal)
case $DEPENDS in
    *mysql*) STECHEC_CHECK_MYSQL ;;
    *) AM_CONDITIONAL([HAVE_MYSQL], [false]) ;;
esac

# Check for pthread.
TBT_CHECK_PTHREAD

#
# Stechec (base system) output files
#
AC_CONFIG_FILES([
  doc/Makefile
  doc/doc.doxy
  conf/Makefile
  stechec/Makefile
  stechec/base_rules/Makefile
  stechec/client/Makefile
  stechec/scripts/Makefile
  stechec/scripts/conf.rb
  stechec/scripts/meta_server/meta_cx.sh
  stechec/server/Makefile
  stechec/tools/Makefile
  stechec/tools/xml_test/Makefile
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
  bb5/teamroster2/Makefile
  bb5/visu2d/Makefile
])
fi

#
# Prologin ouput files
#
if test x$BUILD_prologin != x; then

AC_CONFIG_FILES([
  prologin/Makefile
  prologin/gui_qt/Makefile
  prologin/meta_server/Makefile
  prologin/main_server/Makefile
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
  prologin/prolo2005/visu2d/Makefile
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
  prologin/prolo2006/sdlvisu/Makefile
  prologin/prolo2006/virchor/Makefile
])
fi

fi # !BUILD_prologin

AC_OUTPUT
