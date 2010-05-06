#!/usr/bin/env python
#-*- encoding: utf-8 -*-
"""
urls.py
URL patterns for the stechec app.

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

from django.conf.urls.defaults import *
from concours.stechec import views, models

urlpatterns = patterns('',
    (r'^$', views.index),
    (r'^login/?$', views.login),
    (r'^logout/?$', views.logout),
    (r'^password/change/?$', views.change_password),
    (r'^status/?$', views.status),
    (r'^champions/all/?$', views.all_champions),
    (r'^champions/my/?$', views.my_champions),
    (r'^champions/upload/?$', views.new_champion),
    (r'^champions/edit/(\d+)/?$', views.edit_champion),
    (r'^champions/delete/(\d+)/?$', views.delete_champion),
    (r'^champions/(\d+)/?$', views.detail_champion),
    (r'^matches/new/?$', views.new_match),
    (r'^matches/all/?$', views.all_matches),
    (r'^matches/my/?$', views.my_matches),
    (r'^matches/(\d+)/?$', views.detail_match),
    (r'^matches/(\d+)/view/?$', views.viewer_watch),
    (r'^viewer/json/(\d+)/(\d+)/?$', views.viewer_json),
    (r'^maps/all/?$', views.all_maps),
    (r'^maps/(\d+)/?$', views.detail_map),
    (r'^editeur/?$', views.editeur),
    (r'^editeur/listmaps/?$', views.editeur_listmaps),
    (r'^editeur/save/?$', views.editeur_save),
    (r'^editeur/open/(\d+)/?$', views.editeur_open),
    (r'^tournois/all/?$', views.all_tournois),
    (r'^tournois/(\d+)/?$', views.detail_tournoi),
)
