///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#include "interface.hh"

static PyObject* c_module;
static PyObject* py_module;
static PyObject* champ_module;

static void _init_python();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
PyObject* cxx2lang<PyObject*, int>(int in)
{
  return PyInt_FromLong(in);
}

template <>
PyObject* cxx2lang<PyObject*, bool>(bool in)
{
  return PyBool_FromLong(in);
}

template <typename Cxx>
PyObject* cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  PyObject* out = PyList_New(size);

  for (unsigned int i = 0; i < size; ++i)
    PyList_SET_ITEM(out, i, (cxx2lang<PyObject*, Cxx>(in[i])));

  return out;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<PyObject*, int>(PyObject* in)
{
  long out = PyInt_AsLong(in);
  if (out == -1)
    if (PyErr_Occurred())
    {
      throw 42;
    }

  return out;
}

template <>
bool lang2cxx<PyObject*, bool>(PyObject* in)
{
  return (bool)lang2cxx<PyObject*, int>(in);
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(PyObject* in)
{
  if (!PyList_Check(in))
  {
    PyErr_SetString(PyExc_TypeError, "a list is required");
    throw 42;
  }

  std::vector<Cxx> out;
  unsigned int size = PyList_Size(in);

  for (unsigned int i = 0; i < size; ++i)
    out.push_back(lang2cxx<PyObject*, Cxx>(PyList_GET_ITEM(in, i)));

  return out;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template <>
PyObject* cxx2lang<PyObject*, erreur>(erreur in)
{
  return cxx2lang<PyObject*, int>((int)in);
}

template <>
erreur lang2cxx<PyObject*, erreur>(PyObject* in)
{
  return (erreur)lang2cxx<PyObject*, int>(in);
}

///
// Le type d'une unité sur le terrain
//
template <>
PyObject* cxx2lang<PyObject*, type_unite>(type_unite in)
{
  return cxx2lang<PyObject*, int>((int)in);
}

template <>
type_unite lang2cxx<PyObject*, type_unite>(PyObject* in)
{
  return (type_unite)lang2cxx<PyObject*, int>(in);
}

///
// Représente une position sur le terrain du jeu.
//
template <>
PyObject* cxx2lang<PyObject*, position>(position in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.x)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.y)));
  PyObject* name = PyString_FromString("position");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
position lang2cxx<PyObject*, position>(PyObject* in)
{
  position out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.x = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.y = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  return out;
}

///
// Contient les informations sur la taille du terrain du jeu.
//
template <>
PyObject* cxx2lang<PyObject*, taille_terrain>(taille_terrain in)
{
  PyObject* tuple = PyTuple_New(3);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.taille)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.min_coord)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.max_coord)));
  PyObject* name = PyString_FromString("taille_terrain");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
taille_terrain lang2cxx<PyObject*, taille_terrain>(PyObject* in)
{
  taille_terrain out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.taille = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.min_coord = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.max_coord = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
PyObject* cxx2lang<PyObject*, caracs>(caracs in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.pa_init)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.portee)));
  PyObject* name = PyString_FromString("caracs");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
caracs lang2cxx<PyObject*, caracs>(PyObject* in)
{
  caracs out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pa_init = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.portee = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  return out;
}

///
// Représente une unité sur le terrain.
//
template <>
PyObject* cxx2lang<PyObject*, unite>(unite in)
{
  PyObject* tuple = PyTuple_New(9);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, position>(in.pos)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, bool>(in.ennemi)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, type_unite>(in.type_unite_actuel)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, type_unite>(in.vrai_type_unite)));
  PyTuple_SET_ITEM(tuple, 4, (cxx2lang<PyObject*, int>(in.ko)));
  PyTuple_SET_ITEM(tuple, 5, (cxx2lang<PyObject*, int>(in.pa)));
  PyTuple_SET_ITEM(tuple, 6, (cxx2lang<PyObject*, int>(in.attaques)));
  PyTuple_SET_ITEM(tuple, 7, (cxx2lang<PyObject*, int>(in.attaques_gratuites)));
  PyTuple_SET_ITEM(tuple, 8, (cxx2lang<PyObject*, int>(in.id)));
  PyObject* name = PyString_FromString("unite");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
unite lang2cxx<PyObject*, unite>(PyObject* in)
{
  unite out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pos = lang2cxx<PyObject*, position>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.ennemi = lang2cxx<PyObject*, bool>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.type_unite_actuel = lang2cxx<PyObject*, type_unite>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.vrai_type_unite = lang2cxx<PyObject*, type_unite>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(4);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.ko = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(5);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pa = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(6);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.attaques = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(7);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.attaques_gratuites = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(8);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
PyObject* cxx2lang<PyObject*, cartes>(cartes in)
{
  PyObject* tuple = PyTuple_New(4);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.potion)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.deguisement)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.banzai)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.pacifisme)));
  PyObject* name = PyString_FromString("cartes");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
cartes lang2cxx<PyObject*, cartes>(PyObject* in)
{
  cartes out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.potion = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.deguisement = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.banzai = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pacifisme = lang2cxx<PyObject*, int>(PyObject_GetItem(in, i));
  Py_DECREF(i);
  return out;
}

///
// Renvoie le nombre de points de commandements.
//
static PyObject* p_nombre_pc(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nombre_pc());
  } catch (...) { return NULL; }
}

///
// Renvoie le nombre d'unités en jeu.
//
static PyObject* p_nombre_unites(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nombre_unites(lang2cxx<PyObject*, bool>(a0)));
  } catch (...) { return NULL; }
}

///
// Renvoie le numéro du tour actuel.
//
static PyObject* p_tour_actuel(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_tour_actuel());
  } catch (...) { return NULL; }
}

///
// Renvoie la position du spawn (ennemi ou non).
//
static PyObject* p_pos_renfort(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, position>(api_pos_renfort(lang2cxx<PyObject*, bool>(a0)));
  } catch (...) { return NULL; }
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
static PyObject* p_caracteristiques(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, caracs>(api_caracteristiques(lang2cxx<PyObject*, type_unite>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
static PyObject* p_mes_cartes(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, cartes>(api_mes_cartes());
  } catch (...) { return NULL; }
}

///
// Retourne la liste des unités actuellement en jeu.
//
static PyObject* p_unites(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_unites());
  } catch (...) { return NULL; }
}

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
static PyObject* p_taille_terrain_actuelle(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, taille_terrain>(api_taille_terrain_actuelle());
  } catch (...) { return NULL; }
}

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
static PyObject* p_potion(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_potion(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
static PyObject* p_deguisement(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_deguisement(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, type_unite>(a1)));
  } catch (...) { return NULL; }
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
static PyObject* p_banzai(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_banzai(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
static PyObject* p_pacifisme(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_pacifisme());
  } catch (...) { return NULL; }
}

///
// Déplace une unité vers une position à portée.
//
static PyObject* p_deplacer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_deplacer(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Relève une unité n'ayant plus de marqueurs de KO.
//
static PyObject* p_relever(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_relever(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Attaque une autre unité.
//
static PyObject* p_attaquer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_attaquer(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Fait apparaitre une unité sur la case de spawn.
//
static PyObject* p_renfort(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_renfort(lang2cxx<PyObject*, type_unite>(a0)));
  } catch (...) { return NULL; }
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
static PyObject* p_annuler(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_annuler());
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type erreur
//
static PyObject* p_afficher_erreur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_erreur(lang2cxx<PyObject*, erreur>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type type_unite
//
static PyObject* p_afficher_type_unite(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_type_unite(lang2cxx<PyObject*, type_unite>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type position
//
static PyObject* p_afficher_position(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_position(lang2cxx<PyObject*, position>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type taille_terrain
//
static PyObject* p_afficher_taille_terrain(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_taille_terrain(lang2cxx<PyObject*, taille_terrain>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type caracs
//
static PyObject* p_afficher_caracs(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_caracs(lang2cxx<PyObject*, caracs>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type unite
//
static PyObject* p_afficher_unite(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_unite(lang2cxx<PyObject*, unite>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type cartes
//
static PyObject* p_afficher_cartes(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_cartes(lang2cxx<PyObject*, cartes>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"nombre_pc", p_nombre_pc, METH_VARARGS, "nombre_pc"},
  {"nombre_unites", p_nombre_unites, METH_VARARGS, "nombre_unites"},
  {"tour_actuel", p_tour_actuel, METH_VARARGS, "tour_actuel"},
  {"pos_renfort", p_pos_renfort, METH_VARARGS, "pos_renfort"},
  {"caracteristiques", p_caracteristiques, METH_VARARGS, "caracteristiques"},
  {"mes_cartes", p_mes_cartes, METH_VARARGS, "mes_cartes"},
  {"unites", p_unites, METH_VARARGS, "unites"},
  {"taille_terrain_actuelle", p_taille_terrain_actuelle, METH_VARARGS, "taille_terrain_actuelle"},
  {"potion", p_potion, METH_VARARGS, "potion"},
  {"deguisement", p_deguisement, METH_VARARGS, "deguisement"},
  {"banzai", p_banzai, METH_VARARGS, "banzai"},
  {"pacifisme", p_pacifisme, METH_VARARGS, "pacifisme"},
  {"deplacer", p_deplacer, METH_VARARGS, "deplacer"},
  {"relever", p_relever, METH_VARARGS, "relever"},
  {"attaquer", p_attaquer, METH_VARARGS, "attaquer"},
  {"renfort", p_renfort, METH_VARARGS, "renfort"},
  {"annuler", p_annuler, METH_VARARGS, "annuler"},
  {"afficher_erreur", p_afficher_erreur, METH_VARARGS, "afficher_erreur"},
  {"afficher_type_unite", p_afficher_type_unite, METH_VARARGS, "afficher_type_unite"},
  {"afficher_position", p_afficher_position, METH_VARARGS, "afficher_position"},
  {"afficher_taille_terrain", p_afficher_taille_terrain, METH_VARARGS, "afficher_taille_terrain"},
  {"afficher_caracs", p_afficher_caracs, METH_VARARGS, "afficher_caracs"},
  {"afficher_unite", p_afficher_unite, METH_VARARGS, "afficher_unite"},
  {"afficher_cartes", p_afficher_cartes, METH_VARARGS, "afficher_cartes"},
  {NULL, NULL, 0, NULL}
};

static void _initapi()
{
  c_module = Py_InitModule("_api", api_callback);
}

/*
** Inititialize python, register API functions,
** and load .py file
*/
static void _init_python()
{
  PyObject* name;
  const char* champion_path;
      
  champion_path = getenv("CHAMPION_PATH");
  if (champion_path == NULL)
    champion_path = ".";

  setenv("PYTHONPATH", champion_path, 1);

  static char program_name[] = "stechec";
  Py_SetProgramName(program_name);
  Py_Initialize();
  _initapi();

  name = PyString_FromString("prologin");
  champ_module = PyImport_Import(name);
  Py_DECREF(name);
  if (champ_module == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }

  name = PyString_FromString("api");
  py_module = PyImport_Import(name);
  Py_DECREF(name);
  if (py_module == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }
}

/*
** Run a python function.
*/
static PyObject* _call_python_function(const char* name)
{
  static bool initialized = false;

  if (!initialized)
  {
    initialized = true;
    _init_python();
  }

  PyObject *arglist, *func;
  PyObject *result = NULL;
  
  func = PyObject_GetAttrString(champ_module, (char*)name);
  if (func && PyCallable_Check(func))
  {
    arglist = Py_BuildValue("()");
    result = PyEval_CallObject(func, arglist);
    Py_XDECREF(arglist);
    Py_DECREF(func);
  }
  if (result == NULL && PyErr_Occurred())
    PyErr_Print();

  return result;
}

/*
** Functions called from stechec to C.
*/
extern "C" void init_game()
{
  PyObject* _retval = _call_python_function("init_game");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}


extern "C" position retirer_ko()
{
  PyObject* _retval = _call_python_function("retirer_ko");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  position ret = lang2cxx<PyObject*, position>(_retval);
  Py_XDECREF(_retval);
  return ret;
  } catch (...) { PyErr_Print(); abort(); }
}


extern "C" void jouer()
{
  PyObject* _retval = _call_python_function("jouer");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}


extern "C" void end_game()
{
  PyObject* _retval = _call_python_function("end_game");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}


