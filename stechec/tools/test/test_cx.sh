#! /bin/bash

./cx server &
server_pid=$!
sleep 1

./cx client
client_pid=$1

wait $server_pid
[ $? -ne 0 ] && exit 1

wait $client_pid
[ $? -ne 0 ] && exit 1

:
