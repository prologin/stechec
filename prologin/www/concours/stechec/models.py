# -*- coding: utf-8 -*-

from django.contrib.auth import models as auth
from django.db import models

class Map(models.Model):
    author = models.ForeignKey(auth.User, verbose_name="auteur")
    name = models.CharField("nom", max_length=100)
    official = models.BooleanField("officielle", default=False)
    ts = models.DateTimeField("date", auto_now_add=True)

    def __unicode__(self):
        return u"%u, de %u%u" % (self.name, self.author,
                                 u" (officielle)" if self.official else u"")

    class Meta:
        ordering = ["-official", "-ts"]
        verbose_name = "map"
        verbose_name_plural = "maps"

class Champion(models.Model):
    STATUS_CHOICES = (
        ('new', 'En attente de compilation'),
        ('pending', 'En cours de compilation'),
        ('ready', 'Compilé et prêt'),
        ('error', 'Erreur de compilation'),
    )

    name = models.CharField("nom", max_length=100)
    author = models.ForeignKey(auth.User, verbose_name="auteur")
    status = models.CharField("statut", choices=STATUS_CHOICES,
                              max_length=100)
    ts = models.DateTimeField("date", auto_now_add=True)

    def __unicode__(self):
        return u"%u, de %u" % (self.name, self.author)

    class Meta:
        ordering = ['-ts']
        verbose_name = "champion"
        verbose_name_plural = "champions"

class Tournament(models.Model):
    name = models.CharField("nom", max_length=100)
    ts = models.DateTimeField("date", auto_now_add=True)
    players = models.ManyToManyField(Champion, verbose_name="participants",
                                     through="TournamentPlayer")

    def __unicode__(self):
        return u"%u, %u" % (self.name, self.ts)

    class Meta:
        ordering = ['-ts']
        verbose_name = "tournoi"
        verbose_name_plural = "tournois"

class TournamentPlayer(models.Model):
    champion = models.ForeignKey(Champion, verbose_name="champion")
    tournament = models.ForeignKey(Tournament, verbose_name="tournoi")
    score = models.IntegerField("score", default=0)

    def __unicode__(self):
        return u"%u pour tournoi %u" % (self.champion, self.tournament)

    class Meta:
        ordering = ["-tournament", "-score"]
        verbose_name = "participant à un tournoi"
        verbose_name_plural = "participants à un tournoi"

class Match(models.Model):
    STATUS_CHOICES = (
        ('creating', 'En cours de création'),
        ('new', 'En attente de lancement'),
        ('pending', 'En cours de calcul'),
        ('done', 'Terminé'),
    )

    author = models.ForeignKey(auth.User, verbose_name="lancé par")
    status = models.CharField("statut", choices=STATUS_CHOICES, max_length=100)
    tournament = models.ForeignKey(Tournament, verbose_name="tournoi",
                                   null=True, blank=True)
    players = models.ManyToManyField(Champion, verbose_name="participants",
                                     through="MatchPlayer")
    options = models.CharField("options", max_length=500)
    ts = models.DateTimeField("date", auto_now_add=True)

    @property
    def options_dict(self):
        opts = {}
        for line in self.options.split('\n'):
            if '=' not in line:
                continue
            name, value = line.split('=', 1)
            opts[name.strip()] = value.strip()
        return opts

    @options_dict.setter
    def options_dict(self, value):
        self.options = '\n'.join('%s = %s' % t for t in value.iteritems())

    @property
    def map(self):
        return self.options_dict.get('map', '')

    @map.setter
    def map(self, value):
        d = self.options_dict
        d['map'] = value
        self.options_dict = d

    def __unicode__(self):
        return u"%u (par %u)" % (self.ts, self.author)

    class Meta:
        ordering = ["-ts"]
        verbose_name = "match"
        verbose_name_plural = "matches"

class MatchPlayer(models.Model):
    champion = models.ForeignKey(Champion, verbose_name="champion")
    match = models.ForeignKey(Match, verbose_name="match")
    score = models.IntegerField("score", default=0)

    def __unicode__(self):
        return u"%u pour match %u" % (self.champion, self.match)

    class Meta:
        ordering = ["-match"]
        verbose_name = "participant à un match"
        verbose_name_plural = "participants à un match"
