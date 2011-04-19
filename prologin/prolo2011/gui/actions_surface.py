# -*- coding: utf-8 -*-

import pygame

import game
import surface
import paths

def get_height():
    return ActionsSurface.LINE_HEIGHT * ActionsSurface.LINES \
        + 2 * ActionsSurface.PADDING

class ActionsSurface(surface.Surface):
    PADDING = 8
    LINE_HEIGHT = 15
    LINES = 3
    COLORS = ((0, 128, 255), (255, 128, 0))

    def __init__(self, position, width):
        surface.Surface.__init__(self, position + (width, get_height()))

        self.font = pygame.font.Font(paths.get_font('font.ttf'), 12)
        self.actions_surf = None
        self.cur = 0
        self.max = 0
        self.text_width = self.size[0] - 2 * ActionsSurface.PADDING
        self.text_height = self.size[1] - 2 * ActionsSurface.PADDING

        self.arrow_up = self.font.render(u'↑', True, (255, 255, 255))
        self.arrow_down = self.font.render(u'↓', True, (255, 255, 255))
        self.arrow_size = self.arrow_up.get_size()

    def update_actions(self, actions):
        self.cur = 0
        self.max = max(0, len(actions) - 3)
        size = (self.text_width,
                ActionsSurface.LINE_HEIGHT * len(actions))
        self.actions_surf = pygame.Surface(size).convert_alpha()
        self.actions_surf.fill((0, 0, 0))
        vshift = 0
        for action in actions:
            player = action[1]
            action = game.action_to_str(action)
            text = self.font.render(action, True, ActionsSurface.COLORS[player])
            self.actions_surf.blit(text, (0, vshift))
            vshift += text.get_size()[1]
        self.update_roll(0)

    def update_roll(self, incr):
        self.cur += incr
        if self.cur < 0:
            self.cur = 0
        elif self.cur > self.max:
            self.cur = self.max
        if self.actions_surf is None:
            return
        self.surface.fill((0, 0, 0))
        self.surface.blit(self.actions_surf,
                          (ActionsSurface.PADDING, ActionsSurface.PADDING,
                           self.text_width, self.text_height),
                          (0, self.cur * ActionsSurface.LINE_HEIGHT,
                           self.text_width, self.text_height))
        if self.cur > 0:
            self.surface.blit(self.arrow_up,
                              (self.text_width - self.arrow_size[0], 0))
        if self.cur < self.max:
            self.surface.blit(self.arrow_down,
                              (self.text_width - self.arrow_size[0],
                               self.text_height - self.arrow_size[1]))
