import os
import sys

sys.path.insert(0, '/srv/http')
os.environ['DJANGO_SETTINGS_MODULE'] = 'concours.settings'

import django.core.handlers.wsgi
application = django.core.handlers.wsgi.WSGIHandler()
