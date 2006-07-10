# -*- shell-script -*-
#
# Check for readline compatible library
# Copyright © 2006 Olivier Gournet <victor@towbowltactics.com>
#

#
# STECHEC_CHECK_READLINE()
#
# Defines
#   HAVE_READLINE
#   READLINE_CFLAGS
#   READLINE_LIBS
#
AC_DEFUN([STECHEC_CHECK_READLINE], 
[
   AC_ARG_WITH([readline],
               AS_HELP_STRING([--with-readline=DIR], 
                              [compile with the specified readline library]),
               [dir_readline="$withval"])

   if test x"$dir_readline" != x; then
       READLINE_LIBS="-L$dir_readline/lib "
       READLINE_CFLAGS="-I$dir_readline/include"
   fi

   if test x"$dir_readline" != xno; then
       READLINE_LIBS="${READLINE_LIBS}-lreadline"
       sav_LIBS="$LIBS"
       sav_CPPFLAGS="$CPPFLAGS"
       LIBS="$LIBS $READLINE_LIBS"
       CPPFLAGS="$CPPFLAGS $READLINE_CFLAGS"

       AC_CHECK_LIB([readline], [rl_completion_matches], 
           [have_readline_lib=yes])
       if test x$have_readline_lib != x; then
           AC_CHECK_HEADERS([readline.h readline/readline.h], 
               [have_readline_dev=yes])
       fi
       CPPFLAGS="$sav_CPPFLAGS"
       LIBS="$sav_LIBS"

       if test x$have_readline_dev != xyes ; then
           READLINE_CFLAGS=""
           READLINE_LIBS=""
       fi
       AC_SUBST([READLINE_CFLAGS])
       AC_SUBST([READLINE_LIBS])
   fi
])
