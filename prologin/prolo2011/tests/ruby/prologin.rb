# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

require 'constants.rb'

def init_game()
  # fonction a completer
end

def jouer()
  ma_team = mon_equipe();
  puts "equipe : #{ma_team}"

  for y in (0 .. TAILLE_TERRAIN - 1) do
    for x in (0 .. TAILLE_TERRAIN - 1) do
      p = Position.new(x, y);
      c = regarder_type_case(p);
      if c == "vide" then
        str = "."
      elsif c == "obstacle" then
        str = "#"
      elsif c == "bonus" then
        str = "*"
      elsif c == "point_croisement" then
        str = "+"
      elsif c == "source" then
        str = "!"
      else
        str = "==#{c}=="
      end
      print str;
    end
    puts
  end


end
def end_game()
  # fonction a completer
end
