# -*- coding: utf-8 -*-

import pygame

from api import *

import paths
import surface

class HelpSurface(surface.Surface):
    PADDING = 16
    MARGIN = 64
    WIDTH = 375
    HEIGHT = 350

    BACKGROUND = (0, 0, 0)
    BORDER = (32, 48, 64)

    def __init__(self, screen_dim):
        dim = (HelpSurface.WIDTH, HelpSurface.HEIGHT)
        pos = (
            (screen_dim[0] - dim[0]) / 2,
            (screen_dim[1] - dim[1]) / 2
            )
        surface.Surface.__init__(self, pos + dim)
        self.font = pygame.font.Font(paths.get_font('font.ttf'), 13)

        self.update()

    def update(self):
        self.surface.fill(HelpSurface.BORDER)
        self.surface.fill(HelpSurface.BACKGROUND,
                          (1, 1, self.size[0] - 2, self.size[1] - 2))

        vshift = HelpSurface.PADDING
        messages = (
            (u'H', u'Afficher/cacher l’aide'),
            None,
            (u'Space', u'Activer/désactiver la pause'),
            (u'N', u'Passer au tour suivant (en pause)'),
            None,
            (u'↑', u'Monter dans la liste des actions'),
            (u'↓', u'Descendre dans la liste des actions'),
            )
        for msg in messages:
            if msg is None:
                vshift += HelpSurface.PADDING * 3
                continue
            (letter, msg) = msg
            vshift += self.put_line(letter, msg, vshift) + HelpSurface.PADDING
    
    def put_line(self, letter, message, vshift):
        hshift = HelpSurface.PADDING
        htshift = HelpSurface.PADDING + HelpSurface.MARGIN
        but = self.make_button(letter, self.font)
        self.surface.blit(but, (hshift, vshift))
        text = self.font.render(message, True, (255, 255, 255))
        pos = (
            htshift,
            vshift + (but.get_size()[1] - text.get_size()[1]) / 2
            )
        self.surface.blit(text, pos)
        return but.get_size()[1]
