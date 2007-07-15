#! /bin/sh -e

#
# Helper to build packages for all distribs/arch.
#

if [ $# -lt 2 ]; then
	echo "usage: $0 <tarball> <distrib1> <distrib2> <...>"
	echo "purpose: will chroot in each distrib/directory, build and gather packages."
	exit 1
fi

if [ "`id -u`" != 0 ]; then
	echo "You must be root to do chroot."
	exit 1;
fi

tarball=$1
shift
num_job=0

while [ $# -gt 0 ]; do
	distrib=$1
	shift
	script=unknown
	case $distrib in
	  *breezy*|*dapper*|*sarge*|*sid*|*etch*|*lenny*|*feisty*|*edgy*) script=build-deb.sh;;
	  *fedora4*) script=build-rpm.sh;;
	  *) echo "don't know how to build this distribution: $distrib"; continue;;
	esac

	num_job=$((num_job + 1))
	echo "Launch $distrib (with $script, $num_job running)."
	cp $script $distrib/tmp
	cp $tarball $distrib/tmp
	( 
	  chroot $distrib bash -c "cd /tmp; ./$script --base=/tmp --distrib=$distrib --arch=i386 `basename $tarball`"
	  cp -r $distrib/tmp/dists . || cp -r $distrib/tmp/rpm .
	) > log-$distrib.log 2>&1 &
	sleep 1
done
wait
