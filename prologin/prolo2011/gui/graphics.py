# -*- coding: utf-8 -*-

import sys
import threading
import traceback

import pygame

from api import *
import game
import images
import surface
from field_surface import FieldSurface

# TODO: erase this when the constant is updated upstream in ALL FILES
TAILLE_TERRAIN = 50

class State:
    TURN_FPS = 1

    def __init__(self):
        self.running = True
        self.timer = pygame.time.Clock(State.TURN_FPS)

class Graphics:
    FPS = 20

    def __init__(self, state_reader):
        self.state_reader = state_reader
        self.game_state = None

    def run(self):
        try:
            self.loop()
        except Exception as e:
            print >> sys.stderr, 'An exception was caught in the GUI'
            (type, value, tb) = sys.exc_info()
            traceback.print_tb(tb)
            print type, value
        self.state_reader.wait_end()
        self.release()

    def loop(self):
        looping = True
        while looping:
            self.clock.tick(Graphics.FPS)
            if not self.state_reader.is_ended():
                game_state = self.state_reader.get_next_state()
                if game_state is not None:
                    self.game_state = game_state
                    self.field_surf.update_field(game_state)
            looping = self.handle_events()
            self.update_graphics()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN and \
                    event.key == pygame.K_SPACE:
                self.state_reader.go_next()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                self.field_surf.click(event.pos)
        return True

    def update_graphics(self):
        self.screen.fill((0, 0, 0))
        self.field_surf.blit(self.screen)
        pygame.display.flip()

    def init(self):
        # Pygame initialization
        pygame.init()
        flags = pygame.DOUBLEBUF | pygame.HWSURFACE
        screen_dim = (1, 1)
        # This first instance is used to format new surfaces
        self.screen = pygame.display.set_mode(screen_dim, flags)

        self.clock = pygame.time.Clock()
        self.field_surf = FieldSurface((0, 0))
        screen_dim = self.field_surf.size
        self.screen = pygame.display.set_mode(screen_dim, flags)

    def release(self):
        pygame.quit()
