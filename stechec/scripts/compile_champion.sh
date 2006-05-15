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
#  Copyright (C) 2005 Prologin
#

if [ "$#" -ne 3 ]; then
    cat <<EOF
Usage: $0 contest_dir_name candidate_id champion_id 
EOF
    exit 11
fi

contest_name=$1
candidate_id=$2
champion_id=$3

# Retreive configuration: $CONTEST_PATH and $SERVER_ACCESS (only if no NFS system)  
configuration_file="`dirname $0`/conf.sh"
if [ ! -f $configuration_file ]; then
    echo "Error: can't find configuration file in: $configuration_file"
    exit 2
fi
source $configuration_file

champion_tarball=$CONTEST_PATH/$contest_name/private/candidate_$candidate_id/champion_$champion_id.tgz
champion_final_lib=$CONTEST_PATH/$contest_name/private/candidate_$candidate_id/champion_$champion_id.so
champion_final_log=$CONTEST_PATH/$contest_name/private/candidate_$candidate_id/champion_$champion_id.log
champion_final_codeg=$CONTEST_PATH/$contest_name/private/candidate_$candidate_id/champion_$champion_id.code

tmp_rep=`mktemp /tmp/stechec_compil_XXXXXX`

res=0

if [ -z $SERVER_ACCESS ]; then 
    real_tarball=$champion_tarball
    real_log=$champion_final_log
    real_code=$champion_final_code
else
    real_tarball=`mktemp -q /tmp/tarball_XXXXXX`
    real_log=`mktemp -q /tmp/log_XXXXXX`
    real_code=`mktemp -q /tmp/log_XXXXXX`
    scp $SERVER_ACCESS:$champion_tarball $real_tarball
fi



echo -n "* Locate tarball `basename $real_tarball`: " >> $real_log
[ ! -f $real_tarball ] && { echo FAILED; exit 3; }; echo OK >> $real_log; echo >> $real_log

touch $real_log
chmod 777  $real_log
nice 5

(
    rm -rf $tmp_rep
    mkdir $tmp_rep
    cd $tmp_rep
    cp $real_tarball .

    echo "* Extract data" >> $real_log
    echo "------8<----------------------" >> $real_log
    tar xvzf `basename $real_tarball`  >> $real_log 2>&1
    res=$?
    echo "------>8----------------------" >> $real_log
    echo >> $real_log
    [ $res -ne 0 ] && { echo "* Extraction: FAILED" >> $real_log; exit 3; }

    echo "* Remove old \"champion.so\"" >> $real_log; echo >> $real_log
    rm -rf champion.so

    # FIXME: path, toussa ...
    MAKEFILE_PATH=/home/victor/www/private/makefile
    cp $MAKEFILE_PATH/forb* .

    MAKEFILE=Makefile-c
    [ "`echo *.cc`" != "*.cc" ] && MAKEFILE=Makefile-cxx
    [ "`echo *.cpp`" != "*.cpp" ] && MAKEFILE=Makefile-cxx
    [ "`echo *.pas`" != "*.pas" ] && MAKEFILE=Makefile-pascal
    [ "`echo *.java`" != "*.java" ] && MAKEFILE=Makefile-java
    [ "`echo *.ml`" != "*.ml" ] && MAKEFILE=Makefile-caml

    lang=`echo $MAKEFILE | sed 's/^Makefile-//'` 
    echo "* Compile champion, language \"$lang\" detected." >> $real_log
    echo "------8<----------------------" >> $real_log
    make -f $MAKEFILE_PATH/$MAKEFILE distclean all >> $real_log 2>&1
    res=$?
    echo "------>8----------------------" >> $real_log
    echo >> $real_log
    [ $res -ne 0 ] && { echo "* Compilation: FAILED" >> $real_log; exit 3; }

    echo -n "* Locate champion.so: " >> $real_log
    [ ! -f champion.so ] && { echo "FAILED" >> $real_log; exit 4; }
    echo OK >> $real_log; echo >> $real_log
    
    if [ -z $SERVER_ACCESS ]; then 
        cp champion.so   $champion_final_lib
	[ -f champion.code ] && cp champion.code $real_code
        [ ! -f $champion_final_lib ] && exit 5
    else
        scp champion.so $SERVER_ACCES:$champion_final_lib
        scp $real_log   $SERVER_ACCES:$champion_final_log
        scp $real_code  $SERVER_ACCES:$champion_final_code
    fi
    echo -n ""
) 

res=$?

cd /tmp
rm -rf $tmp_rep

exit $res

