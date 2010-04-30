///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_pascal.rb
//

#include "interface.hh"
#include <stdio.h>
#include <cstdlib>

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
extern "C"{
  void fpc_dynarray_setlength(void *, void *, int, void *);
  void fpc_dynarray_length(void *);
}

extern int INIT_PROLO_INTERFACE_ARRAY_OF_CINT;
///
// Représente une position sur le terrain du jeu.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_POSITION;

///
// Contient les informations sur la taille du terrain du jeu.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_TAILLE_TERRAIN;

///
// Donne les caractéristiques d'un type d'unité.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_CARACS;

///
// Représente une unité sur le terrain.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_UNITE;

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_CARTES;

///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_ERREUR;

///
// Le type d'une unité sur le terrain
//
extern int INIT_PROLO_INTERFACE_ARRAY_OF_TYPE_UNITE;

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
// Le type d'une unité sur le terrain
//
template<>
type_unite lang2cxx<type_unite, type_unite>(type_unite in) {
 return in;
}
template<>
type_unite cxx2lang<type_unite, type_unite>(type_unite in) {
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
// Contient les informations sur la taille du terrain du jeu.
//
template<>
__internal__cxx__taille_terrain lang2cxx<taille_terrain, __internal__cxx__taille_terrain>(taille_terrain in) {
__internal__cxx__taille_terrain out;
out.taille = lang2cxx<int, int>(in.taille);
out.min_coord = lang2cxx<int, int>(in.min_coord);
out.max_coord = lang2cxx<int, int>(in.max_coord);
 return out;
}
template<>
taille_terrain cxx2lang<taille_terrain, __internal__cxx__taille_terrain>(__internal__cxx__taille_terrain in) {
taille_terrain out;
out.taille = cxx2lang<int, int>(in.taille);
out.min_coord = cxx2lang<int, int>(in.min_coord);
out.max_coord = cxx2lang<int, int>(in.max_coord);
 return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template<>
__internal__cxx__caracs lang2cxx<caracs, __internal__cxx__caracs>(caracs in) {
__internal__cxx__caracs out;
out.pa_init = lang2cxx<int, int>(in.pa_init);
out.portee = lang2cxx<int, int>(in.portee);
 return out;
}
template<>
caracs cxx2lang<caracs, __internal__cxx__caracs>(__internal__cxx__caracs in) {
caracs out;
out.pa_init = cxx2lang<int, int>(in.pa_init);
out.portee = cxx2lang<int, int>(in.portee);
 return out;
}

///
// Représente une unité sur le terrain.
//
template<>
__internal__cxx__unite lang2cxx<unite, __internal__cxx__unite>(unite in) {
__internal__cxx__unite out;
out.pos = lang2cxx<position, __internal__cxx__position>(in.pos);
out.ennemi = lang2cxx<bool, bool>(in.ennemi);
out.type_unite_actuel = lang2cxx<type_unite, type_unite>(in.type_unite_actuel);
out.vrai_type_unite = lang2cxx<type_unite, type_unite>(in.vrai_type_unite);
out.ko = lang2cxx<int, int>(in.ko);
out.pa = lang2cxx<int, int>(in.pa);
out.attaques = lang2cxx<int, int>(in.attaques);
out.attaques_gratuites = lang2cxx<int, int>(in.attaques_gratuites);
out.id = lang2cxx<int, int>(in.id);
 return out;
}
template<>
unite cxx2lang<unite, __internal__cxx__unite>(__internal__cxx__unite in) {
unite out;
out.pos = cxx2lang<position, __internal__cxx__position>(in.pos);
out.ennemi = cxx2lang<bool, bool>(in.ennemi);
out.type_unite_actuel = cxx2lang<type_unite, type_unite>(in.type_unite_actuel);
out.vrai_type_unite = cxx2lang<type_unite, type_unite>(in.vrai_type_unite);
out.ko = cxx2lang<int, int>(in.ko);
out.pa = cxx2lang<int, int>(in.pa);
out.attaques = cxx2lang<int, int>(in.attaques);
out.attaques_gratuites = cxx2lang<int, int>(in.attaques_gratuites);
out.id = cxx2lang<int, int>(in.id);
 return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template<>
__internal__cxx__cartes lang2cxx<cartes, __internal__cxx__cartes>(cartes in) {
__internal__cxx__cartes out;
out.potion = lang2cxx<int, int>(in.potion);
out.deguisement = lang2cxx<int, int>(in.deguisement);
out.banzai = lang2cxx<int, int>(in.banzai);
out.pacifisme = lang2cxx<int, int>(in.pacifisme);
 return out;
}
template<>
cartes cxx2lang<cartes, __internal__cxx__cartes>(__internal__cxx__cartes in) {
cartes out;
out.potion = cxx2lang<int, int>(in.potion);
out.deguisement = cxx2lang<int, int>(in.deguisement);
out.banzai = cxx2lang<int, int>(in.banzai);
out.pacifisme = cxx2lang<int, int>(in.pacifisme);
 return out;
}

extern "C" {
///
// Renvoie le nombre de points de commandements.
//
int nombre_pc(void)
{
  int _retval;
  _retval = api_nombre_pc(  );
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le nombre d'unités en jeu.
//
int nombre_unites(bool ennemi)
{
  int _retval;
  bool arg_ennemi = lang2cxx<bool, bool>(ennemi);
  _retval = api_nombre_unites( arg_ennemi );
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le numéro du tour actuel.
//
int tour_actuel(void)
{
  int _retval;
  _retval = api_tour_actuel(  );
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie la position du spawn (ennemi ou non).
//
position pos_renfort(bool ennemi)
{
  __internal__cxx__position _retval;
  bool arg_ennemi = lang2cxx<bool, bool>(ennemi);
  _retval = api_pos_renfort( arg_ennemi );
  return cxx2lang<position, __internal__cxx__position>(_retval);
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
caracs caracteristiques(type_unite tu)
{
  __internal__cxx__caracs _retval;
  type_unite arg_tu = lang2cxx<type_unite, type_unite>(tu);
  _retval = api_caracteristiques( arg_tu );
  return cxx2lang<caracs, __internal__cxx__caracs>(_retval);
}

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
cartes mes_cartes(void)
{
  __internal__cxx__cartes _retval;
  _retval = api_mes_cartes(  );
  return cxx2lang<cartes, __internal__cxx__cartes>(_retval);
}

///
// Retourne la liste des unités actuellement en jeu.
//
unite* unites(void)
{
  std::vector<__internal__cxx__unite> _retval;
  _retval = api_unites(  );
  return cxx2lang_array<unite, __internal__cxx__unite>(_retval, &INIT_PROLO_INTERFACE_ARRAY_OF_UNITE);
}

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
taille_terrain taille_terrain_actuelle(void)
{
  __internal__cxx__taille_terrain _retval;
  _retval = api_taille_terrain_actuelle(  );
  return cxx2lang<taille_terrain, __internal__cxx__taille_terrain>(_retval);
}

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
erreur potion(position cible)
{
  erreur _retval;
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_potion( arg_cible );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur deguisement(position cible, type_unite nouveau_type)
{
  erreur _retval;
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  type_unite arg_nouveau_type = lang2cxx<type_unite, type_unite>(nouveau_type);
  _retval = api_deguisement( arg_cible, arg_nouveau_type );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur banzai(position cible)
{
  erreur _retval;
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_banzai( arg_cible );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur pacifisme(void)
{
  erreur _retval;
  _retval = api_pacifisme(  );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Déplace une unité vers une position à portée.
//
erreur deplacer(position cible, position pos)
{
  erreur _retval;
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  __internal__cxx__position arg_pos = lang2cxx<position, __internal__cxx__position>(pos);
  _retval = api_deplacer( arg_cible, arg_pos );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Relève une unité n'ayant plus de marqueurs de KO.
//
erreur relever(position cible)
{
  erreur _retval;
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_relever( arg_cible );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Attaque une autre unité.
//
erreur attaquer(position attaquant, position cible)
{
  erreur _retval;
  __internal__cxx__position arg_attaquant = lang2cxx<position, __internal__cxx__position>(attaquant);
  __internal__cxx__position arg_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_attaquer( arg_attaquant, arg_cible );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur renfort(type_unite quoi)
{
  erreur _retval;
  type_unite arg_quoi = lang2cxx<type_unite, type_unite>(quoi);
  _retval = api_renfort( arg_quoi );
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
bool annuler(void)
{
  bool _retval;
  _retval = api_annuler(  );
  return cxx2lang<bool, bool>(_retval);
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
// Affiche le contenu d'une valeur de type type_unite
//
void afficher_type_unite(type_unite v)
{
  type_unite arg_v = lang2cxx<type_unite, type_unite>(v);
api_afficher_type_unite( arg_v );
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
// Affiche le contenu d'une valeur de type taille_terrain
//
void afficher_taille_terrain(taille_terrain v)
{
  __internal__cxx__taille_terrain arg_v = lang2cxx<taille_terrain, __internal__cxx__taille_terrain>(v);
api_afficher_taille_terrain( arg_v );
}

///
// Affiche le contenu d'une valeur de type caracs
//
void afficher_caracs(caracs v)
{
  __internal__cxx__caracs arg_v = lang2cxx<caracs, __internal__cxx__caracs>(v);
api_afficher_caracs( arg_v );
}

///
// Affiche le contenu d'une valeur de type unite
//
void afficher_unite(unite v)
{
  __internal__cxx__unite arg_v = lang2cxx<unite, __internal__cxx__unite>(v);
api_afficher_unite( arg_v );
}

///
// Affiche le contenu d'une valeur de type cartes
//
void afficher_cartes(cartes v)
{
  __internal__cxx__cartes arg_v = lang2cxx<cartes, __internal__cxx__cartes>(v);
api_afficher_cartes( arg_v );
}

}
