#!/bin/sh
regen(){
 mv tmp/$1 $1
}

generator player prolo2010 tmp

regen c/interface.cc
regen c/interface.hh
regen c/Makefile

regen cxx/Makefile

rm -rf includes
regen includes

regen java/interface.cc
regen java/interface.hh
regen java/Interface.java
regen java/Makefile

regen pascal/interface.cc
regen pascal/interface.hh
regen pascal/prolo_interface.pas
regen pascal/Makefile

regen php/api.php
regen php/interface.cc
regen php/interface.hh
regen php/Makefile

regen python/api.py
regen python/interface.cc
regen python/interface.hh
regen python/Makefile


#rm -rf tmp
