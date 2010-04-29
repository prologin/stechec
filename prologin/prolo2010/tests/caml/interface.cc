///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_caml.rb
//

extern "C" {
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/alloc.h>
#include <caml/memory.h>
}
#include "interface.hh"

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
value cxx2lang<value, int>(int in)
{
  return Val_int(in);
}

template <>
value cxx2lang<value, bool>(bool in)
{
  return Val_int(in);
}

template <typename Cxx>
value cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  if (size == 0)
    return Atom(0);

  value v = caml_alloc(size, 0);
  for (int i = 0; i < size; ++i)
    Field(v, i) = cxx2lang<value, Cxx>(in[i]);

  return v;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<value, int>(value in)
{
  return Int_val(in);
}

template <>
bool lang2cxx<value, bool>(value in)
{
  return Int_val(in);
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(value in)
{
  std::vector<Cxx> out;
  mlsize_t size = Wosize_val(in);

  for (int i = 0; i < size; ++i)
    out.push_back(lang2cxx<value, Cxx>(Field(in, i)));

  return out;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template <>
value cxx2lang<value, erreur>(erreur in)
{
  return Val_int(in);
}

template <>
erreur lang2cxx<value, erreur>(value in)
{
  return (erreur)Int_val(in);
}

///
// Le type d'une unité sur le terrain
//
template <>
value cxx2lang<value, type_unite>(type_unite in)
{
  return Val_int(in);
}

template <>
type_unite lang2cxx<value, type_unite>(value in)
{
  return (type_unite)Int_val(in);
}

///
// Représente une position sur le terrain du jeu.
//
template <>
value cxx2lang<value, position>(position in)
{
  value out = caml_alloc(2, 0);
  Field(out, 0) = cxx2lang<value, int>(in.x);
  Field(out, 1) = cxx2lang<value, int>(in.y);
  return out;
}

template <>
position lang2cxx<value, position>(value in)
{
  position out;
  out.x = lang2cxx<value, int>(Field(in, 0));
  out.y = lang2cxx<value, int>(Field(in, 1));
  return out;
}

///
// Contient les informations sur la taille du terrain du jeu.
//
template <>
value cxx2lang<value, taille_terrain>(taille_terrain in)
{
  value out = caml_alloc(3, 0);
  Field(out, 0) = cxx2lang<value, int>(in.taille);
  Field(out, 1) = cxx2lang<value, int>(in.min_coord);
  Field(out, 2) = cxx2lang<value, int>(in.max_coord);
  return out;
}

template <>
taille_terrain lang2cxx<value, taille_terrain>(value in)
{
  taille_terrain out;
  out.taille = lang2cxx<value, int>(Field(in, 0));
  out.min_coord = lang2cxx<value, int>(Field(in, 1));
  out.max_coord = lang2cxx<value, int>(Field(in, 2));
  return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
value cxx2lang<value, caracs>(caracs in)
{
  value out = caml_alloc(2, 0);
  Field(out, 0) = cxx2lang<value, int>(in.pa_init);
  Field(out, 1) = cxx2lang<value, int>(in.portee);
  return out;
}

template <>
caracs lang2cxx<value, caracs>(value in)
{
  caracs out;
  out.pa_init = lang2cxx<value, int>(Field(in, 0));
  out.portee = lang2cxx<value, int>(Field(in, 1));
  return out;
}

///
// Représente une unité sur le terrain.
//
template <>
value cxx2lang<value, unite>(unite in)
{
  value out = caml_alloc(9, 0);
  Field(out, 0) = cxx2lang<value, position>(in.pos);
  Field(out, 1) = cxx2lang<value, bool>(in.ennemi);
  Field(out, 2) = cxx2lang<value, type_unite>(in.type_unite_actuel);
  Field(out, 3) = cxx2lang<value, type_unite>(in.vrai_type_unite);
  Field(out, 4) = cxx2lang<value, int>(in.ko);
  Field(out, 5) = cxx2lang<value, int>(in.pa);
  Field(out, 6) = cxx2lang<value, int>(in.attaques);
  Field(out, 7) = cxx2lang<value, int>(in.attaques_gratuites);
  Field(out, 8) = cxx2lang<value, int>(in.id);
  return out;
}

template <>
unite lang2cxx<value, unite>(value in)
{
  unite out;
  out.pos = lang2cxx<value, position>(Field(in, 0));
  out.ennemi = lang2cxx<value, bool>(Field(in, 1));
  out.type_unite_actuel = lang2cxx<value, type_unite>(Field(in, 2));
  out.vrai_type_unite = lang2cxx<value, type_unite>(Field(in, 3));
  out.ko = lang2cxx<value, int>(Field(in, 4));
  out.pa = lang2cxx<value, int>(Field(in, 5));
  out.attaques = lang2cxx<value, int>(Field(in, 6));
  out.attaques_gratuites = lang2cxx<value, int>(Field(in, 7));
  out.id = lang2cxx<value, int>(Field(in, 8));
  return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
value cxx2lang<value, cartes>(cartes in)
{
  value out = caml_alloc(4, 0);
  Field(out, 0) = cxx2lang<value, int>(in.potion);
  Field(out, 1) = cxx2lang<value, int>(in.deguisement);
  Field(out, 2) = cxx2lang<value, int>(in.banzai);
  Field(out, 3) = cxx2lang<value, int>(in.pacifisme);
  return out;
}

template <>
cartes lang2cxx<value, cartes>(value in)
{
  cartes out;
  out.potion = lang2cxx<value, int>(Field(in, 0));
  out.deguisement = lang2cxx<value, int>(Field(in, 1));
  out.banzai = lang2cxx<value, int>(Field(in, 2));
  out.pacifisme = lang2cxx<value, int>(Field(in, 3));
  return out;
}

///
// Renvoie le nombre de points de commandements.
//
extern "C" value ml_nombre_pc(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, int>(api_nombre_pc())));
}

///
// Renvoie le nombre d'unités en jeu.
//
extern "C" value ml_nombre_unites(value ennemi)
{
  CAMLparam0();
  CAMLxparam1(ennemi);

  CAMLreturn((cxx2lang<value, int>(api_nombre_unites(lang2cxx<value, bool>(ennemi)))));
}

///
// Renvoie le numéro du tour actuel.
//
extern "C" value ml_tour_actuel(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, int>(api_tour_actuel())));
}

///
// Renvoie la position du spawn (ennemi ou non).
//
extern "C" value ml_pos_renfort(value ennemi)
{
  CAMLparam0();
  CAMLxparam1(ennemi);

  CAMLreturn((cxx2lang<value, position>(api_pos_renfort(lang2cxx<value, bool>(ennemi)))));
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
extern "C" value ml_caracteristiques(value tu)
{
  CAMLparam0();
  CAMLxparam1(tu);

  CAMLreturn((cxx2lang<value, caracs>(api_caracteristiques(lang2cxx<value, type_unite>(tu)))));
}

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
extern "C" value ml_mes_cartes(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, cartes>(api_mes_cartes())));
}

///
// Retourne la liste des unités actuellement en jeu.
//
extern "C" value ml_unites(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang_array<unite>(api_unites())));
}

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
extern "C" value ml_taille_terrain_actuelle(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, taille_terrain>(api_taille_terrain_actuelle())));
}

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
extern "C" value ml_potion(value cible)
{
  CAMLparam0();
  CAMLxparam1(cible);

  CAMLreturn((cxx2lang<value, erreur>(api_potion(lang2cxx<value, position>(cible)))));
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
extern "C" value ml_deguisement(value cible, value nouveau_type)
{
  CAMLparam0();
  CAMLxparam1(cible);
  CAMLxparam1(nouveau_type);

  CAMLreturn((cxx2lang<value, erreur>(api_deguisement(lang2cxx<value, position>(cible), lang2cxx<value, type_unite>(nouveau_type)))));
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
extern "C" value ml_banzai(value cible)
{
  CAMLparam0();
  CAMLxparam1(cible);

  CAMLreturn((cxx2lang<value, erreur>(api_banzai(lang2cxx<value, position>(cible)))));
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
extern "C" value ml_pacifisme(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, erreur>(api_pacifisme())));
}

///
// Déplace une unité vers une position à portée.
//
extern "C" value ml_deplacer(value cible, value pos)
{
  CAMLparam0();
  CAMLxparam1(cible);
  CAMLxparam1(pos);

  CAMLreturn((cxx2lang<value, erreur>(api_deplacer(lang2cxx<value, position>(cible), lang2cxx<value, position>(pos)))));
}

///
// Relève une unité n'ayant plus de marqueurs de KO.
//
extern "C" value ml_relever(value cible)
{
  CAMLparam0();
  CAMLxparam1(cible);

  CAMLreturn((cxx2lang<value, erreur>(api_relever(lang2cxx<value, position>(cible)))));
}

///
// Attaque une autre unité.
//
extern "C" value ml_attaquer(value attaquant, value cible)
{
  CAMLparam0();
  CAMLxparam1(attaquant);
  CAMLxparam1(cible);

  CAMLreturn((cxx2lang<value, erreur>(api_attaquer(lang2cxx<value, position>(attaquant), lang2cxx<value, position>(cible)))));
}

///
// Fait apparaitre une unité sur la case de spawn.
//
extern "C" value ml_renfort(value quoi)
{
  CAMLparam0();
  CAMLxparam1(quoi);

  CAMLreturn((cxx2lang<value, erreur>(api_renfort(lang2cxx<value, type_unite>(quoi)))));
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
extern "C" value ml_annuler(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);

  CAMLreturn((cxx2lang<value, bool>(api_annuler())));
}

///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" value ml_afficher_erreur(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_erreur(lang2cxx<value, erreur>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type type_unite
//
extern "C" value ml_afficher_type_unite(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_type_unite(lang2cxx<value, type_unite>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type position
//
extern "C" value ml_afficher_position(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_position(lang2cxx<value, position>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type taille_terrain
//
extern "C" value ml_afficher_taille_terrain(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_taille_terrain(lang2cxx<value, taille_terrain>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type caracs
//
extern "C" value ml_afficher_caracs(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_caracs(lang2cxx<value, caracs>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type unite
//
extern "C" value ml_afficher_unite(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_unite(lang2cxx<value, unite>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type cartes
//
extern "C" value ml_afficher_cartes(value v)
{
  CAMLparam0();
  CAMLxparam1(v);

  api_afficher_cartes(lang2cxx<value, cartes>(v));
  CAMLreturn(Val_unit);
}

///
// Fonction appellée au début de la partie.
//
void init_game()
{
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_init_game");
  value _ret = callback(*closure, Val_unit);
  return;
}


///
// Fonction appellée pour la phase de retrait de KO.
//
position retirer_ko()
{
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_retirer_ko");
  value _ret = callback(*closure, Val_unit);
  return lang2cxx<value, position>(_ret);
}


///
// Fonction appellée pour la phase de jeu.
//
void jouer()
{
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_jouer");
  value _ret = callback(*closure, Val_unit);
  return;
}


///
// Fonction appellée à la fin de la partie.
//
void end_game()
{
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_end_game");
  value _ret = callback(*closure, Val_unit);
  return;
}


