from concours.stechec import forms
from concours.stechec import models
from django.conf import settings
from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.views.generic import DetailView, ListView

import os
import os.path

class ChampionView(DetailView):
    context_object_name = "champion"
    model = models.Champion
    template_name = "champion-detail.html"

    @property
    def can_see_log(self):
        ch = self.get_object()
        return self.request.user == ch.author or self.request.user.is_staff

    def get_context_data(self, **kwargs):
        context = super(ChampionView, self).get_context_data(**kwargs)
        context['can_see_log'] = self.can_see_log
        return context

class ChampionsListView(ListView):
    context_object_name = "champions"
    paginate_by = 50
    template_name = "champions-list.html"

    def get_context_data(self, **kwargs):
        context = super(ChampionsListView, self).get_context_data(**kwargs)
        context['show_for_all'] = self.show_for_all
        context['explanation_text'] = self.explanation_text
        return context

class AllChampionsView(ChampionsListView):
    queryset = models.Champion.objects.filter(deleted=False)
    explanation_text = 'Voici la liste de tous les champions participants actuellement.'
    show_for_all = True

class MyChampionsView(ChampionsListView):
    explanation_text = 'Voici la liste de tous vos champions participants actuellement.'
    show_for_all = False

    def get_queryset(self):
        user = self.request.user
        return models.Champion.objects.filter(deleted=False, author=user)

def new_champion(request):
    if request.method == 'POST':
        form = forms.ChampionUploadForm(request.POST, request.FILES)
        if form.is_valid():
            champion = models.Champion(
                name = form.cleaned_data['name'],
                author = request.user,
                status = 'new',
                comment = form.cleaned_data['comment']
            )
            champion.save()

            os.makedirs(champion.directory)
            fp = open(os.path.join(champion.directory, 'champion.tgz'), 'w')
            for chunk in form.cleaned_data['tarball'].chunks():
                fp.write(chunk)
            fp.close()
            return HttpResponseRedirect(champion.get_absolute_url())
    else:
        form = forms.ChampionUploadForm()

    return render_to_response('champion-new.html', {'form': form},
                              context_instance=RequestContext(request))

def master_status(request):
    status = models.master_status()
    status = [(h, p, (100 * (m - s)) / m) for (h, p, s, m) in status]
    status.sort()
    return render_to_response('master-status.html', {'status': status},
                              context_instance=RequestContext(request))
