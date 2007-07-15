#! /bin/sh -e

#
# Helper to build packages
#

unset LANG

if [ $# -lt 1 ]; then
	echo "usage: $0 [opts] <tarball>"
	echo "purpose: will build several debian packages from a tarball (.tar.gz or .tar.bz2)"
	exit 1
fi

base=unknown
distrib=unknown
arch=unknown
while [ $# -gt 1 ]; do
	cmd=`echo $1 | sed -r 's/^--([^=]+)=.*/\1/'`
	arg=`echo $1 | sed -r 's/^[^=]+=(.*)$/\1/'`
	case $cmd in
	  distrib) distrib="$arg";;
	  arch) arch="$arg";;
	  base) base="$arg";;
	  *) echo "unknown option $cmd = $arg"; exit 1;;
	esac
	shift
done

rm -rf $base/rpm

tarball=$1

RPMBUILD="$base/rpm/$distrib"
mkdir -p "$RPMBUILD/SOURCES"  \
         "$RPMBUILD/BUILD"    \
         "$RPMBUILD/SPECS"    \
         "$RPMBUILD/RPMS/$arch" \
         "$RPMBUILD/SRPMS"

# prepare the build
echo "%_topdir $RPMBUILD" > ${HOME}/.rpmmacros

# build the rpm package
rpmbuild -ta $tarball
