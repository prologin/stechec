# -*- coding: utf-8 -*-

import pygame

from api import *

import game
import images
import paths
import surface

class DetailSurface(surface.Surface):
    HEIGHT = 400
    PADDING = 8

    def __init__(self, position, width):
        surface.Surface.__init__(self,
                                 position + (width, DetailSurface.HEIGHT))
        self.imgs = images.load_pix()
        self.font = pygame.font.Font(paths.get_font('font.ttf'), 12)

    def update_pos(self, position, ground, bonus, objects):
        self.surface.fill((0, 0, 0))

        vshift = DetailSurface.PADDING
        vshift += self.put_position(vshift, position)
        if ground == VIDE and bonus == PAS_BONUS and not objects:
            vshift += self.put_empty(vshift)
        else:
            vshift += self.put_ground(vshift, ground)
            vshift += self.put_bonus(vshift, bonus)
            for obj in objects: 
                vshift += self.put_object(obj, vshift)

    def put_position(self, vshift, position):
        text = self.font.render(u'Position : (%d, %d)' % position,
                                True, (255, 255, 255))
        self.surface.blit(text, (DetailSurface.PADDING, vshift))
        return text.get_size()[1]

    def put_empty(self, vshift):
        text = self.font.render(u'<Vide>', True, (128, 128, 128))
        self.surface.blit(text, (DetailSurface.PADDING, vshift))
        return text.get_size()[1]

    def put_bonus(self, vshift, bonus):
        str = ''
        desc = ''
        if bonus == PLUS_LONG:
            str = 'plus_long'
            desc = u'Allongement'
        elif bonus == PLUS_PA:
            str = 'plus_pa'
            desc = u'Point d’action'
        elif bonus == BONUS_CROISEMENT:
            str = 'bonus_croisement'
            desc = u'Point de croisement'
        elif bonus == BONUS_REGENERATION:
            str = 'bonus_regeneration'
            desc = u'Régénération'
        else:
            return 0
        
        img = self.imgs['bonus'][str]
        img_sz = img.get_size()
        pos = (
            self.size[0] - img_sz[0] - DetailSurface.PADDING,
            vshift
            )
        self.surface.blit(img, pos)
        text = self.font.render(desc, True, (255, 255, 255))
        self.surface.blit(text, (DetailSurface.PADDING, vshift))
        return pos[1] + max(img_sz[1], text.get_size()[1])

    def put_ground(self, vshift, ground):
        str = ''
        if ground == VIDE:
            str = 'Vide'
            return 0
        elif ground == OBSTACLE:
            str = 'Obstacle'
        elif ground == POINT_CROISEMENT:
            str = 'Point de croisement'
        elif ground == SOURCE:
            str = 'Source'
        else:
            str = '???'
        text = self.font.render(str, True, (255, 255, 255))
        vshift += DetailSurface.PADDING
        self.surface.blit(text, (DetailSurface.PADDING, vshift))
        return text.get_size()[1]

    def put_object(self, obj, vshift):
        height = 45
        if isinstance(obj, game.Source):
            logo = self.imgs['source_energie']['off']
            if obj.capacite_max < 0:
                logo = logo['consommateur']
            else:
                logo = logo['producteur']
            img_sz = logo.get_size()
            pos = (
                DetailSurface.PADDING,
                vshift + (height - img_sz[1]) / 2
                )
            self.surface.blit(logo, pos)
            hshift = pos[0] + img_sz[0] + DetailSurface.PADDING

            vshift += 8
            message = 'Id: %d     (%d, %d)' % ((obj.id, ) + obj.position)
            text = self.font.render(message, True, (255, 255, 255))
            pos = (hshift, vshift)
            self.surface.blit(text, pos)
            
            message = u'Capacité: %d / %d' % (obj.capacite, obj.capacite_max)
            pos = (hshift, vshift + text.get_size()[1])
            text = self.font.render(message, True, (255, 255, 255))
            self.surface.blit(text, pos)
        elif isinstance(obj, game.Moto):
            logo = self.imgs['moto']['left'][obj.team]
            img_sz = logo.get_size()
            pos = (
                DetailSurface.PADDING,
                vshift + (height - img_sz[1]) / 2
                )
            self.surface.blit(logo, pos)
            hshift = pos[0] + img_sz[0] + DetailSurface.PADDING

            vshift += 8
            message = 'Id: %d (team %d)' % (obj.id, obj.team)
            text = self.font.render(message, True, (255, 255, 255))
            pos = (hshift, vshift)
            self.surface.blit(text, pos)
            
            message = u'Longueur: %d' % (len(obj.nodes))
            pos = (hshift, vshift + text.get_size()[1])
            text = self.font.render(message, True, (255, 255, 255))
            self.surface.blit(text, pos)
        else:
            return 0
        return height
