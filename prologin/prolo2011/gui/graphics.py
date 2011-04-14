# -*- coding: utf-8 -*-

import sys
import threading
import traceback

import pygame

from api import *
import game
import images

# TODO: erase this when the constant is updated upstream
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
            looping = self.handle_events()
            self.update_graphics()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN and \
                    event.key == pygame.K_SPACE:
                self.state_reader.go_next()
        return True

    def update_graphics(self):
        self.screen.blit(self.img_grid, (0, 0))
        if self.game_state is None:
            return

        bonus_img = self.img['bonus']
        for (position, value) in self.game_state.bonusgrid:
            bonus = ''
            if value == PLUS_LONG:
                bonus = 'plus_long'
            elif value == PLUS_PA:
                bonus = 'plus_pa'
            elif value == BONUS_CROISEMENT:
                bonus = 'bonus_croisement'
            elif value == BONUS_REGENERATION:
                bonus = 'bonus_regeneration'
            else:
                continue

            images.place_img(position, self.screen,
                             bonus_img[bonus], self.img_props)

        (conn_trainees, conn_sources) = self.game_state.get_connected_objects()
        img_moto = self.img['trainee_moto']
        for trainee_moto in self.game_state.motos:
            if trainee_moto in conn_trainees:
                state = 'on'
            else:
                state = 'off'
            img = img_moto[state]
            for (position, pattern) in trainee_moto.patterns():
                images.place_img(position, self.screen,
                        img[pattern][trainee_moto.team],
                        self.img_props)
            nodes = trainee_moto.nodes
            if len(nodes) > 1:
                dir = game.undo_direction(nodes[-2], nodes[-1])
            else:
                dir = game.LEFT
            dir = game.direction_to_str(dir)
            images.place_img(trainee_moto.nodes[-1], self.screen,
                             self.img['moto'][dir][trainee_moto.team], self.img_props)

        terrain_img = self.img['terrain']
        for (position, value) in self.game_state.ground:
            if value == OBSTACLE:
                str_val = 'obstacle'
            elif value == POINT_CROISEMENT:
                str_val = 'point_croisement'
            else:
                continue

            images.place_img(position, self.screen,
                             terrain_img[str_val], self.img_props)

        source_imgs = self.img['source_energie']
        for source in self.game_state.sources:
            if source.coefficient < 0:
                status = 'consommateur'
            elif source.coefficient > 0:
                status = 'producteur'
            else:
                images.place_img(source.position, self.screen,
                                 source_imgs['epuise'], self.img_props)
                continue

            if source in conn_sources:
                state = 'on'
            else:
                state = 'off'

            images.place_img(source.position, self.screen,
                             source_imgs[state][status], self.img_props)

        pygame.display.flip()

    def init(self):
        pygame.init()
        self.img_props = images.load_props()
        vshift = self.img_props['vshift']
        tile_size = self.img_props['tile_size']
        screen_dim = (TAILLE_TERRAIN * tile_size[0],
                      vshift + TAILLE_TERRAIN * tile_size[1])
        self.screen = pygame.display.set_mode(screen_dim, pygame.DOUBLEBUF |
                pygame.HWSURFACE)
        self.clock = pygame.time.Clock()
        self.img = images.load_pix(2)
        self.img_grid = images.get_grid((TAILLE_TERRAIN, TAILLE_TERRAIN),
                self.img_props)

    def release(self):
        pygame.quit()
