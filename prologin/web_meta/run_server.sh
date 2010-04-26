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
#  Copyright (C) 2005, 2006, 2007 Prologin
#


#
# /!\ Don't work without NFS.
#


if [ "$#" -le 8 ]; then
    cat <<EOF
Usage: $0 <config_meta> <contest_lib_name> <contest_dir_name> <is_competition>
          <game_id> <nb_champion_instance> <port> <nb_player> -- extra_args
EOF
    exit 1
fi

cd /tmp

# is_competition: 0 -> log output; 1 -> don't log (don't crash nfs)
config_meta=$1
contest_lib_name=$2
contest_dir_name=$3
is_competition=$4
game_id=$5
nb_champion_instance=$6
port=$7
nb_player=$8
# '--' -> $9

shift 10

# open temporary files for logs.
tmp_dir=`mktemp -q -d /tmp/match_XXXXXX`
real_log_file=$tmp_dir/visio.log
real_out_file=$tmp_dir/server.out

# parse extra arguments, to fill our ini.
map="ERROR_not_set"
max_turn=10
nb_team=$((nb_player / nb_champion_instance))
while [ $# -gt 1 ]; do
    case $1 in
        -map) map=$2 ;;
        -turn) max_turn=$2 ;;
        *) echo "Warning: Unknown server argument: '$1' => '$2'" > $real_out_file
    esac
    shift 2
done


# load config and transfert methods.
source "`dirname $0`/meta_cx.sh" "$config_meta"
[ $? -ne 0 ] && echo "Error: can't find configuration file in: `dirname $0`/meta_cx.sh" && exit 12

log_file=$contest_path/$contest_dir_name/viewer/match_$game_id.json
out_file=$contest_path/$contest_dir_name/matchs/match_$game_id/server.out


#
# Create server config file.
#
config_file=$tmp_dir/config.ini
cat > $config_file <<EOF
[client_3]
rules=$contest_lib_name
path=/opt/stechec/lib
library=${contest_lib_name}_jsview
verbose=0
spectator=1
server_port=$port
game_uid=$game_id

[server]
listen_port=$port
nb_spectator=1
verbose=2

[$contest_lib_name]
verbose=0
EOF

#
# Real code to run a server
#

if [ $is_competition = "0" ]; then
    #
    # Run a game, format output
    #
    tmp_out=$tmp_dir/stdout
    echo " * Start at: `date +%T`" >> $real_out_file
    echo >> $real_out_file
    ulimit -c 10000
    (sleep 2; $stechec_install_path/bin/stechec --config=$config_file -i 3 >$real_log_file 2>>$real_out_file) &
    pid=$!
    $stechec_install_path/bin/stechec_server --config=$config_file > $tmp_out 2>> $real_out_file
    res=$?
    sleep 2;
    [ -d "/proc/$pid" ] && kill $pid
    sed -i -e 's/\[[01];3[0-9]m//g;s/\[0m//g' $real_out_file
    echo >> $real_out_file
    echo " * End at: `date +%T`" >> $real_out_file
    echo " * Server exited with return code: $res" >> $real_out_file
    if [ -s $tmp_out ]; then
        echo >> $real_out_file
    	echo "Dumping standard output:" >> $real_out_file
    	cat $tmp_out >> $real_out_file
        # meta_server need it on stdout for match result.
    	cat $tmp_out
    fi

    # Now upload log and visio files.
    # FIXME: make it no-NFS aware.
    mkdir -p $contest_path/$contest_dir_name/matchs/match_$game_id
    chmod 777 $contest_path/$contest_dir_name/matchs/match_$game_id

    upload_file $real_out_file $out_file
    upload_file $real_log_file $log_file

    # If not yet done, sorry, but I must kill you.
    kill -9 $pid

else
    #
    # Run a game, trash all debug output and game log.
    #
    $stechec_install_path/bin/stechec_server --config=$config_file 2> /dev/null
    res=$?
fi

rm -rf $tmp_dir
exit $res
