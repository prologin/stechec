# STECHEC_CHECK_PYTHON()
#
# Description:
#   Detect the Python 2.x installation
#
# Defines
#   PYTHON        - Python executable
#   PYTHON_CFLAGS - C include paths
#   PYTHON_LIBS   - libraries
#   HAVE_PYTHON   - only if Python 2.x was successfully detected

AC_DEFUN([STECHEC_CHECK_PYTHON],
[
  AC_ARG_WITH([python],
    AS_HELP_STRING([--with-python=EXE],
                   [specify the path to a Python 2.x interpreter]),
    [stechec_cv_python=${withval}],
    [stechec_cv_python=no])

  HAVE_PYTHON=no
  if test x"$stechec_cv_python" = x"no"; then
    AC_PATH_PROGS([stechec_cv_python], [python2 python])
    if test "`"$stechec_cv_python" -c "import sys; print sys.version[[0]]"`" = 2; then
      HAVE_PYTHON=yes
      AC_SUBST([PYTHON], ["$stechec_cv_python"])
    fi
  else
    HAVE_PYTHON=yes
    AC_SUBST([PYTHON], ["$stechec_cv_python"])
  fi

  if test x"$HAVE_PYTHON" = x"yes"; then
    stechec_cv_pyconfig="$PYTHON"-config
    AC_CHECK_PROG([stechec_cv_pyconfig], ["$PYTHON"-config],
                  ["$PYTHON"-config], [no])
    if test x"$stechec_cv_pyconfig" = x"no"; then
      AC_MSG_ERROR(["could not find $PYTHON-config"])
    fi
    AC_SUBST([PYTHON_CFLAGS], ["`"$stechec_cv_pyconfig" --includes`"])
    AC_SUBST([PYTHON_LIBS], ["`"$stechec_cv_pyconfig" --libs`"])
  fi
])
