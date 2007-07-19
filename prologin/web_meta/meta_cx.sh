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
# This script loads configuration from conf.yml, for 
# these scripts (used by the meta-server):
#   _ compile_champion.sh
#   _ check_cheating.sh
#   _ run_client.sh
#   _ run_server.sh
#
# This file must be in same directory than other scripts
#

config_meta=$1

TMPFILE=`mktemp /tmp/run.XXXXXX` || exit 1
sed -nr -e '
  s/^([a-zA-Z0-9_]+): *$/}\n\1\(\) {/p;
  s/^  ?([a-zA-Z0-9_]+): ([a-zA-Z0-9_/\.]+)/  \1="\2"/p' \
  "$config_meta" | sed 1d > $TMPFILE
echo "}" >> $TMPFILE
source $TMPFILE
[ $? -ne 0 ] && exit 1
rm -f $TMPFILE

# load connection section
connection


# Get a file from the host.
# A temporary directory will be created if needed.
#  $1 -> file to download from
#  $2 -> directory to download to
# return: path to the dowloaded file.
dl_file() {
    case $method in
        ssh)
            scp $host:"$1" "$2"
            echo "$2/`basename $1`"
            ;;
        nfs)
            # nothing to do. already on the filesystem.
            echo "$1"
            ;;
    esac
}

# Upload a file to the host
#  $1 -> file to send
#  $2 -> directory destination
upload_file() {
    test -r $1 && case $method in
        ssh)
            scp $host:"$1" "$2"
            ;;
        nfs)
            cp $1 $2
            if [ -d $2 ]; then
                chmod o+r "$2/`basename $1`"
            else
                chmod o+r $2
            fi
            ;;
    esac
}

# Abort execution of this script on fatal error.
#   $1 -> error message
#   $2 -> exit code
#   $3 -> optional log file, write error message into it,
#         then try to upload it to $4 before exiting.
#   $4 -> log file dst.
abort() {
    [ $# -ge 4 ] && have_log=1
    if [ $have_log ]; then
        echo "$1" >> $3
        upload_file $3 $4
    else
        echo "$1"
    fi
    [ $2 ] && exit $2 || exit 42
}