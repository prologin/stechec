# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

# Taille du terrain.
TAILLE_TERRAIN = 21

# le prix de la fin d'une partie
PRIX_FIN_PARTIE = 25

# le nombre de points d'actions des unites au debut du tour.
MAX_PA = 3

# Le nombre d'unités par équipe.
NBR_UNITES = 9

# Le nombre maximum de tours.
MAX_TURN = 100

# Représente une position sur le terrain du jeu.
class Position
    attr_reader :x # int
    attr_reader :y # int
  def initialize(x, y)
    @x = x
    @y = y
  end
end

# les caracteristiques d'un objet
class Caracteristiques_objet
    attr_reader :coute # int
    attr_reader :porte # int
  def initialize(coute, porte)
    @coute = coute
    @porte = porte
  end
end

# Représente une unité sur le terrain.
class Unite
    attr_reader :pos_unite # position
    attr_reader :team # int
    attr_reader :ko # int
    attr_reader :pa # int
    attr_reader :objet # type_objet
    attr_reader :id # int
  def initialize(pos_unite, team, ko, pa, objet, id)
    @pos_unite = pos_unite
    @team = team
    @ko = ko
    @pa = pa
    @objet = objet
    @id = id
  end
end

# Représente le type d'une piece.
class Piece
    attr_reader :valeur # int
    attr_reader :pos_piece # position
    attr_reader :tour_apparition # int
  def initialize(valeur, pos_piece, tour_apparition)
    @valeur = valeur
    @pos_piece = pos_piece
    @tour_apparition = tour_apparition
  end
end

