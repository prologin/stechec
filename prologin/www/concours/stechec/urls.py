from concours.stechec import views
from django.conf.urls.defaults import patterns, include, url
from django.contrib.auth.decorators import login_required
from django.contrib.auth import views as auth_views
from django.views.generic import TemplateView

urlpatterns = patterns('concours.stechec',
    url(r'^$', login_required(TemplateView.as_view(template_name='index.html')), name="index"),
    url(r'^login/$', auth_views.login, { 'template_name': 'login.html' }, name="login"),
    url(r'^logout/$', auth_views.logout, { 'next_page': '/' }, name="logout"),

    url(r'^champions/(?P<pk>\d+)/$', views.ChampionView.as_view(), name="champion-detail"),
    url(r'^champions/all/$', views.AllChampionsView.as_view(), name="champions-all"),
    url(r'^champions/my/$', login_required(views.MyChampionsView.as_view()), name="champions-my"),
    url(r'^champions/new/$', login_required(views.new_champion), name="champion-new"),
)
