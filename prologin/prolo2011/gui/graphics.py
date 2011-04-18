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
from team_surface import TeamSurface
from state_surface import StateSurface

class State:
    TURN_FPS = 1

    def __init__(self):
        self.looping = False
        self.ticks = 0
        self.loop_delay = 1000 / State.TURN_FPS

    def switch_looping(self):
        self.looping = not self.looping
        self.ticks = 0

    def check_loop(self):
        if not self.looping:
            return False
        ticks = pygame.time.get_ticks()
        if ticks - self.ticks >= State.TURN_FPS:
            self.ticks = ticks
            return True
        return False

class Graphics:
    FPS = 20

    def __init__(self, state_reader):
        self.state = State()
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
        self.release()

    def loop(self):
        looping = True
        while looping:
            self.clock.tick(Graphics.FPS)
            self.update_state()
            looping = self.handle_events()
            self.update_graphics()

        self.state_reader.wait_end()
        self.state_surf.update_end()
        looping = True
        while not self.state_reader.is_ended():
            if self.state_reader.is_ended():
                break
            self.clock.tick(Graphics.FPS)
            self.update_graphics()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    self.state.switch_looping()
                elif not self.state.looping and event.key == pygame.K_n:
                    self.go_next_turn()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                self.field_surf.click(event.pos)
        return True

    def go_next_turn(self):
        self.state_reader.go_next()
        self.state_surf.update_wait()

    def update_state(self):
        if self.state_reader.is_ended():
            return

        if self.state.check_loop():
            self.go_next_turn()

        game_state = self.state_reader.get_next_state()
        if game_state is not None:
                self.game_state = game_state
                self.field_surf.update_field(game_state)
                self.team_surf.update_teams(game_state)
                self.state_surf.update_turn(self.state_reader.get_turn())

    def update_graphics(self):
        self.screen.fill((0, 0, 0))
        self.field_surf.blit(self.screen)
        self.team_surf.blit(self.screen)
        self.state_surf.blit(self.screen)
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
        self.team_surf = TeamSurface((self.field_surf.size[0], 16))
        self.state_surf = StateSurface(
            (self.team_surf.position[0],
             self.team_surf.position[1] + self.team_surf.size[1]
            ), self.team_surf.size[0])
        screen_dim = (
            self.team_surf.position[0] + self.team_surf.size[0],
            self.field_surf.size[1]
            )
        self.screen = pygame.display.set_mode(screen_dim, flags)

    def release(self):
        pygame.quit()
