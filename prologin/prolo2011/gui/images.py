# -*- coding: utf-8 -*-

import os

import pygame
import numpy

import paths
from game import NONE, LEFT, TOP, RIGHT, BOTTOM
from game import serialize_pattern

BACKGROUND_COLOR = (0, 0, 0)
GRID_COLOR = (128, 192, 255)
GRID_COLOR = tuple(c / 4 for c in GRID_COLOR)

TEAM_COLORS = (
        (0, 128, 255),
        (255, 128, 0),
        (255, 0, 0),
        (0, 255, 0),
        (0, 0, 255),
        (255, 255, 0),
        (0, 255, 255),
        (255, 0, 255),
        )

def custom_to_team(image, team):
    custom_img = image.copy()
    a = pygame.surfarray.pixels3d(custom_img)
    color = TEAM_COLORS[team]
    arr = numpy.array([c / 255.0 for c in color])
    a *= arr
    return custom_img.convert_alpha()

def custom_to_state(image, state):
    if state == 'off':
        custom_img = image.copy()
        size = custom_img.get_size()
        a = pygame.surfarray.pixels_alpha(custom_img)
        a *= 0.5
        return custom_img.convert_alpha()
    return image

def load_image(filename):
    '''Load an image from a file and optimize it'''

    path = paths.get_image(filename)
    img = pygame.image.load(path).convert_alpha()
    return img

def load_pix(teams):
    '''Build a dictionary with the tiles images and return it.'''

    images = {}

    images['source_energie'] = {}
    for state in ('off', 'on'):
        images['source_energie'][state] = {}
        for status in ('producteur', 'consommateur'):
            images['source_energie'][state][status] = \
                    load_image('source_energie-%s-%s.png' %
                            (state, status))

    images['terrain'] = {}
    for case in ('obstacle', 'point_croisement'):
        images['terrain'][case] = load_image('terrain-%s.png' % case)

    images['bonus'] = {}
    for bonus in ('plus_long', 'plus_pa',
                 'bonus_croisement', 'bonus_regeneration'):
        images['bonus'][bonus] = load_image('bonus-%s.png' % bonus)

    images['trainee_moto'] = {}
    for state in ('off', 'on'):
        images['trainee_moto'][state] = {}
    for pattern in (NONE, LEFT, TOP, RIGHT, BOTTOM,
                    LEFT | RIGHT, LEFT | TOP, LEFT | BOTTOM,
                    RIGHT | TOP, RIGHT | BOTTOM, TOP | BOTTOM):
        for state in ('off', 'on'):
            spat = serialize_pattern(pattern)
            img = load_image('trainee_moto-%s-%s.png' % (
                             spat, 'off' or state))
            img_pat = custom_to_state(img, state)
            images['trainee_moto'][state][pattern] = \
                [None for i in xrange(teams)]
            for team in xrange(teams):
                images['trainee_moto'][state][pattern][team] = \
                        custom_to_team(img_pat, team)
    images['moto'] = {}
    for dir in ('left', 'top', 'right', 'bottom'):
        images['moto'][dir] = [None for i in xrange(teams)]
        img = load_image('moto-%s.png' % dir)
        for team in xrange(teams):
            images['moto'][dir][team] = custom_to_team(img, team)
    return images

def load_props():
    '''Build a dictionary with the images properties and return it.'''

    props = {
        'tile_size': (16, 16),
        'vshift': 16
        }
    return props

def get_screen_size(field_size):
    props = load_props()
    return (props['tile_size'][0] * field_size[0],
            props['tile_size'][1] * field_size[1] + props['vshift'])

def get_grid(field_size, props):
    tile_size = props['tile_size']
    vshift = props['vshift']
    surf = pygame.surface.Surface(
            (field_size[0] * tile_size[0],
                field_size[1] * tile_size[1] + vshift))
    surf.fill(BACKGROUND_COLOR)
    a = pygame.surfarray.pixels3d(surf)
    a[::tile_size[0],vshift:] = GRID_COLOR
    a[:,vshift::tile_size[1]] = GRID_COLOR
    return surf.convert_alpha()

def place_img(position, field, sprite, props):
    '''Blit an surface onto the field.

    This operation takes into account the represented object's its position and
    images dimension properties.'''

    middle_bottom = (
        float(position[0] + 0.5) * props['tile_size'][0],
        (position[1] + 1) * props['tile_size'][1]
        )
    sprite_size = sprite.get_size()
    top_right = (
        int(round(middle_bottom[0] - float(sprite_size[0]) / 2)),
        middle_bottom[1] - sprite_size[1] + props['vshift']
        )
    field.blit(sprite, top_right)
