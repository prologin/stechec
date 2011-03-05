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

#
# Usage, version, ...
#
if [[ $# -lt 1 || "$1" == "--help" ]]; then
    cat <<EOF
\`run' is part of \`stechec'. It is designed as a top level interface
for \`client' and \`server', allowing to play a game in one command.

Usage: run <config-file>

Examples:
  run toto.ini       to run a match, using configuration file 'toto.ini'

Report bugs to <serveur@prologin.org>
EOF
    exit 0
elif [ "$1" == "--version" ]; then
    cat <<EOF
run 3.1

Copyright (C) 2005, 2006, 2007 Prologin.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Written by Olivier Gournet.
EOF
    exit 0
fi

abort()
{
    echo $1 1>&2
    exit 1
}

#
# test if sed is the GNU sed
#
if (sed --version | grep GNU) >/dev/null 2>&1; then
	sed=sed
elif (gsed --version | grep GNU) >/dev/null 2>&1; then
	sed=gsed
else
	echo 1>&2 "You need the GNU sed to use $0."
	exit 1
fi

#
# ini file function
#

# $1: client conf
# $2: key
ini_get_bool()
{
    case $1 in
	*$2~=~true* | *$2~=~on* | *$2~=~1* ) echo "1" ;;
    esac
}

# $1: key
# $2: client conf
ini_get_val()
{
    IFS=""
    echo "$2" | $sed -ns "s/^$1=~=//p"
    unset IFS
}

#
# parse all client sections
#
rules=
clients=$($sed -nr -s 's/^\[client_([0-9])+\]/\1/p' "$1" 2> /dev/null)
test $? -ne 0 && abort "cannot parse configuration file: '$1'"
if [ "x$clients" = x ]; then
    abort "must have at least one client to launch"
fi

for id in $clients; do
    client_conf[$id]=$(IFS="" $sed -nr -s "/^\[client_$id\]/ { :n n; /^\[/ { q }; s/^([a-z_0-9]+)=/\1~=~/p; b n }" "$1")
    r=$(ini_get_val rules "${client_conf[$id]}")
    if [ "$rules" -a x"$r" != x"$rules" ]; then
	abort "clients have different rules ($rules != $r, offending id: $id)"
    fi
    rules=$r

    [ $USE_GDB ] && abort "USE_GDB can only be set for the last champion in config file."
    USE_GDB=$(ini_get_bool gdb ${client_conf[$id]})
done

#
# parse rules section
#
rules_conf=$(IFS="" $sed -nr -s "/^\[$rules\]/ { :n n; /^\[/ { q }; s/^([a-z_0-9]+)=/\1~=~/p; b n }" "$1")
NB_INSTANCE=$(ini_get_val nb_player_per_team "$rules_conf")
if [ "x$NB_INSTANCE" = x ]; then
    NB_INSTANCE=1
fi


CLIENT_BIN=stechec
SERVER_BIN=stechec_server

if [ ! `which $CLIENT_BIN` ]; then
    abort "$CLIENT_BIN: command not found. Check your \$PATH."
elif [ ! `which $SERVER_BIN` ]; then
    abort "$SERVER_BIN: command not found. Check your \$PATH."
fi

#
# Launch server
#
$SERVER_BIN -c "$1" &
pid_server=$!

#
# spawn clients
#
for id in $clients; do
    LOOP=1
    if [ x"$(ini_get_bool spectator \"${client_conf[$id]}\")" != x"1" ]; then
	LOOP=$NB_INSTANCE
    fi

    i=1
    while [ $i -le $LOOP ] ; do
	echo "======== launch client id $id ($i)"  1>&2
	if [ x"$(ini_get_bool valgrind \"${client_conf[$id]}\")" == x"1" ]; then
	    VALGRIND=valgrind
	fi
	STDIN_REDIR=$(ini_get_val stdin_redir "${client_conf[$id]}")
	STDOUT_REDIR=$(ini_get_val stdout_redir "${client_conf[$id]}")

	if [ "x$USE_GDB" = "x$id" ]; then
	    gdb -q --args $CLIENT_BIN -i $id --config="$1"
	else
	    if [[ "x$STDIN_REDIR" != "x" && "x$STDOUT_REDIR" != "x"  ]]; then
		$VALGRIND $CLIENT_BIN -i $id --config="$1" < $STDIN_REDIR > $STDOUT_REDIR &
	    else
		$VALGRIND $CLIENT_BIN -i $id --config="$1" &
	    fi
	    pid_client="$pid_client $!"
	fi

	unset VALGRIND STDIN_REDIR STDOUT_REDIR
	i=$((i + 1))
    done
done

# if the user wants to abort on 'wait', try to kill the server
# to avoid future port binding error
kill_server()
{
    [ "x`ps | grep $pid_server`" != "x" ] && kill $pid_server
    failure=64
}
trap "kill_server" INT

# wait that all process exit.
# FIXME: this is completely buggy !
failure=0
for i in $pid_client; do
    wait $i
    [ $? -ne 0 ] && failure=$((failure + 1))
done

wait $pid_server

echo
echo "[0m*** run: end ***" 1>&2
#sleep 1

exit $failure
