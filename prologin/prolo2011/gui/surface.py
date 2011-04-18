# -*- coding: utf-8 -*-

import pygame

class Surface:
    def __init__(self, rect):
        self.position = rect[:2]
        self.set_size(rect[2:])

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

    def set_position(self, position):
        self.position = position

    def set_size(self, size):
        self.size = size
        self.surface = pygame.Surface(self.size)
        self.surface = self.surface.convert_alpha()
        self.surface.fill((0, 0, 0, 0))

    def recv_click(self, position):
        print 'Click @ (%d, %d)' % position

    def make_button(self, label, font):
        text = font.render(label, True, (0, 0, 0))
        text_sz = text.get_size()
        button_sz = (text_sz[0] + 16, text_sz[1] + 8)
        button = pygame.Surface(button_sz)
        button.fill((192, 192, 192))
        button.fill((64, 64, 64),
                    (2, 2, button_sz[0] - 2, button_sz[1] - 2))
        button.fill((128, 128, 128),
                    (2, 2, button_sz[0] - 4, button_sz[1] - 4))
        button.blit(text, ((button_sz[0] - text_sz[0]) / 2,
                           (button_sz[1] - text_sz[1]) / 2))
        return button.convert_alpha()
