time opa --verbose \
  lib.opa lib_2players.opa wait_game.opa lib_games.opa server.opa \
  interface.opa types_com.opa page_help.opa prologin.opa\
&& \
./rmdb.sh
