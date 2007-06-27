# -*- shell-script -*-
# Configure paths for SDL
# Sam Lantinga 9/21/99
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

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
  AC_ARG_ENABLE([sdl],
      [AS_HELP_STRING([--enable-sdl], [Don't build SDL programs (default=enabled)])],
      [enable_sdl="$enableval"],
      [enable_sdl=yes]
  )
           
  if test $enable_sdl = "yes" ; then

      # save our gcc flags.
      ac_save_CFLAGS="$CFLAGS"
      ac_save_CPPFLAGS="$CPPFLAGS"
      ac_save_LDFLAGS="$LDFLAGS"

      sdl_min_version="$1"

      # Make sure we have sdl. If not, this is an error.
      PKG_CHECK_EXISTS([sdl >= $sdl_min_version], [have_sdl="yes"], [have_sdl="no"])

      if test $have_sdl = "no"; then
          	  AC_MSG_ERROR([
*** SDL was not found or does not properly work ! ***
Please fix your configuration, or download and install the library from:
http://www.libsdl.org/

However, you can choose to _not_ build sdl programs by using 
'--disable-sdl' configure flag.
Warning: This is probably _not_ what you want.
*****************************************************])
      fi

      # Set SDL_* variables
      PKG_CHECK_MODULES([SDL], [sdl >= $sdl_min_version])

      # Use SDL_CFLAGS and SDL_LIBS found previously
      CFLAGS="$CFLAGS $SDL_CFLAGS"
      CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
      LDFLAGS="$LDFLAGS $SDL_LIBS"

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
      esac

      HAVE_SDL=yes
      AC_DEFINE([HAVE_SDL], [], [Have SDL])
      AC_SUBST([SDL_CFLAGS])
      AC_SUBST([SDL_LIBS])

      # restore gcc flags.
      CFLAGS="$ac_save_CFLAGS"
      CPPFLAGS="$ac_save_CPPFLAGS"
      LDFLAGS="$ac_save_LDFLAGS"

  fi # !use_sdl
])
