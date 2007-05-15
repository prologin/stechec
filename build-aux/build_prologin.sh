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

sed -i -r 's/^SUBDIRS.*+= doc.*//' Makefile.am
sed -i -r 's/README.stechec//' Makefile.am
rm README
mv README.stechec README

sed -i -r 's/modules="bb5"/modules="ant,prolo2005,prolo2006,prolo2007"/' configure.ac
sed -i -r '/doc\/Makefile/ { d }; /doc\/doc.doxy/ { d }' configure.ac
sed -i -r 's/.*Building with readline:.*//' configure.ac
sed -i -r 's/.*Building with paragui:.*//' configure.ac
sed -i -nre '/if test x.BUILD_bb5 != x; then/ { :n s/^fi$//; t q; n; b n; :q d }; p' configure.ac
