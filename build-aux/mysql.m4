dnl TBT_CHECK_MYSQL(REQUIRED)
dnl
dnl Arguments
dnl   REQUIRED - required/optional
dnl
dnl Description:
dnl   Detect the Mysql installation
dnl
dnl Defines 
dnl   MYSQL_CFLAGS - C include paths
dnl   MYSQL_LIBS   - libraries 
dnl   HAVE_MYSQL   - only if mysql was successfully detected
dnl
dnl Notes
dnl   If REQUIRED is set to 'required' and the package
dnl   is not found, a fatal error message will be issued
dnl   and configure will exit.

AC_DEFUN([TBT_CHECK_MYSQL],
[
na_required=$1

AC_ARG_WITH(mysql,
[  --with-mysql[=dir]        indicate location of the mysql installation],
[na_mysql=${withval}],
[na_mysql=${MYSQLDIR}]
)

if test "$na_mysql" != "no"; then
  [na_parentprefix=`echo "$prefix"|sed -e 's%\(.\)/[^/]*/*$%\1%'`]

  na_searchpath=${na_mysql}/bin:${PATH}:/opt/mysql/bin:$prefix/bin:$na_parentprefix/mysql/bin:/usr/bin
  AC_PATH_PROG(na_mysql,mysql_config,[not found],$na_searchpath)

  if test "$na_mysql" = "not found" ; then
    if test "$na_required" = "required" ; then
      echo
      AC_MSG_ERROR([mysql_config not found in $na_searchpath
Run configure with --with-mysql to specify the Mysql
installation directory
])
    else
        cat <<EOF
******************************************************************
WARNING: mysql_config was not found on your system. As a result,
programs that are needed by the web interface, meta_server and
main_server, will NOT be build.

In most case, you don't need them. Therefore, you can safely
ignore this message.
******************************************************************
EOF
     sleep 2
   fi
  fi
fi

AM_CONDITIONAL(HAVE_MYSQL, test -x "$na_mysql")

if test -x "$na_mysql" ; then
MYSQL_CFLAGS=`$na_mysql --cflags`
MYSQL_LIBS=`$na_mysql --libs`
fi

AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(MYSQL_LIBS)
])

