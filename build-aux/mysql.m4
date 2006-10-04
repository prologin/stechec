# -*- shell-script -*-

# STECHEC_CHECK_MYSQL()
#
# Description:
#   Detect the Mysql installation
#
# Defines 
#   MYSQL_CFLAGS - C include paths
#   MYSQL_LIBS   - libraries 
#   HAVE_MYSQL   - only if mysql was successfully detected
#

AC_DEFUN([STECHEC_CHECK_MYSQL],
[

AC_ARG_WITH([mysql],
  AS_HELP_STRING([--with-mysql=DIR],
                 [compile prologin meta/main server (optionally indicate location of mysql)]),
    [stechec_cv_mysql=${withval}],
    [stechec_cv_mysql=no])

HAVE_MYSQL=no
if test "$stechec_cv_mysql" != "no"; then
  [na_parentprefix=`echo "$prefix"|sed -e 's%\(.\)/[^/]*/*$%\1%'`]

  na_searchpath=${stechec_cv_mysql}/bin:${PATH}:/opt/mysql/bin:$prefix/bin:$na_parentprefix/mysql/bin:/usr/bin
  AC_PATH_PROG([stechec_cv_mysql], [mysql_config] , [not_found], [$na_searchpath])
  if test -x "$stechec_cv_mysql" ; then
      HAVE_MYSQL=yes
      AC_SUBST([MYSQL_CFLAGS], [`$stechec_cv_mysql --cflags`])
      AC_SUBST([MYSQL_LIBS], [`$stechec_cv_mysql --libs`])
  fi
fi

])

