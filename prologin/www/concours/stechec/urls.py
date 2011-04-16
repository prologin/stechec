from django.conf.urls.defaults import patterns, include, url
from django.contrib.auth.decorators import login_required
from django.contrib.auth import views as auth_views
from django.views.generic import TemplateView

urlpatterns = patterns('concours.stechec',
    url(r'^$', login_required(TemplateView.as_view(template_name='index.html')), name="index"),
    url(r'^login/$', auth_views.login, { 'template_name': 'login.html' }, name="login"),
)
