# -*- coding: utf-8 -*-

import pygame

import paths
import game
import images
import surface

class TeamSurface(surface.Surface):
    SIZE = (80 * 2 + 3 * 8, 250)

    def __init__(self, position):
        surface.Surface.__init__(self, position + TeamSurface.SIZE)
        self.img_props = images.load_props()
        self.imgs = images.load_pix()

        self.font = pygame.font.Font(paths.get_font('font.ttf'), 12)
        self.font2 = pygame.font.Font(paths.get_font('font.ttf'), 10)

    def get_score(self, score):
        text = self.font.render(str(score), True, (255, 255, 255))
        surf = pygame.Surface((80, text.get_size()[1]))
        surf.fill((32, 32, 32, 255)) # FIXME: doesn't work
        surf_size = surf.get_size()
        text_size = text.get_size()
        pos = ((surf_size[0] - text_size[0]) / 2,
               (surf_size[1] - text_size[1]) / 2)
        surf.blit(text, pos)
        return surf.convert_alpha()

    def get_bonus(self, name, number):
        text = self.font.render(u'×%d' % number, True, (255, 255, 255))
        text_size = text.get_size()
        surf = pygame.Surface((text_size[0] + 16 + 16,
                               max(text_size[1] + 16, 16 + 16)
                               ))
        surf_size = surf.get_size()
        pos = (16 + 16,
               (surf_size[1] - text_size[1]) / 2)
        surf.blit(text, pos)
        surf.blit(self.imgs['bonus'][name], (0, (surf_size[1] - 16) / 2))
        return surf

    def update_teams(self, game_state):
        if game_state is None:
            return
        self.game_state = game_state

        self.surface.fill((0, 0, 0))
        # For each player
        for i in xrange(2):
            hshift = 8 + i * (8 + 80)
            team = self.game_state.teams[i]
            vshift = 0

            # Header
            trainee = self.imgs['trainee_moto']['off']
            def get_img(pattern):
                return trainee[pattern][i]
            self.surface.blit(get_img(game.RIGHT),
                              (hshift + 0, vshift))
            self.surface.blit(get_img(game.LEFT | game.RIGHT),
                              (hshift + 16, vshift))
            self.surface.blit(get_img(game.LEFT | game.RIGHT),
                              (hshift + 32, vshift))
            self.surface.blit(get_img(game.LEFT | game.RIGHT),
                              (hshift + 48, vshift))
            self.surface.blit(get_img(game.LEFT),
                              (hshift + 64, vshift))
            self.surface.blit(self.imgs['moto']['left'][i],
                              (hshift + 32, vshift))
            vshift += 16

            # Scores
            score = self.get_score(team.score)
            self.surface.blit(score, (hshift, vshift))
            vshift += score.get_size()[1] + 16

            hshift += 16
            # Bonus
            bonus_stat = team.count_bonus()
            bonus = self.get_bonus('plus_long', bonus_stat['plus_long'])
            self.surface.blit(bonus, (hshift, vshift))
            vshift += bonus.get_size()[1]

            bonus = self.get_bonus('plus_pa', bonus_stat['plus_pa'])
            self.surface.blit(bonus, (hshift, vshift))
            vshift += bonus.get_size()[1]

            bonus = self.get_bonus('bonus_croisement', 
                                   bonus_stat['bonus_croisement'])
            self.surface.blit(bonus, (hshift, vshift))
            vshift += bonus.get_size()[1]

            bonus = self.get_bonus('bonus_regeneration',
                                   bonus_stat['bonus_croisement'])
            self.surface.blit(bonus, (hshift, vshift))
            vshift += bonus.get_size()[1]

        vshift += 32
        but = self.make_button('H', self.font2)
        self.surface.blit(but, (8, vshift))
        text = self.font2.render(u'Afficher/Cacher l’aide', True, (255, 255, 255))
        pos = (
            40,
            vshift + (but.get_size()[1] - text.get_size()[1]) / 2
            )
        self.surface.blit(text, pos)
