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

TARFILE=$1
DEBIANDIST=$PWD/deb-build
TARDIR=`echo $TARFILE | sed -r 's/\.tar\.(bz2|gz)//'`

rm -rf $DEBIANDIST
rm -rf $base/dists

if [ "$TARFILE" = "$TARDIR" ]; then
	echo "$TARFILE must be .tar.gz or .tar.bz2"
	exit 1
fi
mkdir -p $DEBIANDIST
tar xjf $TARFILE -C $DEBIANDIST || tar xzf $TARFILE -C $DEBIANDIST

cd $DEBIANDIST/$TARDIR
dpkg-buildpackage -uc -us -rfakeroot

cd $base
binarydir=dists/$distrib/main/binary-$arch
sourcedir=dists/$distrib/main/source
mkdir -p $binarydir $sourcedir
mv $DEBIANDIST/*.deb $binarydir
mv $DEBIANDIST/*.tar.gz $DEBIANDIST/*.dsc $sourcedir
dpkg-scanpackages $binarydir /dev/null | gzip -9c > $binarydir/Packages.gz
dpkg-scanpackages $sourcedir /dev/null | gzip -9c > $sourcedir/Sources.gz

( cd $binarydir/.. && ln -sf binary-$arch/Packages.gz . )
( cd $sourcedir/.. && ln -sf source/Sources.gz . )
