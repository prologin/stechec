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
// Le type d'un objet
//
template <>
PyObject* cxx2lang<PyObject*, type_objet>(type_objet in)
{
  return cxx2lang<PyObject*, int>((int)in);
}

template <>
type_objet lang2cxx<PyObject*, type_objet>(PyObject* in)
{
  return (type_objet)lang2cxx<PyObject*, int>(in);
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
// les caracteristiques d'un objet
//
template <>
PyObject* cxx2lang<PyObject*, caracteristiques_objet>(caracteristiques_objet in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.cout)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.porte)));
  PyObject* name = PyString_FromString("caracteristiques_objet");
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
caracteristiques_objet lang2cxx<PyObject*, caracteristiques_objet>(PyObject* in)
{
  caracteristiques_objet out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.cout = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.porte = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Représente une unité sur le terrain.
//
template <>
PyObject* cxx2lang<PyObject*, unite>(unite in)
{
  PyObject* tuple = PyTuple_New(6);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, position>(in.pos_unite)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.team)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.ko)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.pa)));
  PyTuple_SET_ITEM(tuple, 4, (cxx2lang<PyObject*, type_objet>(in.objet)));
  PyTuple_SET_ITEM(tuple, 5, (cxx2lang<PyObject*, int>(in.id)));
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
  out.pos_unite = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.team = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.ko = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pa = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(4);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.objet = lang2cxx<PyObject*, type_objet>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(5);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Représente le type d'une piece.
//
template <>
PyObject* cxx2lang<PyObject*, piece>(piece in)
{
  PyObject* tuple = PyTuple_New(3);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.valeur)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, position>(in.pos_piece)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.tour_apparition)));
  PyObject* name = PyString_FromString("piece");
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
piece lang2cxx<PyObject*, piece>(PyObject* in)
{
  piece out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.valeur = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pos_piece = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.tour_apparition = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

///
// Renvoie le numero de votre equipe.
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
// Renvoie le score d'une equipe.
//
static PyObject* p_score(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_score(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

///
// Renvoie le nombre d'equipes sur la map
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
// Renvoie les pieces qui sont sur la map
//
static PyObject* p_pieces_en_jeu(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_pieces_en_jeu());
  } catch (...) { return NULL; }
}

///
// Renvoie le 10 prochaines pieces a vennir
//
static PyObject* p_pieces_a_vennir(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_pieces_a_vennir());
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
// Retourne les caracteristiques de l'objet.
//
static PyObject* p_proprietes_objet(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, caracteristiques_objet>(api_proprietes_objet(lang2cxx<PyObject*, type_objet>(a0)));
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
// Achete un objet
//
static PyObject* p_acheter_objet(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_acheter_objet(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, type_objet>(a1)));
  } catch (...) { return NULL; }
}

///
// utilise un objet
//
static PyObject* p_utiliser(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_utiliser(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
static PyObject* p_achever_la_partie(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_achever_la_partie());
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
// Affiche le contenu d'une valeur de type type_objet
//
static PyObject* p_afficher_type_objet(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_type_objet(lang2cxx<PyObject*, type_objet>(a0));
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
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
static PyObject* p_afficher_caracteristiques_objet(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_caracteristiques_objet(lang2cxx<PyObject*, caracteristiques_objet>(a0));
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
// Affiche le contenu d'une valeur de type piece
//
static PyObject* p_afficher_piece(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_piece(lang2cxx<PyObject*, piece>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"mon_equipe", p_mon_equipe, METH_VARARGS, "mon_equipe"},
  {"score", p_score, METH_VARARGS, "score"},
  {"nombre_equipes", p_nombre_equipes, METH_VARARGS, "nombre_equipes"},
  {"tour_actuel", p_tour_actuel, METH_VARARGS, "tour_actuel"},
  {"pieces_en_jeu", p_pieces_en_jeu, METH_VARARGS, "pieces_en_jeu"},
  {"pieces_a_vennir", p_pieces_a_vennir, METH_VARARGS, "pieces_a_vennir"},
  {"unites", p_unites, METH_VARARGS, "unites"},
  {"proprietes_objet", p_proprietes_objet, METH_VARARGS, "proprietes_objet"},
  {"deplacer", p_deplacer, METH_VARARGS, "deplacer"},
  {"acheter_objet", p_acheter_objet, METH_VARARGS, "acheter_objet"},
  {"utiliser", p_utiliser, METH_VARARGS, "utiliser"},
  {"achever_la_partie", p_achever_la_partie, METH_VARARGS, "achever_la_partie"},
  {"afficher_erreur", p_afficher_erreur, METH_VARARGS, "afficher_erreur"},
  {"afficher_type_objet", p_afficher_type_objet, METH_VARARGS, "afficher_type_objet"},
  {"afficher_position", p_afficher_position, METH_VARARGS, "afficher_position"},
  {"afficher_caracteristiques_objet", p_afficher_caracteristiques_objet, METH_VARARGS, "afficher_caracteristiques_objet"},
  {"afficher_unite", p_afficher_unite, METH_VARARGS, "afficher_unite"},
  {"afficher_piece", p_afficher_piece, METH_VARARGS, "afficher_piece"},
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


