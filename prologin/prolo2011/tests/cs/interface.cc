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
// Énumération représentant une erreur renvoyée par une des fonctions d'action
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
// Énumération représentant les différents types de case
//
template <>
gint32 cxx2lang< gint32, type_case >(type_case in)
{
  return (gint32)in;
}

template <>
type_case lang2cxx< gint32, type_case >(gint32 in)
{
  return (type_case)in;
}

///
// Énumération représentant les différents types de bonii
//
template <>
gint32 cxx2lang< gint32, bonus >(bonus in)
{
  return (gint32)in;
}

template <>
bonus lang2cxx< gint32, bonus >(gint32 in)
{
  return (bonus)in;
}

///
// Représente une position sur le terrain du jeu
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
// Caracteristiques d'une source d'énergie
//
void cxx2lang(source_energie in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "SourceEnergie");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
  cxx2lang(in.pos, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Pos"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.coef));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Coef"), &arg);
}

///
// Représente une traînée de moto sur le terrain
//
void cxx2lang(trainee_moto in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TraineeMoto");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
  cxx2lang(in.emplacement, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Emplacement"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.team));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Team"), &arg);
}

///
// Représente une position sur le terrain du jeu
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
// Caracteristiques d'une source d'énergie
//
template <>
MonoObject* cxx2lang< MonoObject*, source_energie >(source_energie in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "SourceEnergie");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
  cxx2lang(in.pos, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Pos"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.coef));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Coef"), &arg);
  return moObj;
}
template <>
source_energie lang2cxx< MonoObject*, source_energie >(MonoObject* in)
{
  source_energie out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "SourceEnergie");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Id"), &out.id);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pos"), &field_out);
  out.pos = lang2cxx< MonoObject*, position >(reinterpret_cast< MonoObject* >(field_out));
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Coef"), &out.coef);
  return out;
}

///
// Représente une traînée de moto sur le terrain
//
template <>
MonoObject* cxx2lang< MonoObject*, trainee_moto >(trainee_moto in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TraineeMoto");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.id));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Id"), &arg);
  cxx2lang(in.emplacement, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Emplacement"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.team));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Team"), &arg);
  return moObj;
}
template <>
trainee_moto lang2cxx< MonoObject*, trainee_moto >(MonoObject* in)
{
  trainee_moto out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TraineeMoto");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Id"), &out.id);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Emplacement"), &field_out);
  out.emplacement = lang2cxx< MonoArray*,  >(reinterpret_cast< MonoArray* >(field_out));
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Team"), &out.team);
  return out;
}

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action
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
// Énumération représentant les différents types de case
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<type_case> >(std::vector<type_case> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TypeCase");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_set(maArray, gint32, i, (cxx2lang< gint32, type_case >(in[i])));
  return maArray;
}

template <>
std::vector<type_case> lang2cxx< MonoArray*, std::vector<type_case> >(MonoArray* in)
{
  std::vector< type_case > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< gint32, type_case >(mono_array_get(in, gint32, i)));
  return out;
}

///
// Énumération représentant les différents types de bonii
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<bonus> >(std::vector<bonus> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Bonus");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_set(maArray, gint32, i, (cxx2lang< gint32, bonus >(in[i])));
  return maArray;
}

template <>
std::vector<bonus> lang2cxx< MonoArray*, std::vector<bonus> >(MonoArray* in)
{
  std::vector< bonus > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< gint32, bonus >(mono_array_get(in, gint32, i)));
  return out;
}

///
// Représente une position sur le terrain du jeu
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
// Caracteristiques d'une source d'énergie
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<source_energie> >(std::vector<source_energie> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "SourceEnergie");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, source_energie >(in[i])));
  return maArray;
}

template <>
std::vector<source_energie> lang2cxx< MonoArray*, std::vector<source_energie> >(MonoArray* in)
{
  std::vector< source_energie > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, source_energie >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

///
// Représente une traînée de moto sur le terrain
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<trainee_moto> >(std::vector<trainee_moto> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TraineeMoto");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, trainee_moto >(in[i])));
  return maArray;
}

template <>
std::vector<trainee_moto> lang2cxx< MonoArray*, std::vector<trainee_moto> >(MonoArray* in)
{
  std::vector< trainee_moto > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, trainee_moto >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

gint32 mon_equipe()
{
	return cxx2lang< gint32, int >(api_mon_equipe());
}
MonoArray* scores()
{
	return cxx2lang< MonoArray*, std::vector<int> >(api_scores());
}
gint32 nombre_equipes()
{
	return cxx2lang< gint32, int >(api_nombre_equipes());
}
gint32 tour_actuel()
{
	return cxx2lang< gint32, int >(api_tour_actuel());
}
MonoArray* sources_energie()
{
	return cxx2lang< MonoArray*, std::vector<source_energie> >(api_sources_energie());
}
MonoArray* trainees_moto()
{
	return cxx2lang< MonoArray*, std::vector<trainee_moto> >(api_trainees_moto());
}
gint32 regarder_type_case(MonoObject* pos)
{
	return cxx2lang< gint32, type_case >(api_regarder_type_case(lang2cxx< MonoObject*, position >(pos)));
}
gint32 regarder_type_bonus(MonoObject* pos)
{
	return cxx2lang< gint32, bonus >(api_regarder_type_bonus(lang2cxx< MonoObject*, position >(pos)));
}
MonoArray* regarder_bonus(int equipe)
{
	return cxx2lang< MonoArray*, std::vector<bonus> >(api_regarder_bonus(lang2cxx< gint32, int >(equipe)));
}
gint32 deplacer(int id, MonoObject* de, MonoObject* vers)
{
	return cxx2lang< gint32, erreur >(api_deplacer(lang2cxx< gint32, int >(id), lang2cxx< MonoObject*, position >(de), lang2cxx< MonoObject*, position >(vers)));
}
gint32 couper_trainee_moto(int id, MonoObject* entre, MonoObject* et)
{
	return cxx2lang< gint32, erreur >(api_couper_trainee_moto(lang2cxx< gint32, int >(id), lang2cxx< MonoObject*, position >(entre), lang2cxx< MonoObject*, position >(et)));
}
gint32 cancel()
{
	return cxx2lang< gint32, erreur >(api_cancel());
}
gint32 enrouler(int id, MonoObject* point)
{
	return cxx2lang< gint32, erreur >(api_enrouler(lang2cxx< gint32, int >(id), lang2cxx< MonoObject*, position >(point)));
}
gint32 regenerer_source_energie(int id)
{
	return cxx2lang< gint32, erreur >(api_regenerer_source_energie(lang2cxx< gint32, int >(id)));
}
gint32 allonger_pa()
{
	return cxx2lang< gint32, erreur >(api_allonger_pa());
}
gint32 agrandir_trainee_moto(int id, int longueur)
{
	return cxx2lang< gint32, erreur >(api_agrandir_trainee_moto(lang2cxx< gint32, int >(id), lang2cxx< gint32, int >(longueur)));
}
gint32 poser_point_croisement(MonoObject* point)
{
	return cxx2lang< gint32, erreur >(api_poser_point_croisement(lang2cxx< MonoObject*, position >(point)));
}
gint32 fusionner(int id1, MonoObject* pos1, int id2, MonoObject* pos2)
{
	return cxx2lang< gint32, erreur >(api_fusionner(lang2cxx< gint32, int >(id1), lang2cxx< MonoObject*, position >(pos1), lang2cxx< gint32, int >(id2), lang2cxx< MonoObject*, position >(pos2)));
}
void afficher_erreur(erreur v)
{
	api_afficher_erreur(lang2cxx< gint32, erreur >(v));
}
void afficher_type_case(type_case v)
{
	api_afficher_type_case(lang2cxx< gint32, type_case >(v));
}
void afficher_bonus(bonus v)
{
	api_afficher_bonus(lang2cxx< gint32, bonus >(v));
}
void afficher_position(MonoObject* v)
{
	api_afficher_position(lang2cxx< MonoObject*, position >(v));
}
void afficher_source_energie(MonoObject* v)
{
	api_afficher_source_energie(lang2cxx< MonoObject*, source_energie >(v));
}
void afficher_trainee_moto(MonoObject* v)
{
	api_afficher_trainee_moto(lang2cxx< MonoObject*, trainee_moto >(v));
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
  mono_add_internal_call("Prologin.Api::MonEquipe", (const void*)mon_equipe);  mono_add_internal_call("Prologin.Api::Scores", (const void*)scores);  mono_add_internal_call("Prologin.Api::NombreEquipes", (const void*)nombre_equipes);  mono_add_internal_call("Prologin.Api::TourActuel", (const void*)tour_actuel);  mono_add_internal_call("Prologin.Api::SourcesEnergie", (const void*)sources_energie);  mono_add_internal_call("Prologin.Api::TraineesMoto", (const void*)trainees_moto);  mono_add_internal_call("Prologin.Api::RegarderTypeCase", (const void*)regarder_type_case);  mono_add_internal_call("Prologin.Api::RegarderTypeBonus", (const void*)regarder_type_bonus);  mono_add_internal_call("Prologin.Api::RegarderBonus", (const void*)regarder_bonus);  mono_add_internal_call("Prologin.Api::Deplacer", (const void*)deplacer);  mono_add_internal_call("Prologin.Api::CouperTraineeMoto", (const void*)couper_trainee_moto);  mono_add_internal_call("Prologin.Api::Cancel", (const void*)cancel);  mono_add_internal_call("Prologin.Api::Enrouler", (const void*)enrouler);  mono_add_internal_call("Prologin.Api::RegenererSourceEnergie", (const void*)regenerer_source_energie);  mono_add_internal_call("Prologin.Api::AllongerPa", (const void*)allonger_pa);  mono_add_internal_call("Prologin.Api::AgrandirTraineeMoto", (const void*)agrandir_trainee_moto);  mono_add_internal_call("Prologin.Api::PoserPointCroisement", (const void*)poser_point_croisement);  mono_add_internal_call("Prologin.Api::Fusionner", (const void*)fusionner);  mono_add_internal_call("Prologin.Api::AfficherErreur", (const void*)afficher_erreur);  mono_add_internal_call("Prologin.Api::AfficherTypeCase", (const void*)afficher_type_case);  mono_add_internal_call("Prologin.Api::AfficherBonus", (const void*)afficher_bonus);  mono_add_internal_call("Prologin.Api::AfficherPosition", (const void*)afficher_position);  mono_add_internal_call("Prologin.Api::AfficherSourceEnergie", (const void*)afficher_source_energie);  mono_add_internal_call("Prologin.Api::AfficherTraineeMoto", (const void*)afficher_trainee_moto);}

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
