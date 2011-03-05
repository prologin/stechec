# -*- shell-script -*-

# Configure paths for ParaGUI
# Alexander Pipelka 17.05.2000
# stolen from Sam Lantinga 
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor
#
# Remodified by Victor (C) 2006
#

#
# STECHEC_CHECK_PARAGUI([MINIMUM-VERSION])
#
# Purpose:
#   Test for PARAGUI, a widget library for SDL.
#
# Defines:
#   PARAGUI_CFLAGS
#   PARAGUI_LIBS
#   HAVE_PARAGUI
#
AC_DEFUN([STECHEC_CHECK_PARAGUI],
[

  AC_ARG_WITH([paragui],
            AS_HELP_STRING([--with-paragui=DIR],
                           [Prefix where PARAGUI is installed (optional)]),
            [paragui_prefix="$withval"],
            [paragui_prefix=""])

  AC_ARG_ENABLE([paraguitest], 
              AS_HELP_STRING([--disable-paraguitest],
                             [Do not try to compile and run a test PARAGUI program]),
              [enable_paraguitest=yes])

  min_paragui_version=ifelse([$1], , 0.11.0, $1)

  no_paragui=yes
  if test x$paragui_prefix != xno; then
    if test x$paragui_prefix != x ; then
       paragui_args="$paragui_args --prefix=$paragui_prefix"
       if test x${PARAGUI_CONFIG+set} != xset ; then
          PARAGUI_CONFIG=$paragui_prefix/bin/paragui-config
       fi
    fi
    AC_PATH_PROG([PARAGUI_CONFIG], [paragui-config], [no], [$prefix/usr/bin:$prefix/bin:$PATH])
    
    AC_MSG_CHECKING([for PARAGUI - version >= $min_paragui_version])

    if test "$PARAGUI_CONFIG" != "no" ; then
      no_paragui=""
      PARAGUI_CFLAGS=`$PARAGUI_CONFIG $paraguiconf_args --cflags`
      PARAGUI_LIBS=`$PARAGUI_CONFIG $paraguiconf_args --libs`

      paragui_major_version=`$PARAGUI_CONFIG $paragui_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
      paragui_minor_version=`$PARAGUI_CONFIG $paragui_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
      paragui_micro_version=`$PARAGUI_CONFIG $paragui_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
      if test "x$enable_paraguitest" = "xyes" ; then
        ac_save_CFLAGS="$CFLAGS"
        ac_save_LIBS="$LIBS"
        CFLAGS="$CFLAGS $PARAGUI_CFLAGS"
        LIBS="$LIBS $PARAGUI_LIBS"
dnl
dnl Now check if the installed PARAGUI is sufficiently new. (Also sanity
dnl checks the results of paragui-config to some extent
dnl
      rm -f conf.paraguitest
      AC_LANG_CPLUSPLUS
      AC_TRY_RUN([
#include "paragui.h"
#include "pgapplication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (int argc, char *argv[])
{
  int major, minor, micro;
  char *tmp_version;

  PG_Application app;
  
  { FILE *fp = fopen("conf.paraguitest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_paragui_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_paragui_version");
     exit(1);
   }

   if (($paragui_major_version > major) ||
      (($paragui_major_version == major) && ($paragui_minor_version > minor)) ||
      (($paragui_major_version == major) && ($paragui_minor_version == minor) && ($paragui_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'paragui-config --version' returned %d.%d.%d, but the minimum version\n", $paragui_major_version, $paragui_minor_version, $paragui_micro_version);
      printf("*** of PARAGUI required is %d.%d.%d.\n", major, minor, micro);
      return 1;
    }
}

],, [no_paragui=yes], [echo $ac_n "cross compiling; assumed OK... $ac_c"])
         CFLAGS="$ac_save_CFLAGS"
         LIBS="$ac_save_LIBS"
       fi # $enable_paraguitest
    fi # $PARAGUI_CONFIG != no
  fi # $paragui_prefix != no


  if test x$no_paragui = x ; then
      # Yeah, it _is_ present !
      AC_MSG_RESULT([yes])
      HAVE_PARAGUI=yes
  else
      # It is not present. Try to print why. This is not fatal.
      if test x$paragui_prefix != xno; then
          AC_MSG_RESULT([no])
      else
          AC_MSG_RESULT([no, disabled by user])
      fi

      PARAGUI_CFLAGS=""
      PARAGUI_LIBS=""
  fi # $no_paragui = yes

  AC_SUBST([PARAGUI_CFLAGS])
  AC_SUBST([PARAGUI_LIBS])
  rm -f conf.paraguitest
])
