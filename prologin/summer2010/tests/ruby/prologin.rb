# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

require 'constants.rb'

def init_game()
  # fonction a completer
end

def afficher()
  ma_team = mon_equipe();
  autre_team = if ma_team == 0 then 1 else 0 end
  puts "mon equipe = #{ma_team}"
  puts "mon argent = #{score(ma_team)}"
  puts "son argent = #{score(autre_team)}"
  pieces = pieces_a_vennir();
  pieces.each do |p|
    puts "au #{p.tour_apparition}e tour, une piece de : #{p.valeur} va arriver en #{p.pos_piece.x} #{p.pos_piece.y}"
  end

  map = []
  for x in (0 .. TAILLE_TERRAIN - 1) do
    map[x] = []
    for y in (0 .. TAILLE_TERRAIN - 1) do
      map[x][y] = "|   "
    end
  end

  pieces = pieces_en_jeu();
  pieces.each do |p|
    map[p.pos_piece.x][p.pos_piece.y] = "|\e[033m #{p.valeur} \e[0m"
  end

  u = unites();
  u.each do |p|
    color = if p.team == ma_team then "\e[034m" else "\e[031m" end
    map[p.pos_unite.x][p.pos_unite.y] = "|#{color}#{p.ko}-#{p.pa}\e[0m"
    puts "id=#{p.id} team = #{p.team} pos = #{p.pos_unite.x}, #{p.pos_unite.y}"
  end

  for y in (0 .. TAILLE_TERRAIN - 1) do
    for x in (0 .. TAILLE_TERRAIN - 1) do
      print map[x][y]
    end
    puts "|"
  end
end

def jouer()
  afficher()
  puts <<EOF
ecrivez une commande ruby.
tapez "next" (ou n) pour passer au tour suivant
tapez "show" (ou s) pour afficher le plateau
EOF
  line = ""
  while (line != "next\n" && line != "n\n")
    line = $stdin.readline
    if line == "show\n" || line == "s\n" then
      afficher()
    elsif line != "next\n" &&  line != "n\n"  then
      out = eval(line);
      puts "retour : #{out}"
    end
  end
end 

def end_game()
  # fonction a completer
end

