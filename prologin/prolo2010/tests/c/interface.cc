///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_c.rb
//

#include "interface.hh"

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
out.ennemi = lang2cxx<int, bool>(in.ennemi);
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
out.ennemi = cxx2lang<int, bool>(in.ennemi);
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
  _retval = api_nombre_pc();
  return cxx2lang<int, int>(_retval);
}

///
// Renvoie le nombre d'unités en jeu.
//
int nombre_unites(int ennemi)
{
  int _retval;
  bool param_ennemi = lang2cxx<int, bool>(ennemi);
  _retval = api_nombre_unites(param_ennemi);
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
// Renvoie la position du spawn (ennemi ou non).
//
position pos_renfort(int ennemi)
{
  __internal__cxx__position _retval;
  bool param_ennemi = lang2cxx<int, bool>(ennemi);
  _retval = api_pos_renfort(param_ennemi);
  return cxx2lang<position, __internal__cxx__position>(_retval);
}

///
// Renvoie les caractéristiques d'un type d'unité.
//
caracs caracteristiques(type_unite tu)
{
  __internal__cxx__caracs _retval;
  type_unite param_tu = lang2cxx<type_unite, type_unite>(tu);
  _retval = api_caracteristiques(param_tu);
  return cxx2lang<caracs, __internal__cxx__caracs>(_retval);
}

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
cartes mes_cartes(void)
{
  __internal__cxx__cartes _retval;
  _retval = api_mes_cartes();
  return cxx2lang<cartes, __internal__cxx__cartes>(_retval);
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
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
taille_terrain taille_terrain_actuelle(void)
{
  __internal__cxx__taille_terrain _retval;
  _retval = api_taille_terrain_actuelle();
  return cxx2lang<taille_terrain, __internal__cxx__taille_terrain>(_retval);
}

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
erreur potion(position cible)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_potion(param_cible);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
erreur deguisement(position cible, type_unite nouveau_type)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  type_unite param_nouveau_type = lang2cxx<type_unite, type_unite>(nouveau_type);
  _retval = api_deguisement(param_cible, param_nouveau_type);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
erreur banzai(position cible)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_banzai(param_cible);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
erreur pacifisme(void)
{
  erreur _retval;
  _retval = api_pacifisme();
  return cxx2lang<erreur, erreur>(_retval);
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
// Relève une unité n'ayant plus de marqueurs de KO.
//
erreur relever(position cible)
{
  erreur _retval;
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_relever(param_cible);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Attaque une autre unité.
//
erreur attaquer(position attaquant, position cible)
{
  erreur _retval;
  __internal__cxx__position param_attaquant = lang2cxx<position, __internal__cxx__position>(attaquant);
  __internal__cxx__position param_cible = lang2cxx<position, __internal__cxx__position>(cible);
  _retval = api_attaquer(param_attaquant, param_cible);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Fait apparaitre une unité sur la case de spawn.
//
erreur renfort(type_unite quoi)
{
  erreur _retval;
  type_unite param_quoi = lang2cxx<type_unite, type_unite>(quoi);
  _retval = api_renfort(param_quoi);
  return cxx2lang<erreur, erreur>(_retval);
}

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
int annuler(void)
{
  bool _retval;
  _retval = api_annuler();
  return cxx2lang<int, bool>(_retval);
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
// Affiche le contenu d'une valeur de type type_unite
//
void afficher_type_unite(type_unite v)
{
  type_unite param_v = lang2cxx<type_unite, type_unite>(v);
api_afficher_type_unite(param_v);
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
// Affiche le contenu d'une valeur de type taille_terrain
//
void afficher_taille_terrain(taille_terrain v)
{
  __internal__cxx__taille_terrain param_v = lang2cxx<taille_terrain, __internal__cxx__taille_terrain>(v);
api_afficher_taille_terrain(param_v);
}

///
// Affiche le contenu d'une valeur de type caracs
//
void afficher_caracs(caracs v)
{
  __internal__cxx__caracs param_v = lang2cxx<caracs, __internal__cxx__caracs>(v);
api_afficher_caracs(param_v);
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
// Affiche le contenu d'une valeur de type cartes
//
void afficher_cartes(cartes v)
{
  __internal__cxx__cartes param_v = lang2cxx<cartes, __internal__cxx__cartes>(v);
api_afficher_cartes(param_v);
}

}
