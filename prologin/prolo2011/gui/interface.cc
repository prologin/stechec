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
PyObject* cxx2lang<PyObject*, std::string>(std::string in)
{
return PyString_FromString (in.c_str());
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

template <>
std::string lang2cxx<PyObject*, std::string>(PyObject* in)
{
  char * out = PyString_AS_STRING(in);
  if (PyErr_Occurred())
    {
      throw 42;
    }
  return out;
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
// Énumération représentant une erreur renvoyée par une des fonctions d'action
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
// Énumération représentant les différents types de case
//
template <>
PyObject* cxx2lang<PyObject*, type_case>(type_case in)
{
  return cxx2lang<PyObject*, int>((int)in);
}

template <>
type_case lang2cxx<PyObject*, type_case>(PyObject* in)
{
  return (type_case)lang2cxx<PyObject*, int>(in);
}

///
// Énumération représentant les différents types de bonii
//
template <>
PyObject* cxx2lang<PyObject*, type_bonus>(type_bonus in)
{
  return cxx2lang<PyObject*, int>((int)in);
}

template <>
type_bonus lang2cxx<PyObject*, type_bonus>(PyObject* in)
{
  return (type_bonus)lang2cxx<PyObject*, int>(in);
}

///
// Représente une position sur le terrain du jeu
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
  out.x = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.y = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Caracteristiques d'une source d'énergie
//
template <>
PyObject* cxx2lang<PyObject*, source_energie>(source_energie in)
{
  PyObject* tuple = PyTuple_New(4);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.id)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, position>(in.pos)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.capacite)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.capacite_max)));
  PyObject* name = PyString_FromString("source_energie");
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
source_energie lang2cxx<PyObject*, source_energie>(PyObject* in)
{
  source_energie out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pos = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.capacite = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.capacite_max = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Représente une traînée de moto sur le terrain
//
template <>
PyObject* cxx2lang<PyObject*, trainee_moto>(trainee_moto in)
{
  PyObject* tuple = PyTuple_New(4);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.id)));
  PyTuple_SET_ITEM(tuple, 1, cxx2lang_array(in.emplacement));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.team)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.longueur)));
  PyObject* name = PyString_FromString("trainee_moto");
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
trainee_moto lang2cxx<PyObject*, trainee_moto>(PyObject* in)
{
  trainee_moto out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.emplacement = lang2cxx_array<position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.team = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.longueur = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// GUI specific
// Retourne la liste des actions effectuées pendant ce tour.
//
extern "C" std::vector<std::vector<int> > api_actions_effectuees();
static PyObject* p_actions_effectuees(PyObject* self, PyObject* args)
{
  std::vector<std::vector<int> > actions = api_actions_effectuees();
  size_t size = actions.size();
  PyObject* out = PyList_New(size);

  for (unsigned int i = 0; i < size; ++i)
    PyList_SET_ITEM(out, i, cxx2lang_array<int>(actions[i]));

  return out;
}

///
// Retourne le numéro de votre équipe
//
static PyObject* p_mon_equipe(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_mon_equipe());
  } catch (...) { return NULL; }
}

///
// Retourne les scores de chaque équipe
//
static PyObject* p_scores(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_scores());
  } catch (...) { return NULL; }
}

///
// Retourne le nombre d'équipes sur le terrain
//
static PyObject* p_nombre_equipes(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nombre_equipes());
  } catch (...) { return NULL; }
}

///
// Retourne le numéro du tour actuel
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
// Retourne la liste des sources d'énergie
//
static PyObject* p_sources_energie(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_sources_energie());
  } catch (...) { return NULL; }
}

///
// Retourne la liste des traînées de moto
//
static PyObject* p_trainees_moto(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_trainees_moto());
  } catch (...) { return NULL; }
}

///
// Retourne le type d'une case
//
static PyObject* p_regarder_type_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, type_case>(api_regarder_type_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne le type de bonus d'une case
//
static PyObject* p_regarder_type_bonus(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, type_bonus>(api_regarder_type_bonus(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la liste des bonus d'une équipe
//
static PyObject* p_regarder_bonus(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_regarder_bonus(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne la liste des id des traînées présentes sur une case
//
static PyObject* p_regarder_trainee_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_regarder_trainee_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Retourne si une case peut être traversée par une traînée de plus
//
static PyObject* p_case_traversable(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_case_traversable(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Renvoie les points que vous allez gagner a la fin du tour
//
static PyObject* p_diff_score(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_diff_score());
  } catch (...) { return NULL; }
}

///
// Renvoie le chemin le plus court entre deux points (fonction lente)
//
static PyObject* p_chemin(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_chemin(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Déplace une moto
//
static PyObject* p_deplacer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_deplacer(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, position>(a2)));
  } catch (...) { return NULL; }
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
static PyObject* p_couper_trainee_moto(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_couper_trainee_moto(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, position>(a2)));
  } catch (...) { return NULL; }
}

///
// Annuler l'action précédente
//
static PyObject* p_cancel(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_cancel());
  } catch (...) { return NULL; }
}

///
// Enrouler la traînée de moto en un point
//
static PyObject* p_enrouler(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_enrouler(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// Régénère une source d'énergie à son maximal
//
static PyObject* p_regenerer_source_energie(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_regenerer_source_energie(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Allonge le tour en rajoutant des points d'action
//
static PyObject* p_allonger_pa(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_allonger_pa());
  } catch (...) { return NULL; }
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
static PyObject* p_agrandir_trainee_moto(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_agrandir_trainee_moto(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
static PyObject* p_poser_point_croisement(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_poser_point_croisement(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
static PyObject* p_fusionner(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
PyObject* a3;
  if (!PyArg_ParseTuple(args, "OOOO", &a0, &a1, &a2, &a3)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_fusionner(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, position>(a1), lang2cxx<PyObject*, int>(a2), lang2cxx<PyObject*, position>(a3)));
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
// Affiche le contenu d'une valeur de type type_case
//
static PyObject* p_afficher_type_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_type_case(lang2cxx<PyObject*, type_case>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type type_bonus
//
static PyObject* p_afficher_type_bonus(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_type_bonus(lang2cxx<PyObject*, type_bonus>(a0));
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
// Affiche le contenu d'une valeur de type source_energie
//
static PyObject* p_afficher_source_energie(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_source_energie(lang2cxx<PyObject*, source_energie>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
static PyObject* p_afficher_trainee_moto(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_trainee_moto(lang2cxx<PyObject*, trainee_moto>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"mon_equipe", p_mon_equipe, METH_VARARGS, "mon_equipe"},  {"scores", p_scores, METH_VARARGS, "scores"},  {"nombre_equipes", p_nombre_equipes, METH_VARARGS, "nombre_equipes"},  {"tour_actuel", p_tour_actuel, METH_VARARGS, "tour_actuel"},  {"sources_energie", p_sources_energie, METH_VARARGS, "sources_energie"},  {"trainees_moto", p_trainees_moto, METH_VARARGS, "trainees_moto"},  {"regarder_type_case", p_regarder_type_case, METH_VARARGS, "regarder_type_case"},  {"regarder_type_bonus", p_regarder_type_bonus, METH_VARARGS, "regarder_type_bonus"},  {"regarder_bonus", p_regarder_bonus, METH_VARARGS, "regarder_bonus"},  {"regarder_trainee_case", p_regarder_trainee_case, METH_VARARGS, "regarder_trainee_case"},  {"case_traversable", p_case_traversable, METH_VARARGS, "case_traversable"},  {"diff_score", p_diff_score, METH_VARARGS, "diff_score"},  {"chemin", p_chemin, METH_VARARGS, "chemin"},  {"deplacer", p_deplacer, METH_VARARGS, "deplacer"},  {"couper_trainee_moto", p_couper_trainee_moto, METH_VARARGS, "couper_trainee_moto"},  {"cancel", p_cancel, METH_VARARGS, "cancel"},  {"enrouler", p_enrouler, METH_VARARGS, "enrouler"},  {"regenerer_source_energie", p_regenerer_source_energie, METH_VARARGS, "regenerer_source_energie"},  {"allonger_pa", p_allonger_pa, METH_VARARGS, "allonger_pa"},  {"agrandir_trainee_moto", p_agrandir_trainee_moto, METH_VARARGS, "agrandir_trainee_moto"},  {"poser_point_croisement", p_poser_point_croisement, METH_VARARGS, "poser_point_croisement"},  {"fusionner", p_fusionner, METH_VARARGS, "fusionner"},  {"afficher_erreur", p_afficher_erreur, METH_VARARGS, "afficher_erreur"},  {"afficher_type_case", p_afficher_type_case, METH_VARARGS, "afficher_type_case"},  {"afficher_type_bonus", p_afficher_type_bonus, METH_VARARGS, "afficher_type_bonus"},  {"afficher_position", p_afficher_position, METH_VARARGS, "afficher_position"},  {"afficher_source_energie", p_afficher_source_energie, METH_VARARGS, "afficher_source_energie"},  {"afficher_trainee_moto", p_afficher_trainee_moto, METH_VARARGS, "afficher_trainee_moto"},  {NULL, NULL, 0, NULL}
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
      
  champion_path = CHAMPION_PATH; // Set by Makefile.am
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

