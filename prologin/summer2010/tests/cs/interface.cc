///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cs.rb
//

#include "interface.hh"

#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <cstring>

CSharpInterface gl_csharp;

template < class Out, class Cxx >
Out cxx2lang(Cxx in)
{
  return (Out)in;
}

template <>
MonoString* cxx2lang<MonoString*, std::string>(std::string in)
{
  return mono_string_new (gl_csharp.getDomain(), in.c_str());
}

template <>
gint32 cxx2lang< gint32, int >(int in)
{
  return (gint32)in;
}

template <>
gint32 cxx2lang< gint32, bool >(bool in)
{
  return (gint32)in;
}

template < class Out, class Cxx >
Cxx lang2cxx(Out in)
{
  return (Cxx)in;
}

template <>
std::string lang2cxx< MonoString*, std::string >(MonoString* in)
{
  return mono_string_to_utf8(in);
}

template <>
int lang2cxx< gint32, int >(gint32 in)
{
  return (int)in;
}

template <>
bool lang2cxx< gint32, bool >(gint32 in)
{
  return (bool)in;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template <>
gint32 cxx2lang< gint32, erreur >(erreur in)
{
  return (gint32)in;
}

template <>
erreur lang2cxx< gint32, erreur >(gint32 in)
{
  return (erreur)in;
}

///
// Le type d'un objet
//
template <>
gint32 cxx2lang< gint32, type_objet >(type_objet in)
{
  return (gint32)in;
}

template <>
type_objet lang2cxx< gint32, type_objet >(gint32 in)
{
  return (type_objet)in;
}

///
// Représente une position sur le terrain du jeu.
//
void cxx2lang(position in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Position");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.x));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "X"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.y));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Y"), &arg);
}

///
// les caracteristiques d'un objet
//
void cxx2lang(caracteristiques_objet in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "CaracteristiquesObjet");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.cout));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Cout"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.porte));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Porte"), &arg);
}

///
// Représente une unité sur le terrain.
//
void cxx2lang(unite in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Unite");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  cxx2lang(in.pos_unite, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "PosUnite"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.team));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Team"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.ko));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ko"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_objet >(in.objet));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Objet"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
}

///
// Représente le type d'une piece.
//
void cxx2lang(piece in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Piece");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.valeur));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Valeur"), &arg);
  cxx2lang(in.pos_piece, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "PosPiece"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.tour_apparition));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "TourApparition"), &arg);
}

///
// Représente une position sur le terrain du jeu.
//
template <>
MonoObject* cxx2lang< MonoObject*, position >(position in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Position");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.x));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "X"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.y));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Y"), &arg);
  return moObj;
}
template <>
position lang2cxx< MonoObject*, position >(MonoObject* in)
{
  position out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Position");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "X"), &out.x);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Y"), &out.y);
  return out;
}

///
// les caracteristiques d'un objet
//
template <>
MonoObject* cxx2lang< MonoObject*, caracteristiques_objet >(caracteristiques_objet in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "CaracteristiquesObjet");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.cout));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Cout"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.porte));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Porte"), &arg);
  return moObj;
}
template <>
caracteristiques_objet lang2cxx< MonoObject*, caracteristiques_objet >(MonoObject* in)
{
  caracteristiques_objet out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "CaracteristiquesObjet");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Cout"), &out.cout);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Porte"), &out.porte);
  return out;
}

///
// Représente une unité sur le terrain.
//
template <>
MonoObject* cxx2lang< MonoObject*, unite >(unite in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Unite");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  cxx2lang(in.pos_unite, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "PosUnite"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.team));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Team"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.ko));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ko"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_objet >(in.objet));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Objet"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
  return moObj;
}
template <>
unite lang2cxx< MonoObject*, unite >(MonoObject* in)
{
  unite out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Unite");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "PosUnite"), &field_out);
  out.pos_unite = lang2cxx< MonoObject*, position >(reinterpret_cast< MonoObject* >(field_out));
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Team"), &out.team);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Ko"), &out.ko);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pa"), &out.pa);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Objet"), &out.objet);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Id"), &out.id);
  return out;
}

///
// Représente le type d'une piece.
//
template <>
MonoObject* cxx2lang< MonoObject*, piece >(piece in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Piece");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.valeur));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Valeur"), &arg);
  cxx2lang(in.pos_piece, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "PosPiece"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.tour_apparition));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "TourApparition"), &arg);
  return moObj;
}
template <>
piece lang2cxx< MonoObject*, piece >(MonoObject* in)
{
  piece out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Piece");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Valeur"), &out.valeur);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "PosPiece"), &field_out);
  out.pos_piece = lang2cxx< MonoObject*, position >(reinterpret_cast< MonoObject* >(field_out));
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "TourApparition"), &out.tour_apparition);
  return out;
}

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<erreur> >(std::vector<erreur> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Erreur");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_set(maArray, gint32, i, (cxx2lang< gint32, erreur >(in[i])));
  return maArray;
}

template <>
std::vector<erreur> lang2cxx< MonoArray*, std::vector<erreur> >(MonoArray* in)
{
  std::vector< erreur > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< gint32, erreur >(mono_array_get(in, gint32, i)));
  return out;
}

///
// Le type d'un objet
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<type_objet> >(std::vector<type_objet> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TypeObjet");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_set(maArray, gint32, i, (cxx2lang< gint32, type_objet >(in[i])));
  return maArray;
}

template <>
std::vector<type_objet> lang2cxx< MonoArray*, std::vector<type_objet> >(MonoArray* in)
{
  std::vector< type_objet > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< gint32, type_objet >(mono_array_get(in, gint32, i)));
  return out;
}

///
// Représente une position sur le terrain du jeu.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<position> >(std::vector<position> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Position");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, position >(in[i])));
  return maArray;
}

template <>
std::vector<position> lang2cxx< MonoArray*, std::vector<position> >(MonoArray* in)
{
  std::vector< position > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, position >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

///
// les caracteristiques d'un objet
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<caracteristiques_objet> >(std::vector<caracteristiques_objet> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "CaracteristiquesObjet");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, caracteristiques_objet >(in[i])));
  return maArray;
}

template <>
std::vector<caracteristiques_objet> lang2cxx< MonoArray*, std::vector<caracteristiques_objet> >(MonoArray* in)
{
  std::vector< caracteristiques_objet > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, caracteristiques_objet >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

///
// Représente une unité sur le terrain.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<unite> >(std::vector<unite> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Unite");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, unite >(in[i])));
  return maArray;
}

template <>
std::vector<unite> lang2cxx< MonoArray*, std::vector<unite> >(MonoArray* in)
{
  std::vector< unite > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, unite >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

///
// Représente le type d'une piece.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<piece> >(std::vector<piece> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Piece");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, piece >(in[i])));
  return maArray;
}

template <>
std::vector<piece> lang2cxx< MonoArray*, std::vector<piece> >(MonoArray* in)
{
  std::vector< piece > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, piece >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

gint32 mon_equipe()
{
	return cxx2lang< gint32, int >(api_mon_equipe());
}

gint32 score(int team)
{
	return cxx2lang< gint32, int >(api_score(lang2cxx< gint32, int >(team)));
}

gint32 nombre_equipes()
{
	return cxx2lang< gint32, int >(api_nombre_equipes());
}

gint32 tour_actuel()
{
	return cxx2lang< gint32, int >(api_tour_actuel());
}

MonoArray* pieces_en_jeu()
{
	return cxx2lang< MonoArray*, std::vector<piece> >(api_pieces_en_jeu());
}

MonoArray* pieces_a_vennir()
{
	return cxx2lang< MonoArray*, std::vector<piece> >(api_pieces_a_vennir());
}

MonoArray* unites()
{
	return cxx2lang< MonoArray*, std::vector<unite> >(api_unites());
}

MonoObject* proprietes_objet(type_objet to)
{
	return cxx2lang< MonoObject*, caracteristiques_objet >(api_proprietes_objet(lang2cxx< gint32, type_objet >(to)));
}

gint32 deplacer(MonoObject* cible, MonoObject* pos)
{
	return cxx2lang< gint32, erreur >(api_deplacer(lang2cxx< MonoObject*, position >(cible), lang2cxx< MonoObject*, position >(pos)));
}

gint32 acheter_objet(MonoObject* cible, type_objet objet)
{
	return cxx2lang< gint32, erreur >(api_acheter_objet(lang2cxx< MonoObject*, position >(cible), lang2cxx< gint32, type_objet >(objet)));
}

gint32 utiliser(MonoObject* attaquant, MonoObject* cible)
{
	return cxx2lang< gint32, erreur >(api_utiliser(lang2cxx< MonoObject*, position >(attaquant), lang2cxx< MonoObject*, position >(cible)));
}

gint32 achever_la_partie()
{
	return cxx2lang< gint32, erreur >(api_achever_la_partie());
}

void afficher_erreur(erreur v)
{
	api_afficher_erreur(lang2cxx< gint32, erreur >(v));
}

void afficher_type_objet(type_objet v)
{
	api_afficher_type_objet(lang2cxx< gint32, type_objet >(v));
}

void afficher_position(MonoObject* v)
{
	api_afficher_position(lang2cxx< MonoObject*, position >(v));
}

void afficher_caracteristiques_objet(MonoObject* v)
{
	api_afficher_caracteristiques_objet(lang2cxx< MonoObject*, caracteristiques_objet >(v));
}

void afficher_unite(MonoObject* v)
{
	api_afficher_unite(lang2cxx< MonoObject*, unite >(v));
}

void afficher_piece(MonoObject* v)
{
	api_afficher_piece(lang2cxx< MonoObject*, piece >(v));
}


/*
** Inititialize Mono and load the DLL file.
*/
CSharpInterface::CSharpInterface()
{
  const char*		champion_path = getenv("CHAMPION_PATH");
  std::string		champion;

  if (!champion_path)
    champion = "./champion-prologin.dll";
  else
  {
    champion = champion_path;
    champion += "/champion-prologin.dll";
  }

  _domain = mono_jit_init(champion.c_str());
  assert(_domain != NULL);

  _assembly = mono_domain_assembly_open(_domain, champion.c_str());
  assert(_assembly != NULL);

  _image = mono_assembly_get_image(_assembly);
  assert(_image != NULL);

  _class = mono_class_from_name(_image, "Prologin", "Prologin");
  assert(_class != NULL);

  _object = mono_object_new(_domain, _class);
  assert(_object);

  mono_runtime_object_init(_object);

  // Register API functions as internal Mono functions
  mono_add_internal_call("Prologin.Api::MonEquipe", (const void*)mon_equipe);
  mono_add_internal_call("Prologin.Api::Score", (const void*)score);
  mono_add_internal_call("Prologin.Api::NombreEquipes", (const void*)nombre_equipes);
  mono_add_internal_call("Prologin.Api::TourActuel", (const void*)tour_actuel);
  mono_add_internal_call("Prologin.Api::PiecesEnJeu", (const void*)pieces_en_jeu);
  mono_add_internal_call("Prologin.Api::PiecesAVennir", (const void*)pieces_a_vennir);
  mono_add_internal_call("Prologin.Api::Unites", (const void*)unites);
  mono_add_internal_call("Prologin.Api::ProprietesObjet", (const void*)proprietes_objet);
  mono_add_internal_call("Prologin.Api::Deplacer", (const void*)deplacer);
  mono_add_internal_call("Prologin.Api::AcheterObjet", (const void*)acheter_objet);
  mono_add_internal_call("Prologin.Api::Utiliser", (const void*)utiliser);
  mono_add_internal_call("Prologin.Api::AcheverLaPartie", (const void*)achever_la_partie);
  mono_add_internal_call("Prologin.Api::AfficherErreur", (const void*)afficher_erreur);
  mono_add_internal_call("Prologin.Api::AfficherTypeObjet", (const void*)afficher_type_objet);
  mono_add_internal_call("Prologin.Api::AfficherPosition", (const void*)afficher_position);
  mono_add_internal_call("Prologin.Api::AfficherCaracteristiquesObjet", (const void*)afficher_caracteristiques_objet);
  mono_add_internal_call("Prologin.Api::AfficherUnite", (const void*)afficher_unite);
  mono_add_internal_call("Prologin.Api::AfficherPiece", (const void*)afficher_piece);
}

MonoImage* CSharpInterface::getImage()
{
    return _image;
}

MonoDomain* CSharpInterface::getDomain()
{
    return _domain;
}

CSharpInterface::~CSharpInterface()
{
  mono_image_close(_image);
  mono_assembly_close(_assembly);
  // XXX -- mono segfaults when calling this. Seems to be a known bug
  //        appearing when mono_jit_clean() is called from a dtor. ???
  //mono_jit_cleanup(_domain);
}

/*
** Calls C# functions from C++
*/
MonoObject* CSharpInterface::callCSharpMethod(const char* name)
{
  MonoMethod*	method;

  method = mono_class_get_method_from_name(_class, name, 0);
  return mono_runtime_invoke(method, _object, NULL, NULL);
}

/*
** Functions called from stechec to C.
*/
void init_game()
{
  gl_csharp.callCSharpMethod("InitGame");
}

void jouer()
{
  gl_csharp.callCSharpMethod("Jouer");
}

void end_game()
{
  gl_csharp.callCSharpMethod("EndGame");
}

