# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

require 'constants.rb'

puts "load from ruby !"

def init_game()
  puts "init_game from ruby"
  # fonction a completer
end

def retirer_ko()
  # fonction a completer
end

def jouer()
  us = unites();
  u = us[0]
  puts "nombre_unites moi : #{nombre_unites(false)}"
  puts "nombre_unites lui : #{nombre_unites(true)}"
  c = caracteristiques("chat")
  puts "nombre_unites lui : #{c.pa_init} #{c.portee}"
  puts "afficher... unite #{u.id} (#{u.pos.x}, #{u.pos.y})"
  afficher_unite(u);
end

def end_game()
  # fonction a completer
  puts "end game !"
end

