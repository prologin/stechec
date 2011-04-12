# -*- coding: utf-8 -*-

from api import *

TAILLE_TERRAIN = 50

LEFT = 8
TOP = 4
RIGHT = 2
BOTTOM = 1
NONE = 0
ALL_DIRECTIONS = (LEFT, TOP, RIGHT, BOTTOM)

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

class Team:
    def __init__(self, no, score, bonus):
        self.no = no
        self.score = score
        self.bonus = bonus

class Source:
    def __init__(self, position, coefficient, grid):
        self.position = position
        self.coefficient = coefficient
        self.grid = grid
        grid[position] = self

    def connections_lookup(self):
        result = set()
        for direction in ALL_DIRECTIONS:
            pos = apply_direction(self.position, direction)
            if not self.grid.contains(pos):
                continue
            obj = self.grid[pos]
            if isinstance(obj, Moto):
                result.add(obj)
        return result

class Moto:
    def __init__(self, team, nodes, grid):
        self.team = team
        self.nodes = nodes
        self.grid = grid
        for pos in nodes:
            self.grid[pos] = self

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
            for direction in ALL_DIRECTIONS:
                pos = apply_direction(node, direction)
                if not self.grid.contains(pos):
                    continue
                obj = self.grid[pos]
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

        self.ground = Grid(lambda x: VIDE)
        self.bonusgrid = Grid(regarder_type_bonus)
        self.objgrid = Grid(lambda x: None)
        self.motos = [
                Moto(m.team, [(pos.x, pos.y) for pos in m.emplacement], self.objgrid)
                for m in trainees_moto()
                ]
        self.sources = [Source((s.pos.x, s.pos.y), s.coef, self.objgrid)
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
            if any(source.coefficient < 0 for source in sources) and \
                any (source.coefficient > 0 for source in sources):
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
