#! /bin/sh

name=$1
newname=$2
if [ "${newname}" = "" ] ; then echo "Usage: $0 SOURCE_TEST_NAME DEST_TEST_NAME" ; exit 1 ; fi
for ext in _{1,2}.{cmd,ref}
do
	if [ ! -f "${name}${ext}" ] ; then echo "Boing! ${name} doesn't exist." ; exit 1 ; fi
	if [ -f "${newname}${ext}" ] ; then echo "Boing! ${newname} already exists." ; exit 1 ; fi
	svn copy "${name}${ext}" "${newname}${ext}"
done
gvim ${newname}_{1,2}.{cmd,ref} Makefile.am test_pool.yml
echo "Note: Use ' svn revert ${newname}_{1,2}.{cmd,ref} ' if you wish to cancel this test copy."
