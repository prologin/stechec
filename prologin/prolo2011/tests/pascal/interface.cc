///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_pascal.rb
//

#include "interface.hh"
#include <cstdio>
#include <cstdlib>


extern "C"{
  void fpc_dynarray_setlength(void *, void *, int, void *);
  int fpc_dynarray_length(void *);
}


template<typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.error;
}

template<>
int lang2cxx<int, int>(int in)
{
  return in;
}
template<>
bool lang2cxx<bool, bool>(bool in)
{
  return in;
}

template<>
std::string lang2cxx<char*, std::string>(char * in)
{
  return in;
}

template<typename Lang, typename Cxx>
std::vector<Cxx> lang2cxx_array(Lang *l)
{
  std::vector<Cxx> vect;
  int len = fpc_dynarray_length(&l);
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back( lang2cxx<Lang, Cxx>(l[i]) );
  return vect;
}

template<typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.err;
}

template<>
char *cxx2lang<char*, std::string>(std::string in)
{
  size_t l = in.length();
  char * out = (char *) malloc(l + 1);
  for (int i = 0; i < l; i++) out[i] = in[i];
  out[l] = 0;
  return out;
}

template<>
int cxx2lang<int, int>(int in)
{
  return in;
}
template<>
bool cxx2lang<bool, bool>(bool in)
{
  return in;
}

template<typename Lang, typename Cxx>
Lang * cxx2lang_array(const std::vector<Cxx>& vect, void *type_ptr)
{
  long len = vect.size();
  Lang * tab = NULL;
  fpc_dynarray_setlength(&tab, type_ptr, 1, &len);
  for (int i = 0; i < len; ++i){
    tab[i] = cxx2lang<Lang, Cxx>(vect[i]);
  }
  return tab;
}
extern int INIT_PROLO_INTERFACE_ARRAY_OF_CINT;
///
// Représente une position sur le terrain du jeu
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_POSITION;

///
// Caracteristiques d'une source d'énergie
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_SOURCE_ENERGIE;

///
// Représente une traînée de moto sur le terrain
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_TRAINEE_MOTO;

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_ERREUR;

///
// Énumération représentant les différents types de case
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_TYPE_CASE;

///
// Énumération représentant les différents types de bonii
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_TYPE_BONUS;

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action
//
template<>
erreur lang2cxx<erreur, erreur>(erreur in) {
 return in;
}
template<>
erreur cxx2lang<erreur, erreur>(erreur in) {
 return in;
}

///
// Énumération représentant les différents types de case
//
template<>
type_case lang2cxx<type_case, type_case>(type_case in) {
 return in;
}
template<>
type_case cxx2lang<type_case, type_case>(type_case in) {
 return in;
}

///
// Énumération représentant les différents types de bonii
//
template<>
type_bonus lang2cxx<type_bonus, type_bonus>(type_bonus in) {
 return in;
}
template<>
type_bonus cxx2lang<type_bonus, type_bonus>(type_bonus in) {
 return in;
}

///
// Représente une position sur le terrain du jeu
//
template<>
__internal__cxx__position lang2cxx<position, __internal__cxx__position>(position in) {
__internal__cxx__position out;
out.x = lang2cxx<int, int>(in.x);
out.y = lang2cxx<int, int>(in.y);
 return out;
}
template<>
position cxx2lang<position, __internal__cxx__position>(__internal__cxx__position in) {
position out;
out.x = cxx2lang<int, int>(in.x);
out.y = cxx2lang<int, int>(in.y);
 return out;
}

///
// Caracteristiques d'une source d'énergie
//
template<>
__internal__cxx__source_energie lang2cxx<source_energie, __internal__cxx__source_energie>(source_energie in) {
__internal__cxx__source_energie out;
out.id = lang2cxx<int, int>(in.id);
out.pos = lang2cxx<position, __internal__cxx__position>(in.pos);
out.coef = lang2cxx<int, int>(in.coef);
 return out;
}
template<>
source_energie cxx2lang<source_energie, __internal__cxx__source_energie>(__internal__cxx__source_energie in) {
source_energie out;
out.id = cxx2lang<int, int>(in.id);
out.pos = cxx2lang<position, __internal__cxx__position>(in.pos);
out.coef = cxx2lang<int, int>(in.coef);
 return out;
}

///
// Représente une traînée de moto sur le terrain
//
template<>
__internal__cxx__trainee_moto lang2cxx<trainee_moto, __internal__cxx__trainee_moto>(trainee_moto in) {
__internal__cxx__trainee_moto out;
out.id = lang2cxx<int, int>(in.id);
out.emplacement = lang2cxx_array<position, __internal__cxx__position>(in.emplacement);
out.team = lang2cxx<int, int>(in.team);
 return out;
}
template<>
trainee_moto cxx2lang<trainee_moto, __internal__cxx__trainee_moto>(__internal__cxx__trainee_moto in) {
trainee_moto out;
out.id = cxx2lang<int, int>(in.id);
out.emplacement = cxx2lang_array<position, __internal__cxx__position>(in.emplacement, &INIT_PROLO_INTERFACE_ARRAY_OF_POSITION);
out.team = cxx2lang<int, int>(in.team);
 return out;
}

extern "C" {
///
// Retourne le numéro de votre équipe
//
int mon_equipe(void)
{
  int _retval;
  _retval = api_mon_equipe(  );
  return cxx2lang<int, int>(_retval);
}

///
// Retourne les scores de chaque équipe
//
int* scores(void)
{
  std::vector<int> _retval;
  _retval = api_scores(  );
  return cxx2lang_array<int, int>(_retval, &INIT_PROLO_INTERFACE_ARRAY_OF_CINT);
}

///
// Retourne le nombre d'équipes sur le terrain
//
int nombre_equipes(void)
{
  int _retval;
  _retval = api_nombre_equipes(  );
  return cxx2lang<int, int>(_retval);
}

///
// Retourne le numéro du tour actuel
//
int tour_actuel(void)
{
  int _retval;
  _retval = api_tour_actuel(  );
  return cxx2lang<int, int>(_retval);
}

///
// Retourne la liste des sources d'énergie
//
source_energie* sources_energie(void)
{
  std::vector<__internal__cxx__source_energie> _retval;
  _retval = api_sources_energie(  );
  return cxx2lang_array<source_energie, __internal__cxx__source_energie>(_retval, &INIT_PROLO_INTERFACE_ARRAY_OF_SOURCE_ENERGIE);
}

///
// Retourne la liste des traînées de moto
//
trainee_moto* trainees_moto(void)
{
  std::vector<__internal__cxx__trainee_moto> _retval;
  _retval = api_trainees_moto(  );
  return cxx2lang_array<trainee_moto, __internal__cxx__trainee_moto>(_retval, &INIT_PROLO_INTERFACE_ARRAY_OF_TRAINEE_MOTO);
}

///
// Retourne le type d'une case
//
type_case regarder_type_case(position pos)
{
  type_case _retval;
  __internal__cxx__position arg_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_regarder_type_case( arg_pos );
  return cxx2lang<type_case, type_case>(_retval);
}

///
// Retourne le type de bonus d'une case
//
type_bonus regarder_type_bonus(position pos)
{
  type_bonus _retval;
  __internal__cxx__position arg_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_regarder_type_bonus( arg_pos );
  return cxx2lang<type_bonus, type_bonus>(_retval);
}

///
// Retourne la liste des bonus d'une équipe
//
type_bonus* regarder_bonus(int equipe)
{
  std::vector<type_bonus> _retval;
  int arg_equipe = lang2cxx<int, int>(equipe);
  _retval = api_regarder_bonus( arg_equipe );
  return cxx2lang_array<type_bonus, type_bonus>(_retval, &INIT_PROLO_INTERFACE_ARRAY_OF_TYPE_BONUS);
}

///
// Déplace une moto
//
erreur deplacer(int id, position de, position vers)
{
  erreur _retval;
  int arg_id = lang2cxx<int, int>(id);
  __internal__cxx__position arg_de = lang2cxx<position, __internal__cxx__position>(de);
  __internal__cxx__position arg_vers = lang2cxx<position, __internal__cxx__position>(vers);
  _retval = api_deplacer( arg_id, arg_de, arg_vers );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
erreur couper_trainee_moto(int id, position entre, position et)
{
  erreur _retval;
  int arg_id = lang2cxx<int, int>(id);
  __internal__cxx__position arg_entre = lang2cxx<position, __internal__cxx__position>(entre);
  __internal__cxx__position arg_et = lang2cxx<position, __internal__cxx__position>(et);
  _retval = api_couper_trainee_moto( arg_id, arg_entre, arg_et );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Annuler l'action précédente
//
erreur cancel(void)
{
  erreur _retval;
  _retval = api_cancel(  );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Enrouler la traînée de moto en un point
//
erreur enrouler(int id, position point)
{
  erreur _retval;
  int arg_id = lang2cxx<int, int>(id);
  __internal__cxx__position arg_point = lang2cxx<position, __internal__cxx__position>(point);
  _retval = api_enrouler( arg_id, arg_point );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Régénère une source d'énergie à son maximal
//
erreur regenerer_source_energie(int id)
{
  erreur _retval;
  int arg_id = lang2cxx<int, int>(id);
  _retval = api_regenerer_source_energie( arg_id );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Allonge le tour en rajoutant des points d'action
//
erreur allonger_pa(void)
{
  erreur _retval;
  _retval = api_allonger_pa(  );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur agrandir_trainee_moto(int id, int longueur)
{
  erreur _retval;
  int arg_id = lang2cxx<int, int>(id);
  int arg_longueur = lang2cxx<int, int>(longueur);
  _retval = api_agrandir_trainee_moto( arg_id, arg_longueur );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
erreur poser_point_croisement(position point)
{
  erreur _retval;
  __internal__cxx__position arg_point = lang2cxx<position, __internal__cxx__position>(point);
  _retval = api_poser_point_croisement( arg_point );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
erreur fusionner(int id1, position pos1, int id2, position pos2)
{
  erreur _retval;
  int arg_id1 = lang2cxx<int, int>(id1);
  __internal__cxx__position arg_pos1 = lang2cxx<position, __internal__cxx__position>(pos1);
  int arg_id2 = lang2cxx<int, int>(id2);
  __internal__cxx__position arg_pos2 = lang2cxx<position, __internal__cxx__position>(pos2);
  _retval = api_fusionner( arg_id1, arg_pos1, arg_id2, arg_pos2 );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Affiche le contenu d'une valeur de type erreur
//
void afficher_erreur(erreur v)
{
  erreur arg_v = lang2cxx<erreur, erreur>(v);
api_afficher_erreur( arg_v );
}

///
// Affiche le contenu d'une valeur de type type_case
//
void afficher_type_case(type_case v)
{
  type_case arg_v = lang2cxx<type_case, type_case>(v);
api_afficher_type_case( arg_v );
}

///
// Affiche le contenu d'une valeur de type type_bonus
//
void afficher_type_bonus(type_bonus v)
{
  type_bonus arg_v = lang2cxx<type_bonus, type_bonus>(v);
api_afficher_type_bonus( arg_v );
}

///
// Affiche le contenu d'une valeur de type position
//
void afficher_position(position v)
{
  __internal__cxx__position arg_v = lang2cxx<position, __internal__cxx__position>(v);
api_afficher_position( arg_v );
}

///
// Affiche le contenu d'une valeur de type source_energie
//
void afficher_source_energie(source_energie v)
{
  __internal__cxx__source_energie arg_v = lang2cxx<source_energie, __internal__cxx__source_energie>(v);
api_afficher_source_energie( arg_v );
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
void afficher_trainee_moto(trainee_moto v)
{
  __internal__cxx__trainee_moto arg_v = lang2cxx<trainee_moto, __internal__cxx__trainee_moto>(v);
api_afficher_trainee_moto( arg_v );
}

}
