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

sed -ir 's/^SUBDIRS.*+= doc.*//' Makefile.am
rm README
mv README.stechec README

sed -ir 's/modules="bb5"/modules="ant,prolo2005,prolo2006,prolo2007"/' configure.ac
