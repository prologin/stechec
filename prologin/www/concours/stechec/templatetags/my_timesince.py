#!/usr/bin/env python
#-*- encoding: utf-8 -*-

from django import template
register = template.Library()

def _timesince(d, now=None):
    import datetime
    from django.utils.tzinfo import LocalTimezone
    from django.utils.translation import ungettext, ugettext

    chunks = (
      (60 * 60 * 24 * 365, lambda n: ungettext('year', 'years', n)),
      (60 * 60 * 24 * 30, lambda n: ungettext('month', 'months', n)),
      (60 * 60 * 24 * 7, lambda n : ungettext('week', 'weeks', n)),
      (60 * 60 * 24, lambda n : ungettext('day', 'days', n)),
      (60 * 60, lambda n: ungettext('hour', 'hours', n)),
      (60, lambda n: ungettext('minute', 'minutes', n)),
      (1, lambda n: 'seconde' if n < 2 else 'secondes')
    )
    # Convert datetime.date to datetime.datetime for comparison.
    if not isinstance(d, datetime.datetime):
        d = datetime.datetime(d.year, d.month, d.day)
    if now and not isinstance(now, datetime.datetime):
        now = datetime.datetime(now.year, now.month, now.day)

    if not now:
        if d.tzinfo:
            now = datetime.datetime.now(LocalTimezone(d))
        else:
            now = datetime.datetime.now()

    # ignore microsecond part of 'd' since we removed it from 'now'
    delta = now - (d - datetime.timedelta(0, 0, d.microsecond))
    since = delta.days * 24 * 60 * 60 + delta.seconds
    if since <= 0:
        # d is in the future compared to now, stop processing.
        return u'0 ' + ugettext('seconde')
    
    s = ''

    for i, (seconds, name) in enumerate(chunks):
        count = since // seconds
        if count != 0:
            s += ugettext('%(number)d %(type)s, ') % {'number': count, 'type': name(count)}
            since -= count * seconds
    
    return s[:-2]

@register.filter
def my_timesince(value, arg=None):
    """Formats a date as the time since that date (i.e. "4 days, 6 hours")."""
    if not value:
        return u''
    try:
        if arg:
            return _timesince(value, arg)
        return _timesince(value)
    except (ValueError, TypeError):
        return u''
my_timesince.is_safe = False

