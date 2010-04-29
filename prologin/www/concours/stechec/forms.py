#!/usr/bin/env python
#-*- encoding: utf-8 -*-
"""
forms.py
Forms for Stechec.

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

from django import forms
from django.forms.util import ValidationError
from concours.stechec.models import Champion, Map
from concours.stechec.constants import *
from concours.stechec.widgets import MapSelect

class ChampionUploadForm(forms.Form):
    nom = forms.CharField(max_length=64, required=True)
    tarball = forms.FileField(required=True, label='Archive (.tgz)')
    commentaire = forms.CharField(required=True, widget=forms.widgets.Textarea())

class ChampionField(forms.Field):
    default_error_messages = {
            'notfound' : u"Ce champion n'existe pas ou n'est pas compilé.",
            'invalid' : u"Vous devez entrer un numéro de champion valide."
    }

    def __init__(self, *args, **kwargs):
        super(ChampionField, self).__init__(*args, **kwargs)

    def clean(self, value):
        super(ChampionField, self).clean(value)
        
        try:
            return Champion.objects.get(
                id=int(value.strip()),
                supprime=False,
                statut=STATUT_CHAMPION_VALIDE)

        except ValueError:
            raise ValidationError(self.error_messages['invalid'])
        
        except Champion.DoesNotExist:
            raise ValidationError(self.error_messages['notfound'])

class CreateMatchForm(forms.Form):
    champion_1 = ChampionField(label='Premier champion')
    champion_2 = ChampionField(label='Deuxième champion')
