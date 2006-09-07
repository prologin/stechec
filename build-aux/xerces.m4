# -*- shell-script -*-
#
# Based on vaucanson xml.m4 macro
#

AC_DEFUN([TBT_CHECK_XML], 
[dnl

  AC_CACHE_CHECK([for Xerces-C++ in user-specified directory],
    [tbt_cv_xml_user_hint],
    [tbt_cv_xml_user_hint=no
     AC_ARG_WITH([xerces],
       [AC_HELP_STRING([--with-xerces=DIR],
         [base directory where Xerces-C++ is installed])],
       [if test -r "$withval/include/xercesc"; then
          tbt_cv_xml_user_hint=$withval
        fi])
    ])

  HAVE_XERCES=no

   # User-specified directory overrides any other definition
   if test "x$tbt_cv_xml_user_hint" != xno; then
      XERCES_EXTRA_CPPFLAGS="-I$tbt_cv_xml_user_hint/include"
      XERCES_EXTRA_LDFLAGS="-L$tbt_cv_xml_user_hint/lib"
   else
      XERCES_EXTRA_CPPFLAGS=''
      XERCES_EXTRA_LDFLAGS=''
   fi


   # Check for xerces presence.
   AC_LANG_PUSH([C++])

   AC_CACHE_CHECK([for previously found Xerces-C++ headers >= 2.3],
     [tbt_cv_xerces],
     [tbt_cv_xerces=unknown])
   if test x$tbt_cv_xerces = xunknown; then
        tbt_cv_xerces=no
        tbt_save_CPPFLAGS=$CPPFLAGS
        CPPFLAGS="$CPPFLAGS $XERCES_EXTRA_CPPFLAGS"
        AC_CHECK_HEADER([xercesc/util/XercesVersion.hpp],
           [AC_COMPILE_IFELSE([AC_LANG_SOURCE([
@%:@include <xercesc/util/XercesVersion.hpp>
@%:@if XERCES_VERSION_MAJOR != 2
@%:@ error "Bad Xerces-C++ major version."
@%:@endif
@%:@if XERCES_VERSION_MINOR < 3
@%:@ error "Bad Xerces-C++ minor version."
@%:@endif
           ])], 
          [tbt_cv_xerces=yes])])
      CPPFLAGS=$tbt_save_CPPFLAGS
   fi
   if test x$tbt_cv_xerces != xyes; then
      AC_ERROR([
****** Xerces-C++ headers not found ! ********
If it already installed, but not at a standard place, use
--with-xerces=PATH configure flag.
Otherwise, download and install it:
http://xml.apache.org/xerces-c/download.cgi
(debian user: libxerces27-dev package should do the trick).
**********************************************])
   fi


   AC_CACHE_CHECK([for useable Xerces-C++ library],
                  [tbt_cv_xerces_lib],
                  [tbt_cv_xerces_lib=no
                   tbt_save_CPPFLAGS=$CPPFLAGS
                   tbt_save_LDFLAGS=$LDFLAGS
                   tbt_save_LIBS=$LIBS
                   CPPFLAGS="$CPPFLAGS $XERCES_EXTRA_CPPFLAGS"
                   LDFLAGS="$LDFLAGS $XERCES_EXTRA_LDFLAGS"
                   LIBS="$LIBS -lxerces-c"
                   AC_LINK_IFELSE([AC_LANG_SOURCE([
@%:@include <xercesc/util/XMLUniDefs.hpp>
@%:@include <xercesc/util/XMLString.hpp>

int main() {
  const char *foo = "foo";
  using namespace xercesc;
  XMLCh* bar = XMLString::transcode(foo);
}
                      ])],
                      [tbt_cv_xerces_lib=yes])
                    LIBS=$tbt_save_LIBS
                    LDFLAGS=$tbt_save_LDFLAGS
                    CPPFLAGS=$tbt_save_CPPFLAGS
                    ])

   if test x$tbt_cv_xerces_lib != xyes; then
      AC_ERROR([
****** Cannot link with Xerces Library ! ********
It should not happen, since headers were already found.
Fix your configuration, gather some help from
http://xml.apache.org/xerces-c/
Or report this as a bug.
************************************************])
   fi

   AC_SUBST([XERCES_CPPFLAGS], ["$XERCES_EXTRA_CPPFLAGS"])
   AC_SUBST([XERCES_LDFLAGS], ["$XERCES_EXTRA_LDFLAGS -lxerces-c"])
   HAVE_XERCES=yes

   AC_LANG_POP([C++])
])
