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
  run toto.xml       to run a match, using configuration file 'toto.xml'

Report bugs to <serveur@prologin.org>
EOF
    exit 0
elif [ "$1" == "--version" ]; then
    cat <<EOF
run 3.1

Copyright (C) 2005, 2006 Prologin.
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
# parse xml file, with our shiny awk parser
#
output=`awk -f "${xml_parser_path}xmlparser.awk" "$1"`
awk -f "${xml_parser_path}xmlparser.awk" "$1" > awk
[ $? -ne 0 ] && abort "Cannot parse $1."
# then go back to ugly sed scripts
clients=`echo "$output" | sed -nr 's!^CONFIG/CLIENT/id=([0-9]+).*$!\1!p' | grep -v 0 | uniq`
USE_VALGRIND=`echo "$output" | sed -nre ':n2 /^CONFIG\/CLIENT\/id=([0-9]+)$/ { h; :n n; /^CONFIG\/CLIENT\/DEBUG\/valgrind=true$/ { g; s/.*([0-9]+)$/x\1x/p; n; b n2; }; b n; }'`
USE_GDB=`echo "$output" | sed -nre ':n2 /^CONFIG\/CLIENT\/id=([0-9]+)$/ { h; :n n; /^CONFIG\/CLIENT\/DEBUG\/gdb=true$/ { g; s/.*([0-9]+)$/x\1x/p; n; b n2; }; b n; }'`

if [ ! `which tbt` ]; then
    abort "tbt: command not found. Check your \$PATH."
elif [ ! `which tbt_server` ]; then
    abort "tbt_server: command not found. Check your \$PATH."
fi

if [ "x$clients" = x ]; then
    abort "must have at least one client to launch."
fi

#
# check that gdb is used _only_ on the last client.
#
for id in $clients; do
    # gdb must be the last program to launch, because it starts on foreground
    [ $HAVE_GDB ] && abort "USE_GDB can only be set for the last champion in config file."
    [ "x$USE_GDB" = "x$id" ] && HAVE_GDB=1
done

#
# Launch server
#
tbt_server "$1" &
pid_server=$!

#
# spawn clients
#
for id in $clients; do
    echo "======== launch client id $id"  1>&2
    case $USE_VALGRIND in
	*x${id}x*) VALGRIND=valgrind ;;
    esac
    STDIN_REDIR=`echo "$output" | sed -nre "/^CONFIG\/CLIENT\/id=${id}$/ { :n n; s,CONFIG/CLIENT/REDIRECTION/stdin=(.*)$,\1,p; t q; b n; :q q }"`
    STDOUT_REDIR=`echo "$output" | sed -nre "/^CONFIG\/CLIENT\/id=${id}$/ { :n n; s,CONFIG/CLIENT/REDIRECTION/stdout=(.*)$,\1,p; t q; b n; :q q }"`
    if [ "x$USE_GDB" = "x$id" ]; then
	gdb -q --args tbt $id "$1"
    else
	if [[ "x$STDIN_REDIR" != "x" &&"x$STDOUT_REDIR" != "x"  ]]; then
	    $VALGRIND tbt $id "$1" < $STDIN_REDIR > $STDOUT_REDIR &
	    sleep 1 # BB5 Kludge. Be sure that clients have their uid.
	else
	    $VALGRIND tbt $id "$1" &
	fi
	pid_client="$pid_client $!"
    fi

    unset VALGRIND STDIN_REDIR
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
