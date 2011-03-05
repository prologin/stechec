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
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template<>
value cxx2lang<value, std::string>(std::string in)
{
  CAMLparam0();
  size_t l = in.length();
  char * out = (char *) malloc(l + 1);
  for (int i = 0; i < l; i++) out[i] = in[i];
  out[l] = 0;
  CAMLreturn(caml_copy_string(out));
}

template <>
value cxx2lang<value, bool>(bool in)
{
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template <typename Cxx>
value cxx2lang_array(const std::vector<Cxx>& in)
{
  CAMLparam0();
  CAMLlocal1(v);

  size_t size = in.size();
  if (size == 0)
    CAMLreturn(Atom(0));

  v = caml_alloc(size, 0);
  for (int i = 0; i < size; ++i)
    caml_initialize(&Field(v, i), cxx2lang<value, Cxx>(in[i]));

  CAMLreturn(v);
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template<>
std::string lang2cxx<value, std::string>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(std::string, String_val(in));
}

template <>
int lang2cxx<value, int>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(int, Int_val(in));
}

template <>
bool lang2cxx<value, bool>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(bool, Int_val(in));
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(value in)
{
  CAMLparam1(in);
  std::vector<Cxx> out;
  mlsize_t size = Wosize_val(in);

  for (int i = 0; i < size; ++i)
    out.push_back(lang2cxx<value, Cxx>(Field(in, i)));

  CAMLreturnT(std::vector<Cxx>, out);
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template <>
value cxx2lang<value, erreur>(erreur in)
{
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template <>
erreur lang2cxx<value, erreur>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(erreur, (erreur)Int_val(in));
}

///
// Le type d'un objet
//
template <>
value cxx2lang<value, type_objet>(type_objet in)
{
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template <>
type_objet lang2cxx<value, type_objet>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(type_objet, (type_objet)Int_val(in));
}

///
// Représente une position sur le terrain du jeu.
//
template <>
value cxx2lang<value, position>(position in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(2, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, int>(in.x));
  caml_initialize(&Field(out, 1), cxx2lang<value, int>(in.y));
  CAMLreturn(out);
}

template <>
position lang2cxx<value, position>(value in)
{
  CAMLparam1(in);
  position out;
  out.x = lang2cxx<value, int>(Field(in, 0));
  out.y = lang2cxx<value, int>(Field(in, 1));
  CAMLreturnT(position, out);
}

///
// les caracteristiques d'un objet
//
template <>
value cxx2lang<value, caracteristiques_objet>(caracteristiques_objet in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(2, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, int>(in.cout));
  caml_initialize(&Field(out, 1), cxx2lang<value, int>(in.porte));
  CAMLreturn(out);
}

template <>
caracteristiques_objet lang2cxx<value, caracteristiques_objet>(value in)
{
  CAMLparam1(in);
  caracteristiques_objet out;
  out.cout = lang2cxx<value, int>(Field(in, 0));
  out.porte = lang2cxx<value, int>(Field(in, 1));
  CAMLreturnT(caracteristiques_objet, out);
}

///
// Représente une unité sur le terrain.
//
template <>
value cxx2lang<value, unite>(unite in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(6, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, position>(in.pos_unite));
  caml_initialize(&Field(out, 1), cxx2lang<value, int>(in.team));
  caml_initialize(&Field(out, 2), cxx2lang<value, int>(in.ko));
  caml_initialize(&Field(out, 3), cxx2lang<value, int>(in.pa));
  caml_initialize(&Field(out, 4), cxx2lang<value, type_objet>(in.objet));
  caml_initialize(&Field(out, 5), cxx2lang<value, int>(in.id));
  CAMLreturn(out);
}

template <>
unite lang2cxx<value, unite>(value in)
{
  CAMLparam1(in);
  unite out;
  out.pos_unite = lang2cxx<value, position>(Field(in, 0));
  out.team = lang2cxx<value, int>(Field(in, 1));
  out.ko = lang2cxx<value, int>(Field(in, 2));
  out.pa = lang2cxx<value, int>(Field(in, 3));
  out.objet = lang2cxx<value, type_objet>(Field(in, 4));
  out.id = lang2cxx<value, int>(Field(in, 5));
  CAMLreturnT(unite, out);
}

///
// Représente le type d'une piece.
//
template <>
value cxx2lang<value, piece>(piece in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(3, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, int>(in.valeur));
  caml_initialize(&Field(out, 1), cxx2lang<value, position>(in.pos_piece));
  caml_initialize(&Field(out, 2), cxx2lang<value, int>(in.tour_apparition));
  CAMLreturn(out);
}

template <>
piece lang2cxx<value, piece>(value in)
{
  CAMLparam1(in);
  piece out;
  out.valeur = lang2cxx<value, int>(Field(in, 0));
  out.pos_piece = lang2cxx<value, position>(Field(in, 1));
  out.tour_apparition = lang2cxx<value, int>(Field(in, 2));
  CAMLreturnT(piece, out);
}

///
// Renvoie le numero de votre equipe.
//
extern "C" value ml_mon_equipe(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, int>(api_mon_equipe())));
}

///
// Renvoie le score d'une equipe.
//
extern "C" value ml_score(value team)
{
  CAMLparam0();
  CAMLxparam1(team);
  CAMLreturn((cxx2lang<value, int>(api_score(lang2cxx<value, int>(team)))));
}

///
// Renvoie le nombre d'equipes sur la map
//
extern "C" value ml_nombre_equipes(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, int>(api_nombre_equipes())));
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
// Renvoie les pieces qui sont sur la map
//
extern "C" value ml_pieces_en_jeu(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang_array<piece>(api_pieces_en_jeu())));
}

///
// Renvoie le 10 prochaines pieces a vennir
//
extern "C" value ml_pieces_a_vennir(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang_array<piece>(api_pieces_a_vennir())));
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
// Retourne les caracteristiques de l'objet.
//
extern "C" value ml_proprietes_objet(value to)
{
  CAMLparam0();
  CAMLxparam1(to);
  CAMLreturn((cxx2lang<value, caracteristiques_objet>(api_proprietes_objet(lang2cxx<value, type_objet>(to)))));
}

///
// Déplace une unité vers une position à portée.
//
extern "C" value ml_deplacer(value cible, value pos)
{
  CAMLparam0();
  CAMLxparam2(cible, pos);
  CAMLreturn((cxx2lang<value, erreur>(api_deplacer(lang2cxx<value, position>(cible), lang2cxx<value, position>(pos)))));
}

///
// Achete un objet
//
extern "C" value ml_acheter_objet(value cible, value objet)
{
  CAMLparam0();
  CAMLxparam2(cible, objet);
  CAMLreturn((cxx2lang<value, erreur>(api_acheter_objet(lang2cxx<value, position>(cible), lang2cxx<value, type_objet>(objet)))));
}

///
// utilise un objet
//
extern "C" value ml_utiliser(value attaquant, value cible)
{
  CAMLparam0();
  CAMLxparam2(attaquant, cible);
  CAMLreturn((cxx2lang<value, erreur>(api_utiliser(lang2cxx<value, position>(attaquant), lang2cxx<value, position>(cible)))));
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
extern "C" value ml_achever_la_partie(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, erreur>(api_achever_la_partie())));
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
// Affiche le contenu d'une valeur de type type_objet
//
extern "C" value ml_afficher_type_objet(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_type_objet(lang2cxx<value, type_objet>(v));
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
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
extern "C" value ml_afficher_caracteristiques_objet(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_caracteristiques_objet(lang2cxx<value, caracteristiques_objet>(v));
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
// Affiche le contenu d'une valeur de type piece
//
extern "C" value ml_afficher_piece(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_piece(lang2cxx<value, piece>(v));
  CAMLreturn(Val_unit);
}

///
// Fonction appellée au début de la partie.
//
void init_game()
{
  CAMLparam0();
  CAMLlocal1(_ret);
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_init_game");
  _ret = callback(*closure, Val_unit);
  CAMLreturn0;
}


///
// Fonction appellée pour la phase de jeu.
//
void jouer()
{
  CAMLparam0();
  CAMLlocal1(_ret);
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_jouer");
  _ret = callback(*closure, Val_unit);
  CAMLreturn0;
}


///
// Fonction appellée à la fin de la partie.
//
void end_game()
{
  CAMLparam0();
  CAMLlocal1(_ret);
  static value *closure = NULL;
  if (closure == NULL)
    closure = caml_named_value("ml_end_game");
  _ret = callback(*closure, Val_unit);
  CAMLreturn0;
}


///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_objet
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//

///
// Affiche le contenu d'une valeur de type unite
//

///
// Affiche le contenu d'une valeur de type piece
//

