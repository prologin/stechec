///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_c.rb
//

#include "interface.hh"
#include <cstdlib>

template<typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.error;
}

template<>
std_string lang2cxx<charp, std_string>(charp in)
{
  return in;
}

template<>
int lang2cxx<int, int>(int in)
{
  return in;
}

template<>
bool lang2cxx<int, bool>(int in)
{
  return in;
}

template<typename Lang, typename Cxx>
void lang2cxx_array(Lang* tab, size_t len, std::vector<Cxx>& vect)
{
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back( lang2cxx<Lang, Cxx>(tab[i]) );
}

template<typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.error;
}

template<>
charp cxx2lang<charp, std_string>(std_string in)
{
  size_t l = in.length();
  char * out = (char *) malloc(l + 1);
  for (int i = 0; i < l; i++) out[i] = in[i];
  out[l] = 0;
  return out;
}

template<>
int cxx2lang<int, bool>(bool in)
{
  return in;
}

template<>
int cxx2lang<int, int>(int in)
{
  return in;
}

template<typename Lang, typename Cxx>
void cxx2lang_array(Lang** tab, size_t* len, const std::vector<Cxx>& vect)
{
  *len = vect.size();
  *tab = (Lang *)malloc((*len) * sizeof(Lang));
  for (int i = 0; i < *len; ++i)
    (*tab)[i] = cxx2lang<Lang, Cxx>(vect[i]);
}
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
bonus lang2cxx<bonus, bonus>(bonus in) {
 return in;
}
template<>
bonus cxx2lang<bonus, bonus>(bonus in) {
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
lang2cxx_array<position, __internal__cxx__position>(in.emplacement_arr, in.emplacement_len, out.emplacement);
out.team = lang2cxx<int, int>(in.team);
 return out;
}
template<>
trainee_moto cxx2lang<trainee_moto, __internal__cxx__trainee_moto>(__internal__cxx__trainee_moto in) {
trainee_moto out;
out.id = cxx2lang<int, int>(in.id);
cxx2lang_array<position, __internal__cxx__position>(out.emplacement_arr, out.emplacement_len, in.emplacement);
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
  _retval = api_mon_equipe();
  return cxx2lang<int, int>(_retval);
}

///
// Retourne les scores de chaque équipe
//
void scores(int** ret_arr, size_t* ret_len)
{
  std::vector<int> _retval;
  _retval = api_scores();
  cxx2lang_array<int, int>(ret_arr, ret_len, _retval);
}

///
// Retourne le nombre d'équipes sur le terrain
//
int nombre_equipes(void)
{
  int _retval;
  _retval = api_nombre_equipes();
  return cxx2lang<int, int>(_retval);
}

///
// Retourne le numéro du tour actuel
//
int tour_actuel(void)
{
  int _retval;
  _retval = api_tour_actuel();
  return cxx2lang<int, int>(_retval);
}

///
// Retourne la liste des sources d'énergie
//
void sources_energie(source_energie** ret_arr, size_t* ret_len)
{
  std::vector<__internal__cxx__source_energie> _retval;
  _retval = api_sources_energie();
  cxx2lang_array<source_energie, __internal__cxx__source_energie>(ret_arr, ret_len, _retval);
}

///
// Retourne la liste des traînées de moto
//
void trainees_moto(trainee_moto** ret_arr, size_t* ret_len)
{
  std::vector<__internal__cxx__trainee_moto> _retval;
  _retval = api_trainees_moto();
  cxx2lang_array<trainee_moto, __internal__cxx__trainee_moto>(ret_arr, ret_len, _retval);
}

///
// Retourne le type d'une case
//
type_case regarder_type_case(position pos)
{
  type_case _retval;
  __internal__cxx__position param_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_regarder_type_case(param_pos);
  return cxx2lang<type_case, type_case>(_retval);
}

///
// Retourne le type de bonus d'une case
//
bonus regarder_type_bonus(position pos)
{
  bonus _retval;
  __internal__cxx__position param_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_regarder_type_bonus(param_pos);
  return cxx2lang<bonus, bonus>(_retval);
}

///
// Retourne la liste des bonus d'une équipe
//
void regarder_bonus(int equipe, bonus** ret_arr, size_t* ret_len)
{
  std::vector<bonus> _retval;
  int param_equipe = lang2cxx<int, int>(equipe);
  _retval = api_regarder_bonus(param_equipe);
  cxx2lang_array<bonus, bonus>(ret_arr, ret_len, _retval);
}

///
// Déplace une moto
//
erreur deplacer(int id, position de, position vers)
{
  erreur _retval;
  int param_id = lang2cxx<int, int>(id);
  __internal__cxx__position param_de = lang2cxx<position, __internal__cxx__position>(de);
  __internal__cxx__position param_vers = lang2cxx<position, __internal__cxx__position>(vers);
  _retval = api_deplacer(param_id, param_de, param_vers);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
erreur couper_trainee_moto(int id, position entre, position et)
{
  erreur _retval;
  int param_id = lang2cxx<int, int>(id);
  __internal__cxx__position param_entre = lang2cxx<position, __internal__cxx__position>(entre);
  __internal__cxx__position param_et = lang2cxx<position, __internal__cxx__position>(et);
  _retval = api_couper_trainee_moto(param_id, param_entre, param_et);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Annuler l'action précédente
//
erreur cancel(void)
{
  erreur _retval;
  _retval = api_cancel();
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Enrouler la traînée de moto en un point
//
erreur enrouler(int id, position point)
{
  erreur _retval;
  int param_id = lang2cxx<int, int>(id);
  __internal__cxx__position param_point = lang2cxx<position, __internal__cxx__position>(point);
  _retval = api_enrouler(param_id, param_point);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Régénère une source d'énergie à son maximal
//
erreur regenerer_source_energie(int id)
{
  erreur _retval;
  int param_id = lang2cxx<int, int>(id);
  _retval = api_regenerer_source_energie(param_id);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Allonge le tour en rajoutant des points d'action
//
erreur allonger_pa(void)
{
  erreur _retval;
  _retval = api_allonger_pa();
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
erreur agrandir_trainee_moto(int id, int longueur)
{
  erreur _retval;
  int param_id = lang2cxx<int, int>(id);
  int param_longueur = lang2cxx<int, int>(longueur);
  _retval = api_agrandir_trainee_moto(param_id, param_longueur);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
erreur poser_point_croisement(position point)
{
  erreur _retval;
  __internal__cxx__position param_point = lang2cxx<position, __internal__cxx__position>(point);
  _retval = api_poser_point_croisement(param_point);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
erreur fusionner(int id1, position pos1, int id2, position pos2)
{
  erreur _retval;
  int param_id1 = lang2cxx<int, int>(id1);
  __internal__cxx__position param_pos1 = lang2cxx<position, __internal__cxx__position>(pos1);
  int param_id2 = lang2cxx<int, int>(id2);
  __internal__cxx__position param_pos2 = lang2cxx<position, __internal__cxx__position>(pos2);
  _retval = api_fusionner(param_id1, param_pos1, param_id2, param_pos2);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Affiche le contenu d'une valeur de type erreur
//
void afficher_erreur(erreur v)
{
  erreur param_v = lang2cxx<erreur, erreur>(v);
api_afficher_erreur(param_v);
}

///
// Affiche le contenu d'une valeur de type type_case
//
void afficher_type_case(type_case v)
{
  type_case param_v = lang2cxx<type_case, type_case>(v);
api_afficher_type_case(param_v);
}

///
// Affiche le contenu d'une valeur de type bonus
//
void afficher_bonus(bonus v)
{
  bonus param_v = lang2cxx<bonus, bonus>(v);
api_afficher_bonus(param_v);
}

///
// Affiche le contenu d'une valeur de type position
//
void afficher_position(position v)
{
  __internal__cxx__position param_v = lang2cxx<position, __internal__cxx__position>(v);
api_afficher_position(param_v);
}

///
// Affiche le contenu d'une valeur de type source_energie
//
void afficher_source_energie(source_energie v)
{
  __internal__cxx__source_energie param_v = lang2cxx<source_energie, __internal__cxx__source_energie>(v);
api_afficher_source_energie(param_v);
}

///
// Affiche le contenu d'une valeur de type trainee_moto
//
void afficher_trainee_moto(trainee_moto v)
{
  __internal__cxx__trainee_moto param_v = lang2cxx<trainee_moto, __internal__cxx__trainee_moto>(v);
api_afficher_trainee_moto(param_v);
}

}
