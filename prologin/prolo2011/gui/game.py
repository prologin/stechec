# -*- coding: utf-8 -*-

from api import *

LEFT = 8
TOP = 4
RIGHT = 2
BOTTOM = 1
NONE = 0
ALL_DIRECTIONS = (LEFT, TOP, RIGHT, BOTTOM)

(NB_SOURCES, CONTENU_CASE, SOURCE_CONTENT, MOTO_POS,
 ACT_DEPLACER, ACT_COUPER_TRAINEE_MOTO, ACT_FUSIONNER, ACT_ENROULER,
 ACT_REGENERER, ACT_ALLONGER_PA, ACT_AGRANDIR, ACT_POSER_PT_CROIX,
 LAST_MSG) = range(13)

def apply_direction(point, direction):
    (x, y) = point
    if direction == LEFT:
        return (x - 1, y)
    elif direction == TOP:
        return (x, y - 1)
    elif direction == RIGHT:
        return (x + 1, y)
    elif direction == BOTTOM:
        return (x, y + 1)
    return None

def undo_direction(point, next_point):
    (ox, oy) = point
    (nx, ny) = next_point
    delta = (nx - ox, ny - oy)
    if delta == (-1, 0):
        return LEFT
    elif delta == (0, -1):
        return TOP
    elif delta == (1, 0):
        return RIGHT
    elif delta == (0, 1):
        return BOTTOM
    return None

def direction_to_str(direction):
    if direction == LEFT:
        return 'left'
    elif direction == TOP:
        return 'top'
    elif direction == RIGHT:
        return 'right'
    elif direction == BOTTOM:
        return 'bottom'
    return None

def serialize_pattern(pat):
    return bin(pat)[2:].rjust(4, '0')

def action_to_str(action):
    act_type = action[0]
    act_args = tuple(action[2:])
    if act_type == ACT_DEPLACER:
        return u'Déplacement de la traînée %d : (%d, %d) → (%d, %d)' % act_args
    elif act_type == ACT_COUPER_TRAINEE_MOTO:
        return u'Coupure de la traînée %d : (%d, %d) ↔ (%d, %d)' % \
            act_args
    elif act_type == ACT_FUSIONNER:
        return u'Fusion des traînées %d et %d : (%d, %d) ↔ (%d, %d)' % \
            act_args
    elif act_type == ACT_ENROULER:
        return u'Enroulage de la traînée %d en (%d, %d)' % act_args
    elif act_type == ACT_REGENER:
        return u'Source %d regénérée' % (act_args[1])
    elif act_type == ACT_ALLONGER_PA:
        return u'Bonus Point d’Action'
    elif act_type == ACT_AGRANDIR:
        return u'Bonus Allonger traînée %d (+%d)' % act_args
    elif act_type == ACT_POSER_PT_CROIX:
        return u'Bonus Point de Croisement en (%d, %d)' % act_args

class Team:
    def __init__(self, no, score, bonus):
        self.no = no
        self.score = score
        self.bonus = bonus

    def count_bonus(self):
        result = {}
        result['plus_long'] = len([b for b in self.bonus if b == PLUS_LONG])
        result['plus_pa'] = len([b for b in self.bonus if b == PLUS_PA])
        result['bonus_croisement'] = len([b for b in self.bonus
                                          if b == BONUS_CROISEMENT])
        result['bonus_renegeration'] = len([b for b in self.bonus
                                            if b == BONUS_REGENERATION])
        return result

class Source:
    def __init__(self, position, capacite, capacite_max, id, grid):
        self.id = id
        self.position = position
        self.capacite = capacite
        self.capacite_max = capacite_max
        self.grid = grid
        grid[position].append(self)

    def connections_lookup(self):
        result = set()
        for direction in ((-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1),
                          (0, 1), (-1, 1)):
            pos = (self.position[0] + direction[0],
                   self.position[1] + direction[1])
            if not self.grid.contains(pos):
                continue
            for obj in self.grid[pos]:
                if isinstance(obj, Moto):
                    result.add(obj)
        return result

class Moto:
    def __init__(self, team, nodes, id, grid):
        self.id = id
        self.team = team
        self.nodes = nodes
        self.grid = grid
        for pos in nodes:
            self.grid[pos].append(self)

    def patterns(self):
        if len(self.nodes) == 1:
            yield (self.nodes[0], 0)
        elif len(self.nodes) > 1:
            yield (self.nodes[0], undo_direction(self.nodes[0], self.nodes[1]))
            for i in xrange(1, len(self.nodes) - 1):
                yield (
                    self.nodes[i],
                    undo_direction(self.nodes[i], self.nodes[i + 1]) |
                    undo_direction(self.nodes[i], self.nodes[i - 1])
                    )
            yield (self.nodes[-1],
                   undo_direction(self.nodes[-1], self.nodes[-2]))

    def connections_lookup(self):
        result = set()
        for node in self.nodes:
            for direction in ((-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0),
                              (1, 1), (0, 1), (-1, 1)):
                pos = (node[0] + direction[0],
                       node[1] + direction[1])
                if not self.grid.contains(pos):
                    continue
                for obj in self.grid[pos]:
                    if isinstance(obj, Source):
                        result.add(obj)
        return result

class Grid:
    SIZE = (TAILLE_TERRAIN, TAILLE_TERRAIN)

    def __init__(self, map=None):
        if map is not None:
            self.grid = [[map(position(x, y)) for x in xrange(TAILLE_TERRAIN)]
                         for y in xrange(TAILLE_TERRAIN)]
        else:
            self.grid = [[None] * TAILLE_TERRAIN for i in xrange(TAILLE_TERRAIN)]

    def __getitem__(self, position):
        (x, y) = position
        return self.grid[y][x]

    def __setitem__(self, position, content):
        (x, y) = position
        self.grid[y][x] = content

    def contains(self, pos):
        (x, y) = pos
        return (x >= 0 and x < TAILLE_TERRAIN) and \
            (y >= 0 and y < TAILLE_TERRAIN)

    def __iter__(self):
        return self.generator()

    def generator(self):
        for y in xrange(TAILLE_TERRAIN):
            for x in xrange(TAILLE_TERRAIN):
                yield ((x, y), self.grid[y][x])

class GameState:
    def __init__(self):
        self.turn_no = tour_actuel()
        self.actions = actions_effectuees()

        self.ground = Grid(lambda x: VIDE)
        self.bonusgrid = Grid(regarder_type_bonus)
        self.objgrid = Grid(lambda x: [])
        self.motos = [
                Moto(m.team, [(pos.x, pos.y) for pos in m.emplacement],
                     m.id, self.objgrid)
                for m in trainees_moto()
                ]
        self.sources = [Source((s.pos.x, s.pos.y),
                               s.capacite, s.capacite_max,
                               s.id,
                               self.objgrid)
                        for s in sources_energie()]

        for y in xrange(TAILLE_TERRAIN):
            for x in xrange(TAILLE_TERRAIN):
                c = regarder_type_case(position(x=x, y=y))
                if c in (OBSTACLE, POINT_CROISEMENT):
                    self.ground[(x, y)] = c

        self.teams = [Team(i, 0, []) for i in xrange(nombre_equipes())]
        for (no, s) in enumerate(scores()):
            self.teams[no].bonus = regarder_bonus(no)
            self.teams[no].score = s

    def get_connected_objects(self):
        connected_trainees_moto = set()
        connected_sources_energie = set()
        for t in self.motos:
            sources = t.connections_lookup()
            if any(source.capacite < 0 for source in sources) and \
                any (source.capacite > 0 for source in sources):
                connected_sources_energie.update(sources)
                connected_trainees_moto.add(t)
        return (connected_trainees_moto, connected_sources_energie)

    

    def dump(self):
        (conn_trainees, conn_sources) = self.get_connected_objects()
        result = ''
        for y in xrange(TAILLE_TERRAIN):
            for x in xrange(TAILLE_TERRAIN):
                obj = self.objgrid[(x, y)]
                if obj is None:
                    result += '.'
                elif isinstance(obj, Source):
                    if obj in conn_sources:
                        result += 'H'
                    else:
                        result += '+'
                elif isinstance(obj, Moto):
                    if obj in conn_trainees:
                        result += 'O'
                    else:
                        result += 'o'
            result += '\n'
        return result[:-1]
