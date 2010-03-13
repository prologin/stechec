#bin/bash
time opacomp --verbose \
  lib2.opa lib_2players.opa wait_game.opa lib_games.opa server.opa \
  interface.opa types_com.opa prologin.opa

./patch_makefile.sh
./rmdb.sh
