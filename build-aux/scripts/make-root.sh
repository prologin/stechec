#/bin/bash

if [ $# -lt 1 ]; then
    echo "usage: $0 <distrib>"
    echo "distrib must exists as debootstrap script"
fi

debootstrap $1 $1

echo '#! /bin/bash' > $1/tmp/upgrade.sh

# ubuntu must include universe
if [ $1 == "dapper" || $1 == "edgy" || $1 == "feisty" ]; then
    sed -ri 's/^(deb.*)/\1 universe/' $1/etc/apt/sources.list
fi

cat > $1/tmp/upgrade.sh <<EOF
apt-get install make gcc g++ ruby libxerces27-dev libsdl1.2-dev  libsdl-image1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev debhelper bzip2 autotools-dev fakeroot
EOF

# feisty needs some more deps
if [ $1 = "feisty" ]; then
    echo 'apt-get install libasound2-dev libartsc0-dev libarts1-dev' >> $1/tmp/upgrade.sh
fi

chmod +x $1/tmp/upgrade.sh
chroot $1 /tmp/upgrade.sh
