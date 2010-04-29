#!/usr/bin/env python
#-*- encoding: utf-8 -*-
"""
models.py
Models for the Stechec website.

Copyright (c) 2008 Pierre "delroth" Bourdon <root@delroth.is-a-geek.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from django.db import models
from django.contrib.auth import models as authmodels
from django.contrib import admin
from concours.stechec.constants import *
import os

class News(models.Model):
    auteur = models.ForeignKey(authmodels.User)
    titre = models.CharField(max_length=256)
    texte = models.TextField()
    date = models.DateTimeField()
    visible = models.BooleanField()

    def __unicode__(self):
        return self.titre

    def get_absolute_url(self):
        return '/news/%d/' % self.id

    class Meta:
        verbose_name_plural = "news"
        ordering = ['-date']

admin.site.register(News)

class Map(models.Model):
    auteur = models.ForeignKey(authmodels.User)
    titre = models.CharField(max_length=256)
    officielle = models.BooleanField(default=False)

    def __unicode__(self):
        return u'%s (de %s)' % (self.titre, self.auteur)

    def delete(self):
        from concours.settings import MAP_PATH
        from os import unlink
        
        try:
            os.unlink(MAP_PATH % self.id)
        except OSError:
            pass

        super(Map, self).delete()

    def get_raw(self):
        from concours.settings import MAP_PATH
        return file(MAP_PATH % self.id).read()

    def set_raw(self, value):
        from concours.settings import MAP_PATH, MAP_WIDTH, MAP_HEIGHT
        from os import chmod
        
        lines = value.split('\n')
        clean_map = []

        for y in xrange(MAP_HEIGHT):
            line = ''
            for x in xrange(MAP_WIDTH):
                try:
                    cell = lines[y][x]
                except KeyError:
                    cell = ' '

                if cell not in 'ABCD$#+ ':
                    cell = ' '

                line += cell
            clean_map.append(line)
    
        fd = file(MAP_PATH % self.id, 'w')
        fd.write('\n'.join(clean_map))
        fd.close()

        chmod(MAP_PATH % self.id, 0664)

    raw = property(get_raw, set_raw)

    @property
    def html(self):
        from concours.stechec.utils import map_to_html
        return map_to_html(self.raw)

    def get_absolute_url(self):
        return '/maps/%d/' % self.id

    class Meta:
        verbose_name = 'carte'
        ordering = ['-officielle', 'id']

admin.site.register(Map)

class Champion(models.Model):
    auteur = models.ForeignKey(authmodels.User)
    supprime = models.BooleanField(default=False, verbose_name=u'Supprimé')
    nom = models.CharField(max_length=64)
    comment = models.TextField(default='')
    statut = models.IntegerField()
    matches = models.ManyToManyField('Match', through='Competiteur')
    position = models.IntegerField(default=-1)

    def __unicode__(self):
        return u'%s (de %s)' % (
            self.nom, self.auteur
        ) + (u' [supprimé]' if self.supprime else '')

    def get_absolute_url(self):
        return '/champions/%d/' % self.id


    @property
    def statut_string(self):
        return STATUTS_CHAMPIONS[self.statut][0]

    @property
    def statut_class(self):
        return STATUTS_CHAMPIONS[self.statut][1]

    @property
    def log_compilation(self):
        from concours.settings import CHAMPION_LOG_PATH
        
        try:
            fd = file(CHAMPION_LOG_PATH % (self.auteur.id, self.id))
            return fd.read()
        except:
            return ''

admin.site.register(Champion)

class Tournoi(models.Model):
    pass

class Match(models.Model):
    createur = models.ForeignKey(authmodels.User)
    map = models.ForeignKey(Map, null=True)
    tournoi = models.ForeignKey(Tournoi, null=True)
    statut = models.IntegerField()
    date = models.DateTimeField(auto_now_add=True)
    champions = models.ManyToManyField(Champion, through='Competiteur')

    def delete(self):
        from shutil import rmtree
        from os import unlink
        from concours.settings import MATCH_DIR, VIEWER_PATH
        
        try:
            unlink(VIEWER_PATH % self.id)
        except OSError:
            pass
        
        rmtree(MATCH_DIR % self.id, ignore_errors=True)

        super(Match, self).delete()
    
    def __unicode__(self):
        return u'Match de %s daté du %s' % (
            self.createur, self.date
        )

    def get_absolute_url(self):
        return '/matches/%d/' % self.id

    @property
    def statut_string(self):
        return STATUTS_MATCHES[self.statut][0]

    @property
    def statut_class(self):
        return STATUTS_MATCHES[self.statut][1]
    
    @property
    def is_termine(self):
        return self.statut == STATUT_MATCH_TERMINE

    @property
    def log(self):
        from concours.settings import SERVER_LOG_PATH
        
        try:
            fd = file(SERVER_LOG_PATH % (self.id))
            return fd.read()
        except:
            return ''

    class Meta:
        verbose_name_plural = "matches"
        ordering = ['statut', '-id']

admin.site.register(Match)

class Competiteur(models.Model):
    match = models.ForeignKey(Match)
    champion = models.ForeignKey(Champion)
    score = models.IntegerField(default=0)
    max_score = None

    def __unicode__(self):
        return u'Match %d, champion %s' % (
            self.match.id, unicode(self.champion)
        )

    @property
    def log(self):
        from concours.settings import COMPETITEUR_LOG_PATH
        try:
            fd = file(COMPETITEUR_LOG_PATH % (self.match.id, self.id))
            return fd.read()
        except:
            return ''

    class Meta:
        ordering = ['-score']
admin.site.register(Competiteur)

