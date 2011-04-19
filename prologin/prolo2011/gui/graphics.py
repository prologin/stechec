# -*- coding: utf-8 -*-

import sys
import threading
import traceback

import pygame

from api import *
import game
import images
import surface

import field_surface, actions_surface
from field_surface import FieldSurface
from team_surface import TeamSurface
from state_surface import StateSurface
from detail_surface import DetailSurface
from help_surface import HelpSurface
from actions_surface import ActionsSurface

class State:
    TURN_FPS = 1

    def __init__(self):
        self.looping = False
        self.ticks = 0
        self.loop_delay = 1000 / State.TURN_FPS
        self.help = False

    def switch_help(self):
        self.help = not self.help

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
            if self.state.help:
                if event.type == pygame.KEYDOWN \
                        and event.key == pygame.K_h:
                    self.state.switch_help()
                continue
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    self.state.switch_looping()
                    if self.state.looping:
                        self.go_next_turn()
                elif not self.state.looping and event.key == pygame.K_n:
                    self.go_next_turn()
                elif event.key == pygame.K_h:
                    self.state.switch_help()
                elif event.key == pygame.K_DOWN:
                    self.actions_surf.update_roll(1)
                elif event.key == pygame.K_UP:
                    self.actions_surf.update_roll(-1)
            elif event.type == pygame.MOUSEBUTTONDOWN:
                self.field_surf.click(pygame.mouse.get_pos())
        return True

    def go_next_turn(self):
        self.state_reader.go_next()
        self.state_surf.update_wait()

    def update_state(self):
        if self.state_reader.is_ended():
            return

        if self.state.help:
            return

        game_state = self.state_reader.get_next_state()
        if game_state is not None:
            self.game_state = game_state
            self.field_surf.update_field(game_state)
            self.team_surf.update_teams(game_state)
            self.state_surf.update_turn(game_state.turn_no)
            self.actions_surf.update_actions(game_state.actions)

            if self.state.check_loop():
                self.go_next_turn()

    def update_graphics(self):
        self.screen.fill((0, 0, 0))
        self.field_surf.blit(self.screen)
        self.team_surf.blit(self.screen)
        self.state_surf.blit(self.screen)
        self.detail_surf.blit(self.screen)
        if self.state.help:
            self.help_surf.blit(self.screen)
        self.actions_surf.blit(self.screen)
        pygame.display.flip()

    def init(self):
        # Pygame initialization
        pygame.init()
        flags = pygame.DOUBLEBUF | pygame.HWSURFACE
        (w, h) = field_surface.get_dim(TAILLE_TERRAIN)
        screen_dim = (
            TeamSurface.SIZE[0] + w,
            h + actions_surface.get_height()
            )
        # This first instance is used to format new surfaces
        self.screen = pygame.display.set_mode(screen_dim, flags)

        self.clock = pygame.time.Clock()
        self.detail_surf = DetailSurface((0, 0), 1)
        self.field_surf = FieldSurface((0, 0), self.detail_surf)
        self.team_surf = TeamSurface((self.field_surf.size[0], 16))
        self.state_surf = StateSurface(
            (self.team_surf.position[0],
             self.team_surf.position[1] + self.team_surf.size[1]
            ), self.team_surf.size[0])
        self.detail_surf.set_position((self.team_surf.position[0],
                                       self.state_surf.position[1]
                                       + self.state_surf.size[1]))
        self.detail_surf.set_size((self.team_surf.size[0],
                                  DetailSurface.HEIGHT))
        self.help_surf = HelpSurface(screen_dim)
        self.actions_surf = ActionsSurface((0, self.field_surf.size[1]),
                                           screen_dim[0])

    def release(self):
        pygame.quit()
