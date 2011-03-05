#!/bin/sh

./bootstrap
./configure --disable-documentation
make install
