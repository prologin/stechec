#! /bin/bash
#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005, 2006 Prologin
#

if [ "$#" -ne 3 ]; then
    cat <<EOF
Usage: $0 <contest_name> <candidat_id> <champion_id> 
EOF
    exit 11
fi

contest_name=$1
candidat_id=$2
champion_id=$3

# load config and transfert methods.
source "`dirname $0`/meta_cx.sh"
[ $? -ne 0 ] && echo "Error: can't find configuration file in: `dirname $0`/meta_cx.sh" && exit 12

champion_path="$contest_path/$contest_name/private/candidat_$candidat_id/"
makefile_path="$contest_path/makefile"

champion_tarball="$champion_path/champion_$champion_id.tgz"
champion_final_lib="$champion_path/champion_$champion_id.so"
champion_final_log="$champion_path/champion_$champion_id.log"
champion_final_code="$champion_path/champion_$champion_id.code"

res=0

tmp_rep=`mktemp -d /tmp/stechec_compil_XXXXXX`
real_tarball=`dl_file $champion_tarball $tmp_rep`
real_log=$tmp_rep/log
real_code=$tmp_rep/code

echo -n "* Locate tarball `basename $real_tarball`: " > $real_log
if [ ! -f $real_tarball ]; then
    abort "FAILED" 3 $real_log $champion_final_log
fi
echo "OK" >> $real_log
echo >> $real_log

renice 5 $$ > /dev/null

(
    cd $tmp_rep
    cp $real_tarball .

    echo "* Extract data"
    echo "------8<----------------------"
    tar xvzf `basename $real_tarball`
    res=$?
    echo "------>8----------------------"
    echo
    if [ $res -ne 0 ]; then
        abort "* Extraction: FAILED" 4 $real_log $champion_final_log
    fi

    cp $makefile_path/forb* .
    cp $makefile_path/stechec_lime.c .

    # FIXME: cp's are kludge. new Makefile are better.
    makefile=Makefile-c
    [ "`echo *.cc`" != "*.cc" ] && makefile=Makefile-cxx
    [ "`echo *.cpp`" != "*.cpp" ] && makefile=Makefile-cxx
    [ "`echo *.pas`" != "*.pas" ] && makefile=Makefile-pascal &&  cp $contest_path/$contest_name/compil/prolo_interface.pas .
    [ "`echo *.java`" != "*.java" ] && makefile=Makefile-java
    [ "`echo *.ml`" != "*.ml" ] && makefile=Makefile-caml  && cp $contest_path/$contest_name/compil/api.ml .
    [ "`echo *.lua`" != "*.lua" ] && makefile=Makefile-lua

    lang=`echo $makefile | sed 's/^Makefile-//'` 
    echo "* Compile champion, language \"$lang\" detected."
    echo "------8<----------------------"
    make -f $makefile_path/$makefile distclean all
    res=$?
    echo "------>8----------------------"
    echo
    if [ $res -ne 0 ]; then
        abort "* Compilation: FAILED" 5 $real_log $champion_final_log
    fi

    echo -n "* Locate champion.so: "
    if [ ! -f champion.so ]; then
        abort "FAILED" 5 $real_log $champion_final_log
    fi
    echo "OK"
    echo ""

    upload_file champion.code $champion_final_code
    upload_file champion.so $champion_final_lib

) >> $real_log 2>&1

res=$?

# eventually upload log file.
upload_file $real_log $champion_final_log

rm -rf $tmp_rep

exit $res
