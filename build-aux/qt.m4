# -*- shell-script -*-

#
# QT_CHECK_QTDIR()
#
# If $QTDIR was not set at this point, try guessing it
#
# --------------------------------------------------------------
AC_DEFUN([QT_CHECK_QTDIR],
[
  AC_MSG_CHECKING([QTDIR])

  if test x"$QTDIR" = x ; then
    QT_SEARCH="/usr/lib/qt3 /usr/local/qt3 /usr/lib/qt /usr/local/qt /usr/share/qt3 /usr/lib/qt-3.3 /sw /opt/qt/3.3 /opt/qt3"
    for i in $QT_SEARCH; do
        if test -f $i/include/qglobal.h -o -f $i/include/qt/qglobal.h; then
	    QTDIR=$i
	    break
	fi
    done
  fi
  AC_MSG_RESULT([$QTDIR])

  # Check that moc is in path
  AC_PATH_PROG([MOC], [moc], [], [$QTDIR/bin:$PATH])
  if test x$MOC = x ; then
    AC_MSG_ERROR([*** moc must be in path])
  fi

  # uic is the Qt user interface compiler
  AC_PATH_PROG([UIC], [uic], [], [$QTDIR/bin:$PATH])
  if test x$UIC = x ; then
    AC_MSG_ERROR([*** uic must be in path])
  fi

  export PKG_CONFIG_PATH="$QTDIR/lib/pkgconfig:$QTDIR/lib64/pkgconfig:$PKG_CONFIG_PATH"

])# QT_CHECK_QTDIR

#
# QT_CHECK_PC()
#
# Check for Qt compiler flags, linker flags, ... with pkg-config
# It seems it is now the universal way of doing it, and it's much
# easier...
#
# --------------------------------------------------------------
AC_DEFUN([QT_CHECK_PC],
[
  PKG_CHECK_MODULES([QT], [qt-mt])
  qt_config=`$PKG_CONFIG --variable=qt_config qt-mt`
  if test "`echo $qt_config | grep stl`" = ""; then
    AC_MSG_ERROR(
[QT (at $QTDIR) has not been installed with STL support.
This can happen if you are using an obsolete distro (slackware), or cygwin.

You must compile yourself a proper version of QT with STL enabled.
Alternatively, you can run configure with the flag: --without-qtdir
to skip building qt programs.])
  fi

  AC_SUBST([QT_CFLAGS])
  AC_SUBST([QT_LIBS])
])# QT_CHECK_PC

#
# QT_CHECK_OLD()
#
# Check for Qt compiler flags, linker flags, and binary packages
# This was an old m4 script found at http://autoqt.sourceforge.net/,
# and pretty much modified since then. But it is too much burden, and
# has been deprecated in favor to pkg-config.
#
# --------------------------------------------------------------
AC_DEFUN([QT_CHECK_OLD],
[
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PATH_X])

if test x"$QTDIR" = x ; then
    AC_MSG_ERROR([*** QTDIR must be defined, or --with-qtdir option given])
fi

# Change backslashes in QTDIR to forward slashes to prevent escaping
# problems later on in the build process, mainly for Cygwin build
# environment using MSVC as the compiler
# TODO: Use sed instead of perl
QTDIR=`echo $QTDIR | perl -p -e 's/\\\\/\\//g'`

# Figure out which version of Qt we are using
AC_MSG_CHECKING([Qt version])
QTINC="include"
test -f $QTDIR/include/qt/qglobal.h && QTINC="include/qt"
QT_VER=`grep 'define.*QT_VERSION_STR\W' $QTDIR/$QTINC/qglobal.h | perl -p -e 's/\D//g'`
case "${QT_VER}" in
    3*)
        QT_MAJOR="3"
    ;;
    *)
        AC_MSG_ERROR([*** Dont know how to handle this Qt major version])
    ;;
esac
AC_MSG_RESULT([$QT_VER ($QT_MAJOR)])

# qembed is the Qt data embedding utility.
# It is located in $QTDIR/tools/qembed, and must be compiled and installed
# manually, we'll let it slide if it isn't present
AC_CHECK_PROG(QEMBED, qembed, qembed)


# Calculate Qt include path
QT_CFLAGS="-I$QTDIR/$QTINC"

QT_IS_EMBEDDED="no"
# On unix, figure out if we're doing a static or dynamic link
case "${host}" in
    *cywin*)
	AC_DEFINE_UNQUOTED(WIN32, "", Defined if on Win32 platform)
        if test -f "$QTDIR/lib/libqt-mt.dll" ; then
            QT_LIB="libqt-mt.dll"
            QT_IS_STATIC="no"
            QT_IS_MT="yes"
        elif test -f "$QTDIR/lib/libqt-mt.dll.a" ; then
            QT_LIB="libqt-mt.dll.a" 
            QT_IS_STATIC="yes"
            QT_IS_MT="yes"
        elif test -f "$QTDIR/lib/libqt.dll" ; then
            QT_LIB="libqt.dll"
            QT_IS_STATIC="no"
            QT_IS_MT="no"
        elif test -f "$QTDIR/lib/libqt.dll.a" ; then
            QT_LIB="libqt.dll.a"
            QT_IS_STATIC="yes"
            QT_IS_MT="no"
        else
            AC_MSG_ERROR(["*** Couldn't find any Qt libraries"])
        fi
        ;;

    *)
        QT_IS_STATIC=`ls $QTDIR/lib/libqt*.a 2> /dev/null`
        if test "x$QT_IS_STATIC" = x; then
            QT_IS_STATIC="no"
        else
            QT_IS_STATIC="yes"
        fi
        if test x$QT_IS_STATIC = xno ; then
            QT_IS_DYNAMIC=`ls $QTDIR/lib/libqt*.{so,dylib} 2> /dev/null` 
            if test "x$QT_IS_DYNAMIC" = x;  then
                AC_MSG_ERROR(["*** Couldn't find any Qt libraries"])
            fi
        fi

        if test "x`ls $QTDIR/lib/libqt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqt"
            QT_IS_MT="no"
        elif test "x`ls $QTDIR/lib/libqt-mt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqt-mt"
            QT_IS_MT="yes"
        elif test "x`ls $QTDIR/lib/libqte.* 2> /dev/null`" != x ; then
            QT_LIB="-lqte"
            QT_IS_MT="no"
            QT_IS_EMBEDDED="yes"
        elif test "x`ls $QTDIR/lib/libqte-mt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqte-mt"
            QT_IS_MT="yes"
            QT_IS_EMBEDDED="yes"
        fi
        ;;
esac

# Check if qt has STL support. We need it
check_stl_qt() 
{
  stechec_cv_header_qt_stl=no
  qt_save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $QT_CFLAGS"
  AC_CHECK_HEADER([qglobal.h],
                  [AC_COMPILE_IFELSE([AC_LANG_SOURCE([
@%:@include <qglobal.h>
@%:@ifdef QT_NO_STL
@%:@ error "No stl support."
@%:@endif
                                                      ])],
                   [stechec_cv_header_qt_stl=yes])])
  CPPFLAGS=$qt_save_CPPFLAGS
  if test "x$stechec_cv_header_qt_stl" != xyes; then
    AC_MSG_ERROR(
[QT has not been installed with STL support.

QT at $QTDIR was not configured to use STL.
This can happen if you are using an obsolete distro (slackware).
In any case, you must compile yourself a proper version of QT with STL enabled.
Alternatively, you can run configure with the flag: --without-qtdir
to skip the build of programs using qt.])
  fi
}

AC_CACHE_VAL([stechec_cv_header_qt_stl],
             [check_stl_qt])


AC_MSG_CHECKING([if QT is static])
AC_MSG_RESULT([$QT_IS_STATIC])
AC_MSG_CHECKING([if QT is multithreaded])
AC_MSG_RESULT([$QT_IS_MT])
AC_MSG_CHECKING([if QT is embedded])
AC_MSG_RESULT([$QT_IS_EMBEDDED])

QT_GUILINK=""
QASSISTANTCLIENT_LDADD="-lqassistantclient"
case "${host}" in
    *irix*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXext -lX11 -lm -lSM -lICE"
        fi
        ;;

    *linux*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes && test $QT_IS_EMBEDDED = no; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXext -lX11 -lm -lSM -lICE -ldl -ljpeg"
        fi
        ;;

    *darwin*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes && test $QT_IS_EMBEDDED = no; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXext -lX11 -lm -lSM -lICE -ldl -ljpeg"
        fi
        ;;

    *netbsd*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes && test $QT_IS_EMBEDDED = no; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXfont -lfntstubs -lXext -lX11 -lm -lSM -lICE -ljpeg"
        fi
        ;;

    *osf*) 
        # Digital Unix (aka DGUX aka Tru64)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXext -lX11 -lm -lSM -lICE"
        fi
        ;;

    *solaris*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lXext -lX11 -lm -lSM -lICE -lresolv -lsocket -lnsl"
        fi
        ;;


    *win*)
        # linker flag to suppress console when linking a GUI app on Win32
        QT_GUILINK="/subsystem:windows"

        if test $QT_IS_MT = yes ; then
           QT_LIBS="/nodefaultlib:libcmt"
        else
           QT_LIBS="/nodefaultlib:libc"
        fi

        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS $QT_LIB kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib shell32.lib imm32.lib advapi32.lib wsock32.lib winspool.lib winmm.lib netapi32.lib qtmain.lib"
        else
            QT_LIBS="$QT_LIBS $QT_LIB"        
            QT_CFLAGS="$QT_CFLAGS -DQT_DLL"
            QT_LIBS="$QT_LIBS qtmain.lib qui.lib user32.lib netapi32.lib"
        fi
        QASSISTANTCLIENT_LDADD="qassistantclient.lib"
        ;;

esac


if test x"$QT_IS_EMBEDDED" = "xyes" ; then
        QT_CFLAGS="-DQWS $QT_CFLAGS"
fi

if test x"$QT_IS_MT" = "xyes" ; then
        QT_CFLAGS="$QT_CFLAGS -D_REENTRANT -DQT_THREAD_SUPPORT"
fi

QT_LIBS="-L$QTDIR/lib $QT_LIBS"

if test x$QT_IS_STATIC = xyes ; then
    OLDLIBS="$LIBS"
    LIBS="$QT_LIBS"
    AC_CHECK_LIB(Xft, XftFontOpen, QT_LIBS="$QT_LIBS -lXft")
    LIBS="$LIBS"
fi

AC_MSG_CHECKING([QT_CFLAGS])
AC_MSG_RESULT([$QT_CFLAGS])
AC_MSG_CHECKING([QT_LIBS])
AC_MSG_RESULT([$QT_LIBS])

AC_SUBST(QT_CFLAGS)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_GUILINK)
AC_SUBST(QASSISTANTCLIENT_LDADD)
])

#
# STECHEC_CHECK_QT()
#
# Define:
#  QT_CFLAGS
#  QT_LIBS
#  HAVE_QT
#
# --------------------------------------------------------------
AC_DEFUN([STECHEC_CHECK_QT],
[
  AC_ARG_WITH([qt],
	[  --with-qt=DIR        Qt installation directory [default=$QTDIR]],
	[QTDIR=$withval])

  if test "x$QTDIR" != "xno"; then
    QT_CHECK_QTDIR

    AC_CHECK_PROG([PKGCONFIG], [pkg-config], [pkg-config])

    # some distros do not ship pkg-config file yet...
    old_IFS=$IFS
    IFS=":"
    for path in $PKG_CONFIG_PATH; do
	if test -r "$path/qt-mt.pc"; then
	    HAVE_PC_FILE=1
	    break
	fi
    done
    IFS=$old_IFS

    if test $PKGCONFIG && test $HAVE_PC_FILE; then 
      QT_CHECK_PC
    else
      QT_CHECK_OLD
    fi
    
    # Actually, if it was missing, an error is issued.
    # Thus, going here means we have it. This should be fixed.
    HAVE_QT=yes
  fi
])# QT_CHECK
