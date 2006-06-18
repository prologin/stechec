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
    echo "cv readline before: $stechec_cv_dir_readline"
   AC_ARG_WITH([readline],
               AS_HELP_STRING([--with-readline=DIR], 
                              [compile with the specified readline library]),
               [stechec_cv_dir_readline="$withval"])

    echo "cv readline after arg: $stechec_cv_dir_readline"
   if test x"$stechec_cv_dir_readline" = x; then
       stechec_cv_dir_readline=yes
   fi
   AC_CACHE_CHECK([whether to use readline],
       [stechec_cv_dir_readline],
       [stechec_cv_dir_readline=yes])
   echo "cv readline after cache: $stechec_cv_dir_readline"
   if test x"$stechec_cv_dir_readline" = xyes; then
       stechec_cv_dir_readline=""
   fi
   echo "cv readline after cache: $stechec_cv_dir_readline"

   if test x"$stechec_cv_dir_readline" != x; then
       readline_libdir="-L$stechec_cv_dir_readline "
       READLINE_CFLAGS="-I$stechec_cv_dir_readline"
   fi

   if test x"$stechec_cv_dir_readline" != xno; then
       sav_LIBS="$LIBS"
       sav_CPPFLAGS="$CPPFLAGS"
       LIBS="$LIBS ${readline_libdir}-lreadline"
       CPPFLAGS="$CPPFLAGS $READLINE_CFLAGS"
       AC_CHECK_LIB([readline], 
           [rl_completion_matches], 
           [have_readline=yes])
       if test x$have_readline; then
           AC_CHECK_HEADERS([readline.h readline/readline.h], 
               [have_readline_dev=yes])
       fi
       CPPFLAGS="$sav_CPPFLAGS"
       LIBS="$sav_LIBS"

       if test x$have_readline_dev = xyes; then
           AC_DEFINE([HAVE_READLINE], [1],
               [Define if you have a readline compatible library])
           AC_SUBST([READLINE_CFLAGS])
           AC_SUBST([READLINE_LIBS], [${readline_libdir}-lreadline])
       fi
   fi
])
