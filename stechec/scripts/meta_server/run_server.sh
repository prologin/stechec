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
# /!\ Don't work without NFS.
#


if [ "$#" -le 6 ]; then
    cat <<EOF
Usage: $0 <contest_lib_name> <contest_dir_name> <is_competition> <game_id> <port> [players_id ...] \
          -- extra_args
EOF
    exit 1
fi

cd /tmp

# is_competition: 0 -> log output; 1 -> don't log (don't crash nfs)
contest_lib_name=$1
contest_dir_name=$2
is_competition=$3
game_id=$4
port=$5

shift 5

# open temporary files for logs.
tmp_dir=`mktemp -q -d /tmp/match_XXXXXX`
real_log_file=$tmp_dir/visio.log
real_out_file=$tmp_dir/server.out

# count players. compat witch stechec v2.
nb_player=0
while [ "x$1" != "x--" ]; do
    nb_player=$((nb_player + 1))
    shift 1
done

# parse extra arguments, to fill our xml.
shift 1
map="ERROR_not_set"
max_turn=10
while [ $# -gt 1 ]; do
    case $1 in
        -map) map=$2 ;;
        -turn) max_turn=$2 ;;
        *) echo "Warning: Unknown server argument: $1=$2" > $real_out_file
    esac
    shift 2
done


# load config and transfert methods.
source "`dirname $0`/meta_cx.sh"
[ $? -ne 0 ] && echo "Error: can't find configuration file in: `dirname $0`/meta_cx.sh" && exit 12

log_file=$contest_path/$contest_dir_name/matchs/match_$game_id/visio
out_file=$contest_path/$contest_dir_name/matchs/match_$game_id/server.out


#
# Create server config file.
#
config_file=$tmp_dir/config.xml
cat > $config_file <<EOF
<?xml version="1.0" ?>
<config>

  <game>
    <rules>$contest_lib_name</rules>
    <nb_team>$nb_player</nb_team>
    <max_turn>$max_turn</max_turn>
    <map>$map</map>
  </game>

  <server>
    <options persistent="false" start_game_timeout="50" />
    <listen port="$port" />
    <log enabled="true" file="$real_log_file" />
    <debug verbose="3" printloc="false" />
    <server_debug verbose="5" printloc="false" />
    <nb_spectator>0</nb_spectator>
  </server>

</config>
EOF

#
# Real code to run a server
#

# FIXME: hack.
export OUTATIME_SHARED_MAP=/home/goinfre/map

if [ $is_competition = "0" ]; then
    #
    # Run a game, format output
    #
    tmp_out=$tmp_dir/stdout
    $stechec_install_path/bin/tbt_server $config_file > $tmp_out 2> $real_out_file
    res=$?
    sed -i -e 's/\[[01];3[0-9]m//g;s/\[0m//g' $real_out_file
    echo "Serveur exited with return code: $res" >> $real_out_file
    echo >> $real_out_file
    if [ -s $tmp_out ]; then
    	echo "Dumping standart output:" >> $real_out_file
    	cat $tmp_out >> $real_out_file
        # meta_server need it on stdout for match result.
    	cat $tmp_out
    fi

    # Now upload log and visio files.
    # FIXME: make it no-NFS aware.
    mkdir -p $contest_path/$contest_dir_name/matchs/match_$game_id
    upload_file $real_out_file $out_file
    upload_file $real_log_file $log_file

else
    #
    # Run a game, trash all debug output and game log.
    #
    $stechec_install_path/bin/tbt_server $config_file 2> /dev/null
    res=$?
fi

rm -rf $tmp_dir
exit $res
