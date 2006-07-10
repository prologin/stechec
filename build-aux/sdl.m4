# -*- shell-script -*-
# Configure paths for SDL
# Sam Lantinga 9/21/99
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

# AM_PATH_SDL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for SDL, and define SDL_CFLAGS and SDL_LIBS
#
AC_DEFUN([AM_PATH_SDL],
[
  AC_ARG_ENABLE([sdltest],
  [  --disable-sdltest       Do not try to compile and run a test SDL program],
  :,
  [enable_sdltest=yes])

  if test x$tbt_cv_sdl_prefix != x ; then
     sdl_args="$sdl_args --prefix=$tbt_cv_sdl_prefix"
     if test x${SDL_CONFIG+set} != xset ; then
        SDL_CONFIG=$tbt_cv_sdl_prefix/bin/sdl-config
     fi
  fi

  PATH="$prefix/bin:$prefix/usr/bin:$PATH"
  AC_PATH_PROG([SDL_CONFIG], [sdl-config], [no], [$PATH])
  AC_ARG_VAR([SDL_CONFIG], [Path to sdl-config program])
  min_sdl_version=ifelse([$1], ,0.11.0,$1)
  AC_MSG_CHECKING([for SDL - version >= $min_sdl_version])
  no_sdl=""
  if test "$SDL_CONFIG" = "no" ; then
    no_sdl=yes
  else
    SDL_CFLAGS=`$SDL_CONFIG $sdlconf_args --cflags`
    SDL_LIBS=`$SDL_CONFIG $sdlconf_args --libs`

    sdl_major_version=`$SDL_CONFIG $sdl_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sdl_minor_version=`$SDL_CONFIG $sdl_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sdl_micro_version=`$SDL_CONFIG $sdl_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_sdltest" = "xyes" ; then
      CFLAGS="$CFLAGS $SDL_CFLAGS"
      CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
      LDFLAGS="$LDFLAGS $SDL_LIBS"
dnl
dnl Now check if the installed SDL is sufficiently new. (Also sanity
dnl checks the results of sdl-config to some extent
dnl
      rm -f conf.sdltest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

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

  /* This hangs on some systems (?)
  system ("touch conf.sdltest");
  */
  { FILE *fp = fopen("conf.sdltest", "a"); if ( fp ) fclose(fp); }

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_sdl_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_sdl_version");
     exit(1);
   }

   if (($sdl_major_version > major) ||
      (($sdl_major_version == major) && ($sdl_minor_version > minor)) ||
      (($sdl_major_version == major) && ($sdl_minor_version == minor) && ($sdl_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'sdl-config --version' returned %d.%d.%d, but the minimum version\n", $sdl_major_version, $sdl_minor_version, $sdl_micro_version);
      printf("*** of SDL required is %d.%d.%d. If sdl-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If sdl-config was wrong, set the environment variable SDL_CONFIG\n");
      printf("*** to point to the correct copy of sdl-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_sdl=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
     fi
  fi
  if test "x$no_sdl" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$SDL_CONFIG" = "no" ; then
       echo "*** The sdl-config script installed by SDL could not be found"
       echo "*** If SDL was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the SDL_CONFIG environment variable to the"
       echo "*** full path to sdl-config."
     else
       if test -f conf.sdltest ; then
        :
       else
          echo "*** Could not run SDL test program, checking why..."
          CFLAGS="$CFLAGS $SDL_CFLAGS"
          LDFLAGS="$LDFLAGS $SDL_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include "SDL.h"

int main(int argc, char *argv[])
{ return 0; }
#undef  main
#define main K_and_R_C_main
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding SDL or finding the wrong"
          echo "*** version of SDL. If it is not finding SDL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means SDL was incorrectly installed"
          echo "*** or that you have moved SDL since it was installed. In the latter case, you"
          echo "*** may want to edit the sdl-config script: $SDL_CONFIG" ])
       fi
     fi
     SDL_CFLAGS=""
     SDL_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  rm -f conf.sdltest
])

#
# SDL_CHECK_EXT(name, with-arg, header-file, library, library-symb, website)
#
# Check an extension, and set SDL_LIBS and SDL_CFLAGS
#
AC_DEFUN([SDL_CHECK_EXT],
[
  AC_ARG_WITH([$2],
      AC_HELP_STRING([--with-$2=DIR], [specify where $1 library is installed [default=SDL]]),
      [tbt_cv_with_$1="$withval"])
  if test "x$tbt_cv_with_$1" != "x"; then
      CPPFLAGS="$CPPFLAGS -I$tbt_cv_with_$1/include/SDL"
      CFLAGS="$CFLAGS -I$tbt_cv_with_$1/include/SDL"
      LDFLAGS="$LDFLAGS -L$tbt_cv_with_$1/lib"
      SDL_CFLAGS="$SDL_CFLAGS -I$tbt_cv_with_$1/include/SDL"
      SDL_LIBS="$SDL_LIBS -L$tbt_cv_with_$1/lib"
  fi

  AC_CHECK_LIB([$4], [$5], [have_$1=yes])
  if test x$have_$1 = x; then
     AC_MSG_ERROR([
********** $1 not found ! **********
Please check your configuration twice, or download and install the library:
$6
If you installed this library in a non standard location, you can try with
--with-$2=DIR flag.
*******************************************])
  fi

  AC_CHECK_HEADER([$3], [have_$1_dev=yes])
  if test x$have_$1_dev = x; then
     AC_MSG_ERROR([
*** $1 development library not found ! ***
Please check your configuration twice, or download and install the library:
$6
*************************************************])
  fi
  SDL_LIBS="$SDL_LIBS -l$4"
])

#
# TBT_CHECK_SDL(MIN_SDL_VERSION)
#
# Check for (optional) sdl presence, and sets SDL_CFLAGS and SDL_LIBS
#
AC_DEFUN([TBT_CHECK_SDL],
[
  AC_ARG_WITH([sdl], 
              AC_HELP_STRING([--with-sdl=DIR], [Prefix where SDL is installed (optional)]),
              [tbt_cv_sdl_prefix="$withval"],
              [tbt_cv_sdl_prefix=""])

  if test "x$tbt_cv_sdl_prefix" = xno; then
      # Explicitly disabled by user.
      AM_CONDITIONAL([HAVE_SDL], [false])
      AC_MSG_CHECKING([for SDL - version >= $1])
      AC_MSG_RESULT([disabled by user])
  else
      # save our gcc flags.
      ac_save_CFLAGS="$CFLAGS"
      ac_save_CPPFLAGS="$CPPFLAGS"
      ac_save_LDFLAGS="$LDFLAGS"

      #
      # Make sure we have sdl. If not, this is an error.
      #
      AM_PATH_SDL([$1],
          :,
	  [AC_MSG_ERROR([
*** SDL was not found or does not properly work ! ***
Please fix your configuration, or download and install the library from:
http://www.libsdl.org/

However, you can choose to _not_ build sdl programs by using 
'--without-sdl' configure flag.
Warning: This is probably _not_ what you want.
*****************************************************])
      ])

      #
      # Check for SDL extensions.
      #

      # Hack to remove -lSDL from LDFLAGS. It breaks AC_CHECK_LIB on Darwin,
      # with this #$%%$& double main() definition.
      LDFLAGS=`echo $LDFLAGS | sed 's/-lSDLmain//'`

      # Check for SDL_image
      SDL_CHECK_EXT([SDL_image], [sdl-image], [SDL_image.h], [SDL_image], [IMG_Load],
	  [http://www.libsdl.org/projects/SDL_image/])

      # Check for SDL_mixer
#     SDL_CHECK_EXT([SDL_mixer], [sdl-mixer], [SDL_mixer.h], [SDL_mixer], [Mix_OpenAudio],
#         [http://www.libsdl.org/projects/SDL_mixer/])

      # Check for SDL_ttf
      SDL_CHECK_EXT([SDL_ttf], [sdl-ttf], [SDL_ttf.h], [SDL_ttf], [TTF_OpenFont],
          [http://www.libsdl.org/projects/SDL_ttf/])

      # Check for SDL_gfx
      SDL_CHECK_EXT([SDL_gfx], [sdl-gfx], [SDL_rotozoom.h], [SDL_gfx], [rotozoomSurface],
          [http://www.ferzkopp.net/Software/SDL_gfx-2.0/])

      # Check for Paragui, only for bb5 module.
      case $used_modules in
          *bb5*)  STECHEC_CHECK_PARAGUI([1.1.8]) ;;
          *)      AM_CONDITIONAL([HAVE_PARAGUI], [false]) ;;
      esac

      AM_CONDITIONAL([HAVE_SDL], [true])
      AC_DEFINE([HAVE_SDL], [], [Have SDL])
      AC_SUBST([SDL_CFLAGS])
      AC_SUBST([SDL_LIBS])

      # restore gcc flags.
      CFLAGS="$ac_save_CFLAGS"
      CPPFLAGS="$ac_save_CPPFLAGS"
      LDFLAGS="$ac_save_LDFLAGS"

  fi # !use_sdl
])
