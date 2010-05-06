#!/usr/bin/env python
#-*- encoding: utf-8 -*-
"""
views.py
Views for the website.

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

from concours.settings import DEBUG, BASE_DIR, VIEWER_PATH
from concours.stechec.forms import ChampionUploadForm, CreateMatchForm
from concours.stechec.models import Champion, Competiteur, Match, Map, Tournoi
from django.contrib.auth import login as _login, logout as _logout
from django.contrib.auth.forms import AuthenticationForm, PasswordChangeForm
from django.contrib.auth.models import User
from django.core.mail import send_mail
from django.http import Http404, HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response as _render_to_response
from django.shortcuts import get_object_or_404
from django.template import RequestContext
from django.template.loader import render_to_string
from django.views.generic import list_detail
from django.db.models import Max, Min
from concours.stechec.constants import *
import os
import os.path
import sys

def generate_password():
    from random import shuffle

    passwd = list(PASSWD_CHARSET)
    shuffle(passwd)
    return ''.join(passwd[:8])

def login_required(func):
    def wrapper(request, *args, **kwargs):
        if not request.user.is_authenticated():
            return HttpResponseRedirect('/login/')
        else:
            return func(request, *args, **kwargs)
    return wrapper

def render_to_response(template):
    request = sys._getframe().f_back.f_locals['request']
    vars = sys._getframe().f_back.f_locals
    return _render_to_response(template, vars,
            context_instance=RequestContext(request))

@login_required
def index(request):
    return render_to_response('index.html')

def login(request):
    if request.user.is_authenticated():
        return HttpResponseRedirect('/')
    if request.method == 'POST':
        form = AuthenticationForm(data=request.POST)
        if form.is_valid():
            _login(request, form.user_cache)
            return HttpResponseRedirect('/')
    else:
        form = AuthenticationForm()
    return render_to_response('login.html')

def status(request):
    from concours.stechec.master import machine_list
    machines = machine_list()
    return render_to_response('machine-list.html')

@login_required
def change_password(request):
    if request.method == 'POST':
        form = PasswordChangeForm(request.user, request.POST)
        if form.is_valid():
            form.save()
            return render_to_response('password-change-success.html')
    else:
        form = PasswordChangeForm(request.user)
    return render_to_response('password-change.html')

@login_required
def logout(request):
    _logout(request)
    return render_to_response('logout-success.html')

def all_champions(request):
    return list_detail.object_list(
        request,
        queryset=Champion.objects.filter(
            supprime=False,
            statut=STATUT_CHAMPION_VALIDE),
        template_object_name='champion',
        template_name='all-champions.html',
        paginate_by=25
    )

@login_required
def my_champions(request):
    return list_detail.object_list(
        request,
        queryset=Champion.objects.filter(auteur=request.user, supprime=False),
        template_object_name='champion',
        template_name='my-champions.html',
        paginate_by=25
    )

def save_champion(request, champion, tarball):
    directory = os.path.join(BASE_DIR, 'private', 'candidat_%d' % request.user.id)
    if not os.path.exists(directory):
        os.mkdir(directory)
    
    try:
        os.chmod(directory, 0777)
    except:
        pass
    
    directory = os.path.join(directory, 'champion_%d' % champion.id)
    if not os.path.exists(directory):
        os.mkdir(directory)
    
    try:
        os.chmod(directory, 0777)
    except:
        pass
    
    filepath = os.path.join(directory, 'champion.tgz')
    file = open(filepath, 'w')
    for chunk in tarball.chunks():
        file.write(chunk)
    file.close()

    try:
        os.chmod(filepath, 0666)
    except:
        pass

@login_required
def new_champion(request):
    if request.method == 'POST':
        form = ChampionUploadForm(request.POST, request.FILES)
        if form.is_valid():
            champion = Champion(
                    nom=form.cleaned_data['nom'],
                    auteur=request.user,
                    comment=form.cleaned_data['commentaire'],
                    statut=STATUT_CHAMPION_NOUVEAU)
            
            champion.save()
            
            try:
                save_champion(request, champion, form.cleaned_data['tarball'])
            except:
                champion.delete()
                raise
            
            return render_to_response('upload-success.html')
    else:
        form = ChampionUploadForm()
    
    return render_to_response('upload-champion.html')

@login_required
def edit_champion(request, champion_id):
    champion = get_object_or_404(Champion, id=champion_id, auteur=request.user)

    if request.method == 'POST':
        form = ChampionUploadForm(request.POST, request.FILES)
        
        if form.is_valid():
            champion.statut = STATUT_CHAMPION_NOUVEAU
            champion.nom = form.cleaned_data['nom']
            champion.comment = form.cleaned_data['commentaire']

            save_champion(request, champion, form.cleaned_data['tarball'])

            champion.save()
            
            return render_to_response('upload-success.html')
    else:
        form = ChampionUploadForm(initial={'nom' : champion.nom, 'commentaire' : champion.comment})

    return render_to_response('upload-champion.html')

@login_required
def delete_champion(request, champion_id):
    champion = get_object_or_404(Champion, id=champion_id, supprime=False, auteur=request.user)

    if request.method == 'POST':
        champion.supprime = True
        champion.save()

        return render_to_response('delete-champion-success.html')
    else:
        return render_to_response('delete-champion.html')

def detail_champion(request, champion_id):
    champion = get_object_or_404(Champion, id=champion_id)
    can_see_log = champion.auteur == request.user or request.user.is_staff
    return render_to_response('detail-champion.html')

@login_required
def new_match(request):
    from itertools import groupby
    if request.method == 'POST':
        form = CreateMatchForm(request.POST)
    else:
        form = CreateMatchForm()
    
    if form.is_valid():
        match = Match(
                statut=STATUT_MATCH_INDEFINI,
                createur=request.user)
        
        match.save()

        for i in xrange(1, 3):
            comp = Competiteur(champion=form.cleaned_data['champion_%d' % i], match=match)
            comp.save()
        
        match.statut = STATUT_MATCH_NOUVEAU
        match.save()

        return render_to_response('new-match-success.html')
    else:
        return render_to_response('new-match.html')

def all_matches(request):
    from constants import *
    return list_detail.object_list(
            request,
            queryset=Match.objects.all(),
            template_object_name='match',
            template_name='all-matches.html',
            paginate_by=250
    )

@login_required
def my_matches(request):
    from constants import *
    return list_detail.object_list(
            request,
            queryset=Match.objects.filter(createur=request.user),
            template_object_name='match',
            template_name='my-matches.html',
            paginate_by=25
    )

def detail_match(request, match_id):
    try:
        match = Match.objects.annotate(Max('competiteur__score')).annotate(Min('competiteur__id')).get(id=int(match_id))
    except (Match.DoesNotExist, ValueError):
        raise Http404

    return render_to_response('detail-match.html')

def all_maps(request):
    return list_detail.object_list(
        request,
        queryset=Map.objects.all(),
        template_object_name='map',
        template_name='all-maps.html',
        paginate_by=25
    )

def detail_map(request, map_id):
    map = get_object_or_404(Map, id=map_id)
    return render_to_response('detail-map.html')

def viewer_watch(request, match_id):
    match = get_object_or_404(Match, id=match_id)
    try:
        match_id = int(match_id)
        fd = file(VIEWER_PATH % match_id)
    except:
        raise Http404

    competiteurs = match.competiteur_set.order_by('id')
    
    fd.readline()
    initial_frame = fd.readline().strip()
    turn_count = fd.read().count('\n') + 1
    return render_to_response('viewer-watch.html')

def viewer_json(request, match_id, turn_id):
    try:
        match_id = int(match_id)
        turn_id = int(turn_id)
        
        json = os.path.join(VIEWER_PATH % match_id)
        fd = file(json)
    
    except Exception, e:
        raise Http404
    
    if turn_id < 0:
        raise Http404

    fd.readline() # timestamp

    for i in xrange(turn_id):
        fd.readline()
    
    return HttpResponse(fd.readline())

def editeur(request):
    return render_to_response('editeur.html')

def editeur_listmaps(request):
    maps = Map.objects.order_by('auteur__username', 'titre')
    return render_to_response('editeur-listmaps.html')

def editeur_save(request):
    if not request.method == 'POST' or not 'data' in request.POST or not 'title' in request.POST:
        raise Http404

    if not request.user.is_authenticated():
        return HttpResponse(u'Vous devez être inscrit et connecté pour enregistrer une carte sur le site.\n'
                u'Vous pouvez néanmoins enregister votre carte dans un cookie.', mimetype='text/plain')
    
    data = request.POST['data'].upper()
    title = request.POST['title'].strip()

    if not all(_ in data for _ in list(u'ABCD')):
        return HttpResponse(u'Votre carte contient des erreurs et ne peut donc pas être enregistrée.', mimetype='text/plain')
    
    for item in Map.objects.all():
        try:
            if item.raw == data:
                return HttpResponse(u'Une carte similaire, « %s », a déjà été enregistrée par %s.' % (item.titre, item.auteur), mimetype='text/plain')
        except:
            pass
        
        if item.titre.lower() == title.lower():
            return HttpResponse(u'Une carte du même titre a déjà été enregistrée par %s.' % (u'vous-même' if item.auteur == request.user else item.auteur), mimetype='text/plain')


    map = Map(titre=title, auteur=request.user)
    map.save()
    map.raw = data

    return HttpResponse(u'Votre carte a bien été enregistrée.', mimetype='text/plain')

def editeur_open(request, map_id):
    return HttpResponse(get_object_or_404(Map, id=map_id).raw, mimetype='text/plain')

def all_tournois(request):
    return list_detail.object_list(
        request,
        queryset=Tournoi.objects.all(),
        template_object_name='tournoi',
        template_name='all-tournois.html',
        paginate_by=25
    )

def detail_tournoi(request, tournoi_id):
    tournoi = get_object_or_404(Tournoi, id=tournoi_id)
    participants = tournoi.participants.order_by('-score')
    return render_to_response('detail-tournoi.html')
