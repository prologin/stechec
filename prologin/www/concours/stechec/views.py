from concours.stechec import models
from django.views.generic import ListView

class AllChampionsView(ListView):
    context_object_name = "champions"
    paginate_by = 50
    queryset = models.Champion.objects.all()
    template_name = "all_champions.html"
