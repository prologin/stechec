#! /bin/sh -e

#
# Helper to build packages
#

if [ $# -ne 1 ]; then
	echo "usage: $0 <tar-file>"
	echo "purpose: will build debian packages from this tarball (.tar.gz or .tar.bz2)"
	exit 1
fi

DEBIANDIST=$PWD/deb-build

TARFILE=$1
TARDIR=`echo $TARFILE | sed -r 's/\.tar\.(bz2|gz)//'`
if [ "$TARFILE" = "$TARDIR" ]; then
	echo "$TARFILE must be .tar.gz or .tar.bz2"
	exit 1
fi
mkdir -p $DEBIANDIST/binary $DEBIANDIST/source
tar xjf $TARFILE -C $DEBIANDIST || tar xzf $TARFILE -C $DEBIANDIST

cd $DEBIANDIST/$TARDIR
dpkg-buildpackage -uc -us -rfakeroot

cd $DEBIANDIST
mv *.deb binary/
mv *.tar.gz *.dsc source/
dpkg-scanpackages binary /dev/null | gzip -9c > binary/Package.gz
dpkg-scanpackages source /dev/null | gzip -9c > source/Package.gz

