# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

# Taille du terrain
TAILLE_TERRAIN = 100

# Nombre de tours par partie
FIN_PARTIE = 1500

# Nombre de points d'action par tour
MAX_PA = 3

# Taille des traînées de moto
TAILLE_TRAINEE = 900

# Longueur maximale de l'allongement
MAX_ALLONGEMENT = 5

# Représente une position sur le terrain du jeu
class Position
    attr_reader :x # int
    attr_reader :y # int
  def initialize(x, y)
    @x = x
    @y = y
  end
end

# Caracteristiques d'une source d'énergie
class Source_energie
    attr_reader :id # int
    attr_reader :pos # position
    attr_reader :coef # int
  def initialize(id, pos, coef)
    @id = id
    @pos = pos
    @coef = coef
  end
end

# Représente une traînée de moto sur le terrain
class Trainee_moto
    attr_reader :id # int
    attr_reader :emplacement # position array
    attr_reader :team # int
  def initialize(id, emplacement, team)
    @id = id
    @emplacement = emplacement
    @team = team
  end
end

