# -*- coding: utf-8 -*-

import pygame

from api import *

import game
import images
import surface
import paths

def get_dim(taille):
    imgp = images.load_props()
    vshift = imgp['vshift']
    tile_size = imgp['tile_size']
    return (taille * tile_size[0],
            vshift + taille * tile_size[1])

class FieldSurface(surface.Surface):
    BACKGROUND_COLOR = (0, 0, 0)
    GRID_COLOR = (32, 48, 64)
    ERROR_COLOR = (255, 0, 0)

    def __init__(self, position, detail):
        self.img_props = imgp = images.load_props()

        vshift = imgp['vshift']
        tile_size = imgp['tile_size']
        size = (TAILLE_TERRAIN * tile_size[0],
                vshift + TAILLE_TERRAIN * tile_size[1])

        surface.Surface.__init__(self, position + size)
        self.imgs = images.load_pix()
        self.grid_surface = self.get_grid()
        self.detail = detail
        self.detail_pos = None

        self.font = pygame.font.Font(paths.get_font('font.ttf'), 12)

    def get_grid(self):
        imgp = self.img_props
        tile_size = imgp['tile_size']
        vshift = imgp['vshift']
        surf = pygame.surface.Surface(
            (TAILLE_TERRAIN * tile_size[0],
             TAILLE_TERRAIN * tile_size[1] + vshift))
        surf.fill(FieldSurface.BACKGROUND_COLOR)
        a = pygame.surfarray.pixels3d(surf)
        a[::tile_size[0],vshift:] = FieldSurface.GRID_COLOR
        a[:,vshift::tile_size[1]] = FieldSurface.GRID_COLOR
        return surf.convert_alpha()


    def recv_click(self, position):
        if self.game_state is None:
            return None
        x = position[0] / self.img_props['tile_size'][0]
        y = (position[1] - self.img_props['vshift']) \
            / self.img_props['tile_size'][1]
        if y < 0:
            return None
        self.update_detail((x, y))

    def update_detail(self, position):
        self.detail_pos = pos = position
        ground = self.game_state.ground[pos]
        bonus = self.game_state.bonusgrid[pos]
        obj = self.game_state.objgrid[pos]
        self.detail.update_pos(position, ground, bonus, obj)

    def update_field(self, game_state):
        self.game_state = game_state

        self.surface.fill((0, 0, 0))
        self.surface.blit(self.grid_surface, (0, 0))
        if self.game_state is None:
            return

        bonus_img = self.imgs['bonus']
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

            images.place_img(position, self.surface,
                             bonus_img[bonus], self.img_props)

        (conn_trainees, conn_sources) = self.game_state.get_connected_objects()
        img_moto = self.imgs['trainee_moto']
        for trainee_moto in self.game_state.motos:
            if trainee_moto in conn_trainees:
                state = 'on'
            else:
                state = 'off'
            img = img_moto[state]
            for (position, pattern) in trainee_moto.patterns():
                images.place_img(position, self.surface,
                        img[pattern][trainee_moto.team],
                        self.img_props)
            nodes = trainee_moto.nodes
            if len(nodes) > 1:
                dir = game.undo_direction(nodes[-2], nodes[-1])
            else:
                dir = game.LEFT
            dir = game.direction_to_str(dir)
            images.place_img(trainee_moto.nodes[-1], self.surface,
                             self.imgs['moto'][dir][trainee_moto.team], self.img_props)

        terrain_img = self.imgs['terrain']
        for (position, value) in self.game_state.ground:
            if value == OBSTACLE:
                str_val = 'obstacle'
            elif value == POINT_CROISEMENT:
                str_val = 'point_croisement'
            elif value != VIDE:
                self.surface.fill(FieldSurface.ERROR_COLOR,
                                  (self.img_props['tile_size'][0] * position[0],
                                   self.img_props['tile_size'][1] * position[1] + \
                                       self.img_props['vshift'],
                                   self.img_props['tile_size'][0],
                                   self.img_props['tile_size'][1]))
                continue
            else:
                continue

            images.place_img(position, self.surface,
                             terrain_img[str_val], self.img_props)

        source_imgs = self.imgs['source_energie']
        for source in self.game_state.sources:
            if source.capacite < 0:
                status = 'consommateur'
            elif source.capacite > 0:
                status = 'producteur'
            else:
                images.place_img(source.position, self.surface,
                                 source_imgs['epuise'], self.img_props)
                self.paste_source_label(source.position, source.capacite)
                continue

            if source in conn_sources:
                state = 'on'
            else:
                state = 'off'

            images.place_img(source.position, self.surface,
                             source_imgs[state][status], self.img_props)
            self.paste_source_label(source.position, source.capacite)

        if self.detail_pos is not None:
            self.update_detail(self.detail_pos)

    def paste_source_label(self, pos, capacite):
        if capacite != 0:
            color = (0, 0, 0)
        else:
            color = (255, 255, 255)
        text = self.font.render(u'%d' % abs(capacite), True, color)
        props = self.img_props
        middle = (
            float(pos[0] + 0.5) * props['tile_size'][0],
            (pos[1] + 1 - 0.85) * props['tile_size'][1]
            )
        size = text.get_size()
        top_right = (
            int(round(middle[0] - float(size[0]) / 2)),
            int(round(middle[1] - float(size[1]) / 2)) + props['vshift']
            )
        self.surface.blit(text, top_right)
