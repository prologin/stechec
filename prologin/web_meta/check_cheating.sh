#! /bin/sh
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
#  Copyright (C) 2005, 2006, 2007 Prologin
#

if [ "$#" -ne 4 ]; then
    cat <<EOF
Usage: $0 <config_meta> <contest_name> <candidat_id> <champion_id> 
EOF
    exit 11
fi

config_meta=$1
contest_name=$2
candidat_id=$3
champion_id=$4

# load config and transfert methods.
source "`dirname $0`/meta_cx.sh" "$config_meta"
[ $? -ne 0 ] && echo "Error: can't find configuration file in: `dirname $0`/meta_cx.sh" && exit 12

champion_path=$contest_path/$contest_name/private/candidat_$candidat_id/
champion_lib=$champion_path/champion_$champion_id.so
champion_log=$champion_path/champion_$champion_id.log

tmp_rep=`mktemp -q -d /tmp/check_triche_XXXXXX`

#
# Download the compilation log
#
real_log=`dl_file $champion_log $tmp_rep`
if [ ! -f $real_log ]; then
    abort "Go beserk. Can't get the logfile." 4
fi

{
    #
    # Download the champion
    #
    real_lib=`dl_file $champion_lib`
    if [ ! -f $real_lib ]; then
        abort "Can't locate champion: `basename $champion_lib`" 3 $real_log $champion_log
    fi

    #
    # Real code to detect cheating
    #
    echo "* Check cheating"
    echo "------8<----------------------"

    res=0
    forbid=`nm $real_lib | grep "__wrap_"`
    if [ $? -eq 0 ]; then
	echo "$forbid" | while read foo func; do
	    echo "$func" | sed -e 's/__wrap_//'
	done
	res=1
    fi

    echo "------>8----------------------"
    echo

    if [ $res -eq 0 ]; then
        echo "* Good, no cheating."
    else
        echo "* It seems you try to use forbiden function !!! It's bad."
        echo "* If you think these functions are legitimate, fell free to address your complain"
        echo "  in the forum."
        echo
    fi

} >> $real_log

upload_file $real_log $champion_log

rm -rf $tmp_rep
exit $res
