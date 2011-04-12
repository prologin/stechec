# -*- coding: utf-8 -*-

import sys
import threading

import pygame

from api import *
import game
TAILLE_TERRAIN = 50

class State:
    TURN_FPS = 1

    def __init__(self):
        self.running = True
        self.timer = pygame.time.Clock(State.TURN_FPS)

class Graphics:
    CELL_SIZE = (10, 10)
    FPS = 20

    def __init__(self, pipe, new_turn, end_turn):
        self.game_state = None
        self.waiting_turn = False

        self.pipe = pipe
        self.new_turn = new_turn
        self.end_turn = end_turn
        self.end_game = threading.Event()

    def check_update(self):
        if self.new_turn.is_set():
            self.game_state = self.pipe.get()
            self.new_turn.clear()
            self.waiting_turn = True

    def pass_turn(self):
        if self.waiting_turn:
            self.end_turn.set()
            self.waiting_turn = False

    def run(self):
        try:
            looping = True
            while looping:
                self.clock.tick(Graphics.FPS)
                if not self.end_game.is_set():
                    self.check_update()
                looping = self.handle_events()
                self.update_graphics()
        finally:
            while not self.end_game.is_set():
                self.end_turn.set()
        self.release()

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN and \
                    event.key == pygame.K_SPACE:
                self.pass_turn()
        return True

    def update_graphics(self):
        self.screen.fill((0, 0, 0))
        if self.game_state is None:
            return
        cell_size = Graphics.CELL_SIZE
        for ((x, y), value) in self.game_state.ground:
            color = (0, 0, 0)
            if value == OBSTACLE:
                color = (255, 255, 255)
            elif value == POINT_CROISEMENT:
                color = (128, 128, 128)
            self.screen.fill(color,
                    (x * cell_size[0], y * cell_size[0],
                     cell_size[0], cell_size[1]))
        for ((x, y), value) in self.game_state.bonusgrid:
            if value == PAS_BONUS:
                continue
            color = (192, 64, 192)
            self.screen.fill(color,
                    (x * cell_size[0], y * cell_size[1],
                     cell_size[0], cell_size[1]))
        for ((x, y), value) in self.game_state.objgrid:
            color = (0, 0, 0)
            if isinstance(value, game.Moto):
                if value.team == 0:
                    color = (32, 128, 224)
                else:
                    color = (224, 128, 32)
            elif isinstance(value, game.Source):
                if value.coefficient < 0:
                    color = (255, 0, 0)
                elif value.coefficient == 0:
                    color = (255, 255, 0)
                else:
                    color = (0, 255, 0)
            else:
                continue
            self.screen.fill(color,
                    (x * cell_size[0], y * cell_size[1],
                     cell_size[0], cell_size[1]))
        pygame.display.flip()

    def init(self):
        pygame.init()
        screen_dim = (50 * 10, 50 * 10)
        self.screen = pygame.display.set_mode(screen_dim, pygame.DOUBLEBUF |
                pygame.HWSURFACE)
        self.clock = pygame.time.Clock()

    def quit(self):
        self.end_game.set()

    def release(self):
        pygame.quit()
