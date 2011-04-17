from concours.stechec import models
from django.views.generic import DetailView, ListView

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
        context['explanation_text'] = self.explanation_text
        return context

class AllChampionsView(ChampionsListView):
    queryset = models.Champion.objects.filter(deleted=False)
    explanation_text = 'Voici la liste de tous les champions participants actuellement.'

class MyChampionsView(ChampionsListView):
    explanation_text = 'Voici la liste de tous vos champions participants actuellement.'
    def get_queryset(self):
        user = self.request.user
        return models.Champion.objects.filter(deleted=False, author=user)
