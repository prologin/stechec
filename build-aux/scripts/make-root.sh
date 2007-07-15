#/bin/bash

if [ $# -lt 1 ]; then
    echo "usage: $0 <distrib>"
    echo "distrib must exists as debootstrap script"
fi

debootstrap $1 $1

cat > $1/tmp/upgrade.sh <<EOF
#! /bin/bash
apt-get install make gcc g++ ruby libxerces27-dev libsdl1.2-dev  libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev debhelper bzip2 autotools-dev fakeroot
EOF
chmod +x $1/tmp/upgrade.sh
chroot $1 /tmp/upgrade.sh
