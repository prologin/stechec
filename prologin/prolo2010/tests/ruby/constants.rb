# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_ruby.rb

# Taille de départ du terrain.
TAILLE_DEPART = 19

# Le nombre maximal d'unités pouvant appartenir à une équipe.
NBR_MAX_UNITES = 10

# Le temps, en nombre de tours, entre deux rétrécissement du terrain.
TEMPS_RETRECISSEMENT = 5

# Le nombre maximum en jeu de chaque carte.
MAX_CARTES = 2

# Représente une position sur le terrain du jeu.
class Position
    attr_reader :x # int
    attr_reader :y # int
  def initialize(x, y)
    @x = x
    @y = y
  end
end

# Contient les informations sur la taille du terrain du jeu.
class Taille_terrain
    attr_reader :taille # int
    attr_reader :min_coord # int
    attr_reader :max_coord # int
  def initialize(taille, min_coord, max_coord)
    @taille = taille
    @min_coord = min_coord
    @max_coord = max_coord
  end
end

# Donne les caractéristiques d'un type d'unité.
class Caracs
    attr_reader :pa_init # int
    attr_reader :portee # int
  def initialize(pa_init, portee)
    @pa_init = pa_init
    @portee = portee
  end
end

# Représente une unité sur le terrain.
class Unite
    attr_reader :pos # position
    attr_reader :ennemi # bool
    attr_reader :type_unite_actuel # type_unite
    attr_reader :vrai_type_unite # type_unite
    attr_reader :ko # int
    attr_reader :pa # int
    attr_reader :attaques # int
    attr_reader :attaques_gratuites # int
    attr_reader :id # int
  def initialize(pos, ennemi, type_unite_actuel, vrai_type_unite, ko, pa, attaques, attaques_gratuites, id)
    @pos = pos
    @ennemi = ennemi
    @type_unite_actuel = type_unite_actuel
    @vrai_type_unite = vrai_type_unite
    @ko = ko
    @pa = pa
    @attaques = attaques
    @attaques_gratuites = attaques_gratuites
    @id = id
  end
end

# Représente l'ensemble des cartes que vous pouvez utiliser.
class Cartes
    attr_reader :potion # int
    attr_reader :deguisement # int
    attr_reader :banzai # int
    attr_reader :pacifisme # int
  def initialize(potion, deguisement, banzai, pacifisme)
    @potion = potion
    @deguisement = deguisement
    @banzai = banzai
    @pacifisme = pacifisme
  end
end

