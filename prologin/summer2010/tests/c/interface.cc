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
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
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
// Le type d'un objet
//
template<>
type_objet lang2cxx<type_objet, type_objet>(type_objet in) {
 return in;
}
template<>
type_objet cxx2lang<type_objet, type_objet>(type_objet in) {
 return in;
}

///
// Représente une position sur le terrain du jeu.
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
// les caracteristiques d'un objet
//
template<>
__internal__cxx__caracteristiques_objet lang2cxx<caracteristiques_objet, __internal__cxx__caracteristiques_objet>(caracteristiques_objet in) {
__internal__cxx__caracteristiques_objet out;
out.cout = lang2cxx<int, int>(in.cout);
out.porte = lang2cxx<int, int>(in.porte);
 return out;
}
template<>
caracteristiques_objet cxx2lang<caracteristiques_objet, __internal__cxx__caracteristiques_objet>(__internal__cxx__caracteristiques_objet in) {
caracteristiques_objet out;
out.cout = cxx2lang<int, int>(in.cout);
out.porte = cxx2lang<int, int>(in.porte);
 return out;
}

///
// Représente une unité sur le terrain.
//
template<>
__internal__cxx__unite lang2cxx<unite, __internal__cxx__unite>(unite in) {
__internal__cxx__unite out;
out.pos_unite = lang2cxx<position, __internal__cxx__position>(in.pos_unite);
out.team = lang2cxx<int, int>(in.team);
out.ko = lang2cxx<int, int>(in.ko);
out.pa = lang2cxx<int, int>(in.pa);
out.objet = lang2cxx<type_objet, type_objet>(in.objet);
out.id = lang2cxx<int, int>(in.id);
 return out;
}
template<>
unite cxx2lang<unite, __internal__cxx__unite>(__internal__cxx__unite in) {
unite out;
out.pos_unite = cxx2lang<position, __internal__cxx__position>(in.pos_unite);
out.team = cxx2lang<int, int>(in.team);
out.ko = cxx2lang<int, int>(in.ko);
out.pa = cxx2lang<int, int>(in.pa);
out.objet = cxx2lang<type_objet, type_objet>(in.objet);
out.id = cxx2lang<int, int>(in.id);
 return out;
}

///
// Représente le type d'une piece.
//
template<>
__internal__cxx__piece lang2cxx<piece, __internal__cxx__piece>(piece in) {
__internal__cxx__piece out;
out.valeur = lang2cxx<int, int>(in.valeur);
out.pos_piece = lang2cxx<position, __internal__cxx__position>(in.pos_piece);
out.tour_apparition = lang2cxx<int, int>(in.tour_apparition);
 return out;
}
template<>
piece cxx2lang<piece, __internal__cxx__piece>(__internal__cxx__piece in) {
piece out;
out.valeur = cxx2lang<int, int>(in.valeur);
out.pos_piece = cxx2lang<position, __internal__cxx__position>(in.pos_piece);
out.tour_apparition = cxx2lang<int, int>(in.tour_apparition);
 return out;
}

extern "C" {
///
// Renvoie le numero de votre equipe.
//
int mon_equipe(void)
{
  int _retval;
  _retval = api_mon_equipe();
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le score d'une equipe.
//
int score(int team)
{
  int _retval;
  int param_team = lang2cxx<int, int>(team);
  _retval = api_score(param_team);
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le nombre d'equipes sur la map
//
int nombre_equipes(void)
{
  int _retval;
  _retval = api_nombre_equipes();
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le numéro du tour actuel.
//
int tour_actuel(void)
{
  int _retval;
  _retval = api_tour_actuel();
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie les pieces qui sont sur la map
//
void pieces_en_jeu(piece** ret_arr, size_t* ret_len)
{
  std::vector<__internal__cxx__piece> _retval;
  _retval = api_pieces_en_jeu();
  cxx2lang_array<piece, __internal__cxx__piece>(ret_arr, ret_len, _retval);
}

///
// Renvoie le 10 prochaines pieces a vennir
//
void pieces_a_vennir(piece** ret_arr, size_t* ret_len)
{
  std::vector<__internal__cxx__piece> _retval;
  _retval = api_pieces_a_vennir();
  cxx2lang_array<piece, __internal__cxx__piece>(ret_arr, ret_len, _retval);
}

///
// Retourne la liste des unités actuellement en jeu.
//
void unites(unite** ret_arr, size_t* ret_len)
{
  std::vector<__internal__cxx__unite> _retval;
  _retval = api_unites();
  cxx2lang_array<unite, __internal__cxx__unite>(ret_arr, ret_len, _retval);
}

///
// Retourne les caracteristiques de l'objet.
//
caracteristiques_objet proprietes_objet(type_objet to)
{
  __internal__cxx__caracteristiques_objet _retval;
  type_objet param_to = lang2cxx<type_objet, type_objet>(to);
  _retval = api_proprietes_objet(param_to);
  return cxx2lang<caracteristiques_objet, __internal__cxx__caracteristiques_objet>(_retval);
}

///
// Déplace une unité vers une position à portée.
//
erreur deplacer(position cible, position pos)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  __internal__cxx__position param_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_deplacer(param_cible, param_pos);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Achete un objet
//
erreur acheter_objet(position cible, type_objet objet)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  type_objet param_objet = lang2cxx<type_objet, type_objet>(objet);
  _retval = api_acheter_objet(param_cible, param_objet);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// utilise un objet
//
erreur utiliser(position attaquant, position cible)
{
  erreur _retval;
  __internal__cxx__position param_attaquant = lang2cxx<position, __internal__cxx__position>(attaquant);
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_utiliser(param_attaquant, param_cible);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
erreur achever_la_partie(void)
{
  erreur _retval;
  _retval = api_achever_la_partie();
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
// Affiche le contenu d'une valeur de type type_objet
//
void afficher_type_objet(type_objet v)
{
  type_objet param_v = lang2cxx<type_objet, type_objet>(v);
api_afficher_type_objet(param_v);
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
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
void afficher_caracteristiques_objet(caracteristiques_objet v)
{
  __internal__cxx__caracteristiques_objet param_v = lang2cxx<caracteristiques_objet, __internal__cxx__caracteristiques_objet>(v);
api_afficher_caracteristiques_objet(param_v);
}

///
// Affiche le contenu d'une valeur de type unite
//
void afficher_unite(unite v)
{
  __internal__cxx__unite param_v = lang2cxx<unite, __internal__cxx__unite>(v);
api_afficher_unite(param_v);
}

///
// Affiche le contenu d'une valeur de type piece
//
void afficher_piece(piece v)
{
  __internal__cxx__piece param_v = lang2cxx<piece, __internal__cxx__piece>(v);
api_afficher_piece(param_v);
}

}
