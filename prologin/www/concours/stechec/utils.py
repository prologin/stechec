#!/usr/bin/env python
#-*- encoding: utf-8 -*-

from concours.settings import *

GRAMMAR = {
        '#': 'wall',
        '$': 'soft',
        '+': 'mesh',
        ' ': 'empty',
        'A': 'p1',
        'B': 'p2',
        'C': 'p3',
        'D': 'p4'}

def map_to_html(data):
    lines = data.split('\n')
    buff = '<div id="map">\n'

    for line in xrange(MAP_HEIGHT):
        buff += '\n<div class="row">\n'
        for col in xrange(MAP_WIDTH):
            try:
                block = GRAMMAR[lines[line][col]]
            except:
                block = 'empty'

            buff += '<div class="cell %s"></div>\n' % block
        buff += '</div>\n'
    buff += '</div>\n'
    return buff

DIFF_CHARSET = list(' #$+ABCD')

def compute_diff(first_turn, second_turn):
    m = 'm'
    e = 'e'
    b = 'b'

    first_turn = eval(first_turn)
    second_turn = eval(second_turn)

    assert(len(first_turn[m]) == (MAP_HEIGHT * MAP_WIDTH))
    assert(len(second_turn[m]) == (MAP_HEIGHT * MAP_WIDTH))

    changes = []
    for y in xrange(MAP_HEIGHT):
        for x in xrange(MAP_WIDTH):
            index = y * MAP_WIDTH + x
            if first_turn[m][index] != second_turn[m][index]:
                try:
                    d = DIFF_CHARSET.index(second_turn[m][index])
                except ValueError:
                    d = 0
                changes.append([x, y, d])

    return repr([changes, second_turn[b], second_turn[e]]).replace(' ', '')
