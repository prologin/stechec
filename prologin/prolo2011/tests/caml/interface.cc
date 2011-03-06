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
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
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
// �num�ration repr�sentant les diff�rents types de case
//
template <>
value cxx2lang<value, type_case>(type_case in)
{
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template <>
type_case lang2cxx<value, type_case>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(type_case, (type_case)Int_val(in));
}

///
// �num�ration repr�sentant les diff�rents types de bonii
//
template <>
value cxx2lang<value, type_bonus>(type_bonus in)
{
  CAMLparam0();
  CAMLreturn(Val_int(in));
}

template <>
type_bonus lang2cxx<value, type_bonus>(value in)
{
  CAMLparam1(in);
  CAMLreturnT(type_bonus, (type_bonus)Int_val(in));
}

///
// Repr�sente une position sur le terrain du jeu
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
// Caracteristiques d'une source d'�nergie
//
template <>
value cxx2lang<value, source_energie>(source_energie in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(3, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, int>(in.id));
  caml_initialize(&Field(out, 1), cxx2lang<value, position>(in.pos));
  caml_initialize(&Field(out, 2), cxx2lang<value, int>(in.coef));
  CAMLreturn(out);
}

template <>
source_energie lang2cxx<value, source_energie>(value in)
{
  CAMLparam1(in);
  source_energie out;
  out.id = lang2cxx<value, int>(Field(in, 0));
  out.pos = lang2cxx<value, position>(Field(in, 1));
  out.coef = lang2cxx<value, int>(Field(in, 2));
  CAMLreturnT(source_energie, out);
}

///
// Repr�sente une tra�n�e de moto sur le terrain
//
template <>
value cxx2lang<value, trainee_moto>(trainee_moto in)
{
  CAMLparam0();
  CAMLlocal1(out);
  out = caml_alloc(3, 0);
  caml_initialize(&Field(out, 0), cxx2lang<value, int>(in.id));
  caml_initialize(&Field(out, 1), cxx2lang_array(in.emplacement));
  caml_initialize(&Field(out, 2), cxx2lang<value, int>(in.team));
  CAMLreturn(out);
}

template <>
trainee_moto lang2cxx<value, trainee_moto>(value in)
{
  CAMLparam1(in);
  trainee_moto out;
  out.id = lang2cxx<value, int>(Field(in, 0));
  out.emplacement = lang2cxx_array<position>(Field(in, 1));
  out.team = lang2cxx<value, int>(Field(in, 2));
  CAMLreturnT(trainee_moto, out);
}

///
// Retourne le num�ro de votre �quipe
//
extern "C" value ml_mon_equipe(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, int>(api_mon_equipe())));
}

///
// Retourne les scores de chaque �quipe
//
extern "C" value ml_scores(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang_array<int>(api_scores())));
}

///
// Retourne le nombre d'�quipes sur le terrain
//
extern "C" value ml_nombre_equipes(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, int>(api_nombre_equipes())));
}

///
// Retourne le num�ro du tour actuel
//
extern "C" value ml_tour_actuel(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, int>(api_tour_actuel())));
}

///
// Retourne la liste des sources d'�nergie
//
extern "C" value ml_sources_energie(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang_array<source_energie>(api_sources_energie())));
}

///
// Retourne la liste des tra�n�es de moto
//
extern "C" value ml_trainees_moto(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang_array<trainee_moto>(api_trainees_moto())));
}

///
// Retourne le type d'une case
//
extern "C" value ml_regarder_type_case(value pos)
{
  CAMLparam0();
  CAMLxparam1(pos);
  CAMLreturn((cxx2lang<value, type_case>(api_regarder_type_case(lang2cxx<value, position>(pos)))));
}

///
// Retourne le type de bonus d'une case
//
extern "C" value ml_regarder_type_bonus(value pos)
{
  CAMLparam0();
  CAMLxparam1(pos);
  CAMLreturn((cxx2lang<value, type_bonus>(api_regarder_type_bonus(lang2cxx<value, position>(pos)))));
}

///
// Retourne la liste des bonus d'une �quipe
//
extern "C" value ml_regarder_bonus(value equipe)
{
  CAMLparam0();
  CAMLxparam1(equipe);
  CAMLreturn((cxx2lang_array<type_bonus>(api_regarder_bonus(lang2cxx<value, int>(equipe)))));
}

///
// D�place une moto
//
extern "C" value ml_deplacer(value id, value de, value vers)
{
  CAMLparam0();
  CAMLxparam3(id, de, vers);
  CAMLreturn((cxx2lang<value, erreur>(api_deplacer(lang2cxx<value, int>(id), lang2cxx<value, position>(de), lang2cxx<value, position>(vers)))));
}

///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
extern "C" value ml_couper_trainee_moto(value id, value entre, value et)
{
  CAMLparam0();
  CAMLxparam3(id, entre, et);
  CAMLreturn((cxx2lang<value, erreur>(api_couper_trainee_moto(lang2cxx<value, int>(id), lang2cxx<value, position>(entre), lang2cxx<value, position>(et)))));
}

///
// Annuler l'action pr�c�dente
//
extern "C" value ml_cancel(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, erreur>(api_cancel())));
}

///
// Enrouler la tra�n�e de moto en un point
//
extern "C" value ml_enrouler(value id, value point)
{
  CAMLparam0();
  CAMLxparam2(id, point);
  CAMLreturn((cxx2lang<value, erreur>(api_enrouler(lang2cxx<value, int>(id), lang2cxx<value, position>(point)))));
}

///
// R�g�n�re une source d'�nergie � son maximal
//
extern "C" value ml_regenerer_source_energie(value id)
{
  CAMLparam0();
  CAMLxparam1(id);
  CAMLreturn((cxx2lang<value, erreur>(api_regenerer_source_energie(lang2cxx<value, int>(id)))));
}

///
// Allonge le tour en rajoutant des points d'action
//
extern "C" value ml_allonger_pa(value unit)
{
  CAMLparam0();
  CAMLxparam1(unit);
  CAMLreturn((cxx2lang<value, erreur>(api_allonger_pa())));
}

///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
extern "C" value ml_agrandir_trainee_moto(value id, value longueur)
{
  CAMLparam0();
  CAMLxparam2(id, longueur);
  CAMLreturn((cxx2lang<value, erreur>(api_agrandir_trainee_moto(lang2cxx<value, int>(id), lang2cxx<value, int>(longueur)))));
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
extern "C" value ml_poser_point_croisement(value point)
{
  CAMLparam0();
  CAMLxparam1(point);
  CAMLreturn((cxx2lang<value, erreur>(api_poser_point_croisement(lang2cxx<value, position>(point)))));
}

///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
extern "C" value ml_fusionner(value id1, value pos1, value id2, value pos2)
{
  CAMLparam0();
  CAMLxparam4(id1, pos1, id2, pos2);
  CAMLreturn((cxx2lang<value, erreur>(api_fusionner(lang2cxx<value, int>(id1), lang2cxx<value, position>(pos1), lang2cxx<value, int>(id2), lang2cxx<value, position>(pos2)))));
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
// Affiche le contenu d'une valeur de type type_case
//
extern "C" value ml_afficher_type_case(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_type_case(lang2cxx<value, type_case>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type type_bonus
//
extern "C" value ml_afficher_type_bonus(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_type_bonus(lang2cxx<value, type_bonus>(v));
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
// Affiche le contenu d'une valeur de type source_energie
//
extern "C" value ml_afficher_source_energie(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_source_energie(lang2cxx<value, source_energie>(v));
  CAMLreturn(Val_unit);
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
extern "C" value ml_afficher_trainee_moto(value v)
{
  CAMLparam0();
  CAMLxparam1(v);
  api_afficher_trainee_moto(lang2cxx<value, trainee_moto>(v));
  CAMLreturn(Val_unit);
}

///
// Fonction appell�e au d�but de la partie
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
// Fonction appell�e pour la phase de jeu
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
// Fonction appell�e � la fin de la partie
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
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type type_bonus
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type source_energie
//

///
// Affiche le contenu d'une valeur de type trainee_moto
//

