from django import forms

class ChampionUploadForm(forms.Form):
    name = forms.CharField(max_length=64, required=True, label="Nom du champion")
    tarball = forms.FileField(required=True, label="Archive des sources (.tgz)")
    comment = forms.CharField(required=True, widget=forms.widgets.Textarea(), label="Commentaire")
