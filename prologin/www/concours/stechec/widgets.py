#!/usr/bin/env python
#-*- coding: utf-8 -*-

from django.forms.widgets import Select
from django.utils.encoding import force_unicode
from django.utils.html import escape, conditional_escape
from itertools import chain

class MapSelect(Select):
    def render_options(self, choices, selected_choices):
        def render_option(map):
            titre = force_unicode(map.titre)
            officielle = map.officielle
            attrs = (force_unicode(map.id) in selected_choices) and u' selected="selected"' or '' 
            if officielle:
                attrs += u' class="award"'

            return u'<option value="%d"%s>%s</option>' % (
                map.id, attrs,
                conditional_escape(titre))
        # Normalize to strings.
        selected_choices = set([force_unicode(v) for v in selected_choices])
        output = []
        for auteur, maps in chain(self.choices, choices):
            output.append(u'<optgroup label="%s">' % escape(force_unicode(auteur)))
            for map_id, map in maps:
                output.append(render_option(map))
            output.append(u'</optgroup>')
        return u'\n'.join(output)
