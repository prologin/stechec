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
// Le type d'une unité sur le terrain
//
template <>
gint32 cxx2lang< gint32, type_unite >(type_unite in)
{
  return (gint32)in;
}

template <>
type_unite lang2cxx< gint32, type_unite >(gint32 in)
{
  return (type_unite)in;
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
// Contient les informations sur la taille du terrain du jeu.
//
void cxx2lang(taille_terrain in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TailleTerrain");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.taille));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Taille"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.min_coord));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "MinCoord"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.max_coord));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "MaxCoord"), &arg);
}

///
// Donne les caractéristiques d'un type d'unité.
//
void cxx2lang(caracs in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Caracs");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.portee));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Portee"), &arg);
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
  cxx2lang(in.pos, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Pos"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, bool >(in.ennemi));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ennemi"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_unite >(in.type_unite_actuel));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "TypeUniteActuel"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_unite >(in.vrai_type_unite));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "VraiTypeUnite"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.ko));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ko"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.attaques));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Attaques"), &arg);
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
void cxx2lang(cartes in, MonoObject* moObj = NULL)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Cartes");
  if (!moObj) moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.soin));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Soin"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.deguisement));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Deguisement"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.banzai));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Banzai"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pacifisme));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pacifisme"), &arg);
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
// Contient les informations sur la taille du terrain du jeu.
//
template <>
MonoObject* cxx2lang< MonoObject*, taille_terrain >(taille_terrain in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TailleTerrain");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.taille));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Taille"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.min_coord));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "MinCoord"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.max_coord));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "MaxCoord"), &arg);
  return moObj;
}
template <>
taille_terrain lang2cxx< MonoObject*, taille_terrain >(MonoObject* in)
{
  taille_terrain out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TailleTerrain");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Taille"), &out.taille);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "MinCoord"), &out.min_coord);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "MaxCoord"), &out.max_coord);
  return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
MonoObject* cxx2lang< MonoObject*, caracs >(caracs in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Caracs");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.portee));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Portee"), &arg);
  return moObj;
}
template <>
caracs lang2cxx< MonoObject*, caracs >(MonoObject* in)
{
  caracs out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Caracs");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pa"), &out.pa);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Portee"), &out.portee);
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
  cxx2lang(in.pos, mono_field_get_value_object(gl_csharp.getDomain(), mono_class_get_field_from_name(mcKlass, "Pos"), moObj));
  arg = reinterpret_cast< void* >(cxx2lang< gint32, bool >(in.ennemi));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ennemi"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_unite >(in.type_unite_actuel));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "TypeUniteActuel"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, type_unite >(in.vrai_type_unite));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "VraiTypeUnite"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.ko));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Ko"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pa));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pa"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.attaques));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Attaques"), &arg);
  return moObj;
}
template <>
unite lang2cxx< MonoObject*, unite >(MonoObject* in)
{
  unite out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Unite");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pos"), &field_out);
  out.pos = lang2cxx< MonoObject*, position >(reinterpret_cast< MonoObject* >(field_out));
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Ennemi"), &out.ennemi);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "TypeUniteActuel"), &out.type_unite_actuel);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "VraiTypeUnite"), &out.vrai_type_unite);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Ko"), &out.ko);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pa"), &out.pa);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Attaques"), &out.attaques);
  return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
MonoObject* cxx2lang< MonoObject*, cartes >(cartes in)
{
  void* arg;
  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Cartes");
  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);
  mono_runtime_object_init(moObj);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.soin));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Soin"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.deguisement));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Deguisement"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.banzai));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Banzai"), &arg);
  arg = reinterpret_cast< void* >(cxx2lang< gint32, int >(in.pacifisme));
  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, "Pacifisme"), &arg);
  return moObj;
}
template <>
cartes lang2cxx< MonoObject*, cartes >(MonoObject* in)
{
  cartes out;
  void*      field_out;
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Cartes");
  (void)field_out;
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Soin"), &out.soin);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Deguisement"), &out.deguisement);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Banzai"), &out.banzai);
  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, "Pacifisme"), &out.pacifisme);
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
// Le type d'une unité sur le terrain
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<type_unite> >(std::vector<type_unite> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TypeUnite");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_set(maArray, gint32, i, (cxx2lang< gint32, type_unite >(in[i])));
  return maArray;
}

template <>
std::vector<type_unite> lang2cxx< MonoArray*, std::vector<type_unite> >(MonoArray* in)
{
  std::vector< type_unite > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< gint32, type_unite >(mono_array_get(in, gint32, i)));
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
// Contient les informations sur la taille du terrain du jeu.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<taille_terrain> >(std::vector<taille_terrain> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "TailleTerrain");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, taille_terrain >(in[i])));
  return maArray;
}

template <>
std::vector<taille_terrain> lang2cxx< MonoArray*, std::vector<taille_terrain> >(MonoArray* in)
{
  std::vector< taille_terrain > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, taille_terrain >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<caracs> >(std::vector<caracs> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Caracs");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, caracs >(in[i])));
  return maArray;
}

template <>
std::vector<caracs> lang2cxx< MonoArray*, std::vector<caracs> >(MonoArray* in)
{
  std::vector< caracs > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, caracs >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
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
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
MonoArray* cxx2lang< MonoArray*, std::vector<cartes> >(std::vector<cartes> in)
{
  gint32 size = in.size();
  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), "Prologin", "Cartes");
  if (size == 0)
    return mono_array_new(gl_csharp.getDomain(), mcKlass, 0);

  MonoArray * maArray = mono_array_new(gl_csharp.getDomain(), mcKlass, size);
  for (int i = 0; i < size; ++i)
		mono_array_setref(maArray, i, (cxx2lang< MonoObject*, cartes >(in[i])));
  return maArray;
}

template <>
std::vector<cartes> lang2cxx< MonoArray*, std::vector<cartes> >(MonoArray* in)
{
  std::vector< cartes > out;
  gint32 size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
		out.push_back(lang2cxx< MonoObject*, cartes >(reinterpret_cast<MonoObject*>(mono_array_get(in, MonoObject*, i))));
  return out;
}

gint32 tour_actuel()
{
	return cxx2lang< gint32, int >(api_tour_actuel());
}

MonoObject* pos_spawn(bool ennemi)
{
	return cxx2lang< MonoObject*, position >(api_pos_spawn(lang2cxx< gint32, bool >(ennemi)));
}

MonoObject* caracteristiques(type_unite tu)
{
	return cxx2lang< MonoObject*, caracs >(api_caracteristiques(lang2cxx< gint32, type_unite >(tu)));
}

MonoObject* mes_cartes()
{
	return cxx2lang< MonoObject*, cartes >(api_mes_cartes());
}

MonoArray* unites()
{
	return cxx2lang< MonoArray*, std::vector<unite> >(api_unites());
}

MonoObject* taille_terrain_actuelle()
{
	return cxx2lang< MonoObject*, taille_terrain >(api_taille_terrain_actuelle());
}

gint32 soin(MonoObject* cible)
{
	return cxx2lang< gint32, erreur >(api_soin(lang2cxx< MonoObject*, position >(cible)));
}

gint32 deguisement(MonoObject* cible, type_unite nouveau_type)
{
	return cxx2lang< gint32, erreur >(api_deguisement(lang2cxx< MonoObject*, position >(cible), lang2cxx< gint32, type_unite >(nouveau_type)));
}

gint32 banzai(MonoObject* cible)
{
	return cxx2lang< gint32, erreur >(api_banzai(lang2cxx< MonoObject*, position >(cible)));
}

gint32 pacifisme()
{
	return cxx2lang< gint32, erreur >(api_pacifisme());
}

gint32 deplacer(MonoObject* cible, MonoObject* pos)
{
	return cxx2lang< gint32, erreur >(api_deplacer(lang2cxx< MonoObject*, position >(cible), lang2cxx< MonoObject*, position >(pos)));
}

gint32 relever(MonoObject* cible)
{
	return cxx2lang< gint32, erreur >(api_relever(lang2cxx< MonoObject*, position >(cible)));
}

gint32 attaquer(MonoObject* attaquant, MonoObject* cible)
{
	return cxx2lang< gint32, erreur >(api_attaquer(lang2cxx< MonoObject*, position >(attaquant), lang2cxx< MonoObject*, position >(cible)));
}

gint32 spawn(type_unite quoi)
{
	return cxx2lang< gint32, erreur >(api_spawn(lang2cxx< gint32, type_unite >(quoi)));
}

gint32 annuler()
{
	return cxx2lang< gint32, bool >(api_annuler());
}

void afficher_erreur(erreur v)
{
	api_afficher_erreur(lang2cxx< gint32, erreur >(v));
}

void afficher_type_unite(type_unite v)
{
	api_afficher_type_unite(lang2cxx< gint32, type_unite >(v));
}

void afficher_position(MonoObject* v)
{
	api_afficher_position(lang2cxx< MonoObject*, position >(v));
}

void afficher_taille_terrain(MonoObject* v)
{
	api_afficher_taille_terrain(lang2cxx< MonoObject*, taille_terrain >(v));
}

void afficher_caracs(MonoObject* v)
{
	api_afficher_caracs(lang2cxx< MonoObject*, caracs >(v));
}

void afficher_unite(MonoObject* v)
{
	api_afficher_unite(lang2cxx< MonoObject*, unite >(v));
}

void afficher_cartes(MonoObject* v)
{
	api_afficher_cartes(lang2cxx< MonoObject*, cartes >(v));
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
  mono_add_internal_call("Prologin.Api::TourActuel", (const void*)tour_actuel);
  mono_add_internal_call("Prologin.Api::PosSpawn", (const void*)pos_spawn);
  mono_add_internal_call("Prologin.Api::Caracteristiques", (const void*)caracteristiques);
  mono_add_internal_call("Prologin.Api::MesCartes", (const void*)mes_cartes);
  mono_add_internal_call("Prologin.Api::Unites", (const void*)unites);
  mono_add_internal_call("Prologin.Api::TailleTerrainActuelle", (const void*)taille_terrain_actuelle);
  mono_add_internal_call("Prologin.Api::Soin", (const void*)soin);
  mono_add_internal_call("Prologin.Api::Deguisement", (const void*)deguisement);
  mono_add_internal_call("Prologin.Api::Banzai", (const void*)banzai);
  mono_add_internal_call("Prologin.Api::Pacifisme", (const void*)pacifisme);
  mono_add_internal_call("Prologin.Api::Deplacer", (const void*)deplacer);
  mono_add_internal_call("Prologin.Api::Relever", (const void*)relever);
  mono_add_internal_call("Prologin.Api::Attaquer", (const void*)attaquer);
  mono_add_internal_call("Prologin.Api::Spawn", (const void*)spawn);
  mono_add_internal_call("Prologin.Api::Annuler", (const void*)annuler);
  mono_add_internal_call("Prologin.Api::AfficherErreur", (const void*)afficher_erreur);
  mono_add_internal_call("Prologin.Api::AfficherTypeUnite", (const void*)afficher_type_unite);
  mono_add_internal_call("Prologin.Api::AfficherPosition", (const void*)afficher_position);
  mono_add_internal_call("Prologin.Api::AfficherTailleTerrain", (const void*)afficher_taille_terrain);
  mono_add_internal_call("Prologin.Api::AfficherCaracs", (const void*)afficher_caracs);
  mono_add_internal_call("Prologin.Api::AfficherUnite", (const void*)afficher_unite);
  mono_add_internal_call("Prologin.Api::AfficherCartes", (const void*)afficher_cartes);
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

position retirer_ko()
{
  return lang2cxx< MonoObject*, position >(gl_csharp.callCSharpMethod("RetirerKo"));
}

void jouer()
{
  gl_csharp.callCSharpMethod("Jouer");
}

void end_game()
{
  gl_csharp.callCSharpMethod("EndGame");
}

