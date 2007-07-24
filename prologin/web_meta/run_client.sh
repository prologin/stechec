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

#
# /!\ With no NFS don't work.
#

if [ "$#" -ne 11 ]; then
    cat <<EOF
Usage: $0 <config_meta> <contest_lib_name> <contest_dir_name> <is_competition>
          <candidat_id> <champion_id> <competiteur_id> <team_id> <game_id>
          <port> <ip_server>
EOF
    exit 11
fi

cd /tmp

# is_competition: 0 -> log output; 1 -> don't log (don't crash nfs)
config_meta=$1
contest_lib_name=$2
contest_dir_name=$3
is_competition=$4
candidat_id=$5
champion_id=$6
competiteur_id=$7
team_id=$8
game_id=$9
shift
port=$9
shift
ip_server=$9

time_limit=200
reserve_time=2000
memory_limit=20000
log_limit=128

# load config and transfert methods.
source "`dirname $0`/meta_cx.sh" "$config_meta"
[ $? -ne 0 ] && echo "Error: can't find configuration file in: `dirname $0`/meta_cx.sh" && exit 12

champion_path="$contest_path/$contest_dir_name/private/candidat_$candidat_id/champion_$champion_id.so"
out_file="$contest_path/$contest_dir_name/matchs/match_$game_id/client_$competiteur_id.out"

# FIXME: and with ssh ?
mkdir -p $contest_path/$contest_dir_name/matchs/match_$game_id

#
# Open log file, if requested
#
tmp_dir=`mktemp -q -d /tmp/match_XXXXXX`
real_out_file="$tmp_dir/log_match"

#
# Retrieve champion.so
#
real_lib=`dl_file $champion_path`
echo -n "* Locate champion.so: " > $real_out_file
if [ ! -f $real_lib ]; then
    abort "FAILED" 3 $real_out_file $out_file
fi
echo "OK" >> $real_out_file
champion_library=`echo $real_lib | sed s/\.so$//`

#
# Hack for java !
#
nm $real_lib | grep -q "_Jv_AttachCurrentThread"
[ "$?" -eq 0 ] && memory_limit=70000

#
# Create client configuration file.
#
config_file="$tmp_dir/config.xml"
cat > $config_file <<EOF
<?xml version="1.0" encoding="iso-8859-1" ?>
<!DOCTYPE config SYSTEM "file://${stechec_install_path}/share/stechec/config.dtd">
<config>

  <client id="${team_id}">
    <rules>$contest_lib_name</rules>
    <champion>$champion_library</champion>
    <connect val="network" host="$ip_server" port="$port" game_uid="$game_id" />
    <mode replay="false" spectator="false" />
    <limit memory="$memory_limit" time="$time_limit" time_reserve="$reserve_time" />
    <debug verbose="2" />
  </client>

</config>
EOF


#
# Real code to run a client
#
if [ $is_competition = "0" ]; then
    echo " * Memory limit: $memory_limit" >> $real_out_file
    echo " * Time limit: $time_limit (reserve $reserve_time)" >> $real_out_file
    echo " * Start at: `date +%T`" >> $real_out_file
    echo >> $real_out_file

    $stechec_install_path/bin/stechec $team_id $config_file 2>&1        \
        | $stechec_install_path/bin/log_limit $log_limit                \
        | sed -e 's/\[[01];3[0-9]m//g;s/\[0m//g' >> $real_out_file

    # get error return for our command
    # yes... it's bash... it could be better with zsh :)
    res="${PIPESTATUS[0]} ${PIPESTATUS[1]}"
    truncated=$(echo $res | cut -f2 -d' ')
    res=$(echo $res | cut -f1 -d' ')

    if [ $truncated -eq 1 ]; then
        echo "[...]"
        echo " * Log was truncated, it is limited to ${log_limit}ko."
    fi

    echo >> $real_out_file
    echo " * Client exited with return code: $res" >> $real_out_file
    echo " * End at: `date +%T`" >> $real_out_file

    # eventually upload log file.
    upload_file $real_out_file $out_file
else
    $stechec_install_path/bin/stechec $team_id $config_file                     \
       > /dev/null 2> /dev/null 
    res=$?
fi

rm -rf $tmp_dir

exit $res
