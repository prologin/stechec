# -*- coding: utf-8 -*-

from api import *

import pygame

import surface
import paths

class StateSurface(surface.Surface):
    TEXT_SIZE = 12
    TEXT_HEIGHT = 15
    def __init__(self, position, width):
        rect = position + (width, 3 * StateSurface.TEXT_HEIGHT)
        surface.Surface.__init__(self, rect)
        self.font = pygame.font.Font(paths.get_font('font.ttf'),
                                     StateSurface.TEXT_SIZE)
        self.last_turn = -1

    def update_turn(self, turn):
        self.surface.fill((0, 0, 0))
        self.last_turn = turn
        message = u'Tour %d / %d' % (turn, FIN_PARTIE)
        text = self.font.render(message, True, (255, 255, 255))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            0
            )
        self.surface.blit(text, pos)

    def update_wait(self):
        message = u'Attente du prochain'
        text = self.font.render(message, True, (128, 128, 128))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            StateSurface.TEXT_HEIGHT
            )
        self.surface.blit(text, pos)
        message = u'champion…'
        text = self.font.render(message, True, (128, 128, 128))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            StateSurface.TEXT_HEIGHT * 2
            )
        self.surface.blit(text, pos)

    def update_end(self):
        self.surface.fill((0, 0, 0))
        message = u'Tour %d / %d' % (self.last_turn, FIN_PARTIE)
        text = self.font.render(message, True, (255, 255, 255))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            0
            )
        self.surface.blit(text, pos)

        message = u'Match terminé'
        text = self.font.render(message, True, (128, 128, 128))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            StateSurface.TEXT_HEIGHT
            )
        self.surface.blit(text, pos)

    def update_wait_end(self, turn):
        self.surface.fill((0, 0, 0))
        self.last_turn = turn
        message = u'Tour %d / %d' % (turn, FIN_PARTIE)
        text = self.font.render(message, True, (255, 255, 255))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            0
            )
        self.surface.blit(text, pos)

        message = u'Attente de la fin'
        text = self.font.render(message, True, (255, 0, 0))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            StateSurface.TEXT_HEIGHT
            )
        self.surface.blit(text, pos)

        message = u'du match'
        text = self.font.render(message, True, (255, 0, 0))
        size = text.get_size()
        pos = (
            (self.size[0] - size[0]) / 2,
            StateSurface.TEXT_HEIGHT * 2
            )
        self.surface.blit(text, pos)
