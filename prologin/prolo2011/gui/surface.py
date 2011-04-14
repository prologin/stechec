# -*- coding: utf-8 -*-

import pygame

class Surface:
    def __init__(self, rect):
        self.position = rect[:2]
        self.size = rect[2:]

        self.surface = pygame.Surface(self.size)
        self.surface = self.surface.convert_alpha()
        self.surface.fill((0, 0, 0, 0))

    def blit(self, surface):
        surface.blit(self.surface, self.position)

    def click(self, position):
        (ex, ey) = position
        if ex < self.position[0] or ey < self.position[1] \
                or ex > self.position[0] + self.size[0] \
                or ey > self.position[1] + self.size[1]:
            return False
        self.recv_click((
                position[0] - self.position[0],
                position[1] - self.position[1]
                ))

    def recv_click(self, position):
        print 'Click @ (%d, %d)' % position
