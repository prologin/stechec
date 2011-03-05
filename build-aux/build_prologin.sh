#! /bin/bash

#
# This script is only intended for power-user :)
# If you don't understand what is it, you don't need it.
#
# Remove bb5 specifity from configure.ac and Makefile.am,
# to build clean tarball with prologin only stuff.
#
# To be launch once before bootstrap, never commit modifications.
# Launch it in stechec's root directory
#

test -f configure.ac || exit 1

sed -ir '/^SUBDIRS.*+= doc/ { d }' Makefile.am
sed -ir 's/README.stechec //' Makefile.am
sed -ir '/BUILD_bb5/ { d }' Makefile.am

if [ -f README.stechec ]; then
    rm README
    mv README.stechec README
fi

sed -ir '/doc\/Makefile/ { d }; /doc\/doc.doxy/ { d }' configure.ac
sed -ir 's/modules="bb5"/modules="ant,prolo2005,prolo2006,prolo2007"/' configure.ac
sed -ir 's!http://projects.nekeme.net/projects/tbt/newticket!info@prologin.org!' configure.ac
sed -i '/^STECHEC_ADDMODULE(\[bb5\])/ { d }' configure.ac
sed -ir '/^DEPENDS_bb5/ { d }' configure.ac
sed -nir '/^if test x\$BUILD_bb5/ { :nn /^fi/ { d }; n; b nn }; p' configure.ac
sed -ir '/Building with paragui/ { d }; /Building with readline/ { d }' configure.ac

# ???
rm -f configure.acr Makefile.amr
