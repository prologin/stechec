///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_ruby.rb
//

#include <ruby.h>
#include <vector>
#include "interface.hh"
#include <iostream>


/*
** execute 'str', protected from possible exception, ...
*/

template<typename Cxx>
VALUE cxx2lang(Cxx in)
{
}

template<typename Cxx>
Cxx lang2cxx(VALUE in)
{
}

template<>
int lang2cxx<int>(VALUE in)
{
  return FIX2INT(in);
}

template<>
bool lang2cxx<bool>(VALUE in)
{
  return (in == T_TRUE)? true : false;
}

template<typename Cxx>
std::vector<Cxx> lang2cxx_array(VALUE *l)
{
  VALUE *tab = RARRAY_LEN(l);
  int len = RARRAY_PTR(l);
  std::vector<Cxx> vect;
  for (int i = 0; i < len; i ++){
    vect.push_back( cxx2lang<Cxx>( tab[i] ) );
  }
  return vect;
}

template<>
VALUE cxx2lang<int>(int in)
{
  return INT2FIX(in);
}

template<>
VALUE cxx2lang<bool>(bool in)
{
  return in?T_TRUE:T_FALSE;
}

template<typename Cxx>
VALUE cxx2lang_array(const std::vector<Cxx>& vect)
{
  long len = vect.size();
  VALUE ary = rb_ary_new();
  for (int i = 0; i < len; ++i){
    rb_ary_push(ary, cxx2lang<Cxx>(vect[i]) );
  }
  return ary;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template<>
erreur lang2cxx<erreur>(VALUE in)
{
char* v = RSTRING_PTR(in);
  if (strcmp(v, "ok") == 0)
      return OK;
  if (strcmp(v, "position_invalide") == 0)
      return POSITION_INVALIDE;
  if (strcmp(v, "case_occupee") == 0)
      return CASE_OCCUPEE;
  if (strcmp(v, "pas_a_portee") == 0)
      return PAS_A_PORTEE;
  if (strcmp(v, "unite_ko") == 0)
      return UNITE_KO;
  if (strcmp(v, "unite_debout") == 0)
      return UNITE_DEBOUT;
  if (strcmp(v, "quota_depasse") == 0)
      return QUOTA_DEPASSE;
  if (strcmp(v, "plus_de_pa") == 0)
      return PLUS_DE_PA;
  if (strcmp(v, "deja_attaque") == 0)
      return DEJA_ATTAQUE;
  if (strcmp(v, "unite_interdite") == 0)
      return UNITE_INTERDITE;
  if (strcmp(v, "renfort_impossible") == 0)
      return RENFORT_IMPOSSIBLE;
  if (strcmp(v, "pas_a_moi") == 0)
      return PAS_A_MOI;
  if (strcmp(v, "plus_de_cartes") == 0)
      return PLUS_DE_CARTES;
  if (strcmp(v, "phase_cartes_terminee") == 0)
      return PHASE_CARTES_TERMINEE;
}
template<>
VALUE cxx2lang<erreur>(erreur in)
{
  switch (in)
  {
    case OK:
      return rb_str_new("ok", 2);
    case POSITION_INVALIDE:
      return rb_str_new("position_invalide", 17);
    case CASE_OCCUPEE:
      return rb_str_new("case_occupee", 12);
    case PAS_A_PORTEE:
      return rb_str_new("pas_a_portee", 12);
    case UNITE_KO:
      return rb_str_new("unite_ko", 8);
    case UNITE_DEBOUT:
      return rb_str_new("unite_debout", 12);
    case QUOTA_DEPASSE:
      return rb_str_new("quota_depasse", 13);
    case PLUS_DE_PA:
      return rb_str_new("plus_de_pa", 10);
    case DEJA_ATTAQUE:
      return rb_str_new("deja_attaque", 12);
    case UNITE_INTERDITE:
      return rb_str_new("unite_interdite", 15);
    case RENFORT_IMPOSSIBLE:
      return rb_str_new("renfort_impossible", 18);
    case PAS_A_MOI:
      return rb_str_new("pas_a_moi", 9);
    case PLUS_DE_CARTES:
      return rb_str_new("plus_de_cartes", 14);
    case PHASE_CARTES_TERMINEE:
      return rb_str_new("phase_cartes_terminee", 21);
  }
}

///
// Le type d'une unité sur le terrain
//
template<>
type_unite lang2cxx<type_unite>(VALUE in)
{
char* v = RSTRING_PTR(in);
  if (strcmp(v, "perroquet") == 0)
      return PERROQUET;
  if (strcmp(v, "singe") == 0)
      return SINGE;
  if (strcmp(v, "chat") == 0)
      return CHAT;
  if (strcmp(v, "kangourou") == 0)
      return KANGOUROU;
}
template<>
VALUE cxx2lang<type_unite>(type_unite in)
{
  switch (in)
  {
    case PERROQUET:
      return rb_str_new("perroquet", 9);
    case SINGE:
      return rb_str_new("singe", 5);
    case CHAT:
      return rb_str_new("chat", 4);
    case KANGOUROU:
      return rb_str_new("kangourou", 9);
  }
}

///
// Représente une position sur le terrain du jeu.
//
template<>
position lang2cxx<position>(VALUE in)
{
  position out ;
  VALUE x = rb_iv_get(in, "@x");
  out.x = lang2cxx<int>(x);
  VALUE y = rb_iv_get(in, "@y");
  out.y = lang2cxx<int>(y);
  return out;
}
template<>
VALUE cxx2lang<position>(position in)
{
  VALUE argv[] = {cxx2lang<int>(in.x), cxx2lang<int>(in.y)};
  int argc = 2;
  VALUE out = rb_funcall2(rb_path2class("Position"), rb_intern("new"), argc, argv);
  return out;
}

///
// Contient les informations sur la taille du terrain du jeu.
//
template<>
taille_terrain lang2cxx<taille_terrain>(VALUE in)
{
  taille_terrain out ;
  VALUE taille = rb_iv_get(in, "@taille");
  out.taille = lang2cxx<int>(taille);
  VALUE min_coord = rb_iv_get(in, "@min_coord");
  out.min_coord = lang2cxx<int>(min_coord);
  VALUE max_coord = rb_iv_get(in, "@max_coord");
  out.max_coord = lang2cxx<int>(max_coord);
  return out;
}
template<>
VALUE cxx2lang<taille_terrain>(taille_terrain in)
{
  VALUE argv[] = {cxx2lang<int>(in.taille), cxx2lang<int>(in.min_coord), cxx2lang<int>(in.max_coord)};
  int argc = 3;
  VALUE out = rb_funcall2(rb_path2class("Taille_terrain"), rb_intern("new"), argc, argv);
  return out;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template<>
caracs lang2cxx<caracs>(VALUE in)
{
  caracs out ;
  VALUE pa_init = rb_iv_get(in, "@pa_init");
  out.pa_init = lang2cxx<int>(pa_init);
  VALUE portee = rb_iv_get(in, "@portee");
  out.portee = lang2cxx<int>(portee);
  return out;
}
template<>
VALUE cxx2lang<caracs>(caracs in)
{
  VALUE argv[] = {cxx2lang<int>(in.pa_init), cxx2lang<int>(in.portee)};
  int argc = 2;
  VALUE out = rb_funcall2(rb_path2class("Caracs"), rb_intern("new"), argc, argv);
  return out;
}

///
// Représente une unité sur le terrain.
//
template<>
unite lang2cxx<unite>(VALUE in)
{
  unite out ;
  VALUE pos = rb_iv_get(in, "@pos");
  out.pos = lang2cxx<position>(pos);
  VALUE ennemi = rb_iv_get(in, "@ennemi");
  out.ennemi = lang2cxx<bool>(ennemi);
  VALUE type_unite_actuel = rb_iv_get(in, "@type_unite_actuel");
  out.type_unite_actuel = lang2cxx<type_unite>(type_unite_actuel);
  VALUE vrai_type_unite = rb_iv_get(in, "@vrai_type_unite");
  out.vrai_type_unite = lang2cxx<type_unite>(vrai_type_unite);
  VALUE ko = rb_iv_get(in, "@ko");
  out.ko = lang2cxx<int>(ko);
  VALUE pa = rb_iv_get(in, "@pa");
  out.pa = lang2cxx<int>(pa);
  VALUE attaques = rb_iv_get(in, "@attaques");
  out.attaques = lang2cxx<int>(attaques);
  VALUE attaques_gratuites = rb_iv_get(in, "@attaques_gratuites");
  out.attaques_gratuites = lang2cxx<int>(attaques_gratuites);
  VALUE id = rb_iv_get(in, "@id");
  out.id = lang2cxx<int>(id);
  return out;
}
template<>
VALUE cxx2lang<unite>(unite in)
{
  VALUE argv[] = {cxx2lang<position>(in.pos), cxx2lang<bool>(in.ennemi), cxx2lang<type_unite>(in.type_unite_actuel), cxx2lang<type_unite>(in.vrai_type_unite), cxx2lang<int>(in.ko), cxx2lang<int>(in.pa), cxx2lang<int>(in.attaques), cxx2lang<int>(in.attaques_gratuites), cxx2lang<int>(in.id)};
  int argc = 9;
  VALUE out = rb_funcall2(rb_path2class("Unite"), rb_intern("new"), argc, argv);
  return out;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template<>
cartes lang2cxx<cartes>(VALUE in)
{
  cartes out ;
  VALUE potion = rb_iv_get(in, "@potion");
  out.potion = lang2cxx<int>(potion);
  VALUE deguisement = rb_iv_get(in, "@deguisement");
  out.deguisement = lang2cxx<int>(deguisement);
  VALUE banzai = rb_iv_get(in, "@banzai");
  out.banzai = lang2cxx<int>(banzai);
  VALUE pacifisme = rb_iv_get(in, "@pacifisme");
  out.pacifisme = lang2cxx<int>(pacifisme);
  return out;
}
template<>
VALUE cxx2lang<cartes>(cartes in)
{
  VALUE argv[] = {cxx2lang<int>(in.potion), cxx2lang<int>(in.deguisement), cxx2lang<int>(in.banzai), cxx2lang<int>(in.pacifisme)};
  int argc = 4;
  VALUE out = rb_funcall2(rb_path2class("Cartes"), rb_intern("new"), argc, argv);
  return out;
}

VALUE rb_nombre_pc()
{
  return cxx2lang<int>(api_nombre_pc());
}

VALUE rb_nombre_unites(VALUE ennemi)
{
  return cxx2lang<int>(api_nombre_unites(lang2cxx<bool>(ennemi)));
}

VALUE rb_tour_actuel()
{
  return cxx2lang<int>(api_tour_actuel());
}

VALUE rb_pos_renfort(VALUE ennemi)
{
  return cxx2lang<position>(api_pos_renfort(lang2cxx<bool>(ennemi)));
}

VALUE rb_caracteristiques(VALUE tu)
{
  return cxx2lang<caracs>(api_caracteristiques(lang2cxx<type_unite>(tu)));
}

VALUE rb_mes_cartes()
{
  return cxx2lang<cartes>(api_mes_cartes());
}

VALUE rb_unites()
{
  return cxx2lang_array<>(api_unites());
}

VALUE rb_taille_terrain_actuelle()
{
  return cxx2lang<taille_terrain>(api_taille_terrain_actuelle());
}

VALUE rb_potion(VALUE cible)
{
  return cxx2lang<erreur>(api_potion(lang2cxx<position>(cible)));
}

VALUE rb_deguisement(VALUE cible, VALUE nouveau_type)
{
  return cxx2lang<erreur>(api_deguisement(lang2cxx<position>(cible), lang2cxx<type_unite>(nouveau_type)));
}

VALUE rb_banzai(VALUE cible)
{
  return cxx2lang<erreur>(api_banzai(lang2cxx<position>(cible)));
}

VALUE rb_pacifisme()
{
  return cxx2lang<erreur>(api_pacifisme());
}

VALUE rb_deplacer(VALUE cible, VALUE pos)
{
  return cxx2lang<erreur>(api_deplacer(lang2cxx<position>(cible), lang2cxx<position>(pos)));
}

VALUE rb_relever(VALUE cible)
{
  return cxx2lang<erreur>(api_relever(lang2cxx<position>(cible)));
}

VALUE rb_attaquer(VALUE attaquant, VALUE cible)
{
  return cxx2lang<erreur>(api_attaquer(lang2cxx<position>(attaquant), lang2cxx<position>(cible)));
}

VALUE rb_renfort(VALUE quoi)
{
  return cxx2lang<erreur>(api_renfort(lang2cxx<type_unite>(quoi)));
}

VALUE rb_annuler()
{
  return cxx2lang<bool>(api_annuler());
}

VALUE rb_afficher_erreur(VALUE v)
{
api_afficher_erreur(lang2cxx<erreur>(v));
}

VALUE rb_afficher_type_unite(VALUE v)
{
api_afficher_type_unite(lang2cxx<type_unite>(v));
}

VALUE rb_afficher_position(VALUE v)
{
api_afficher_position(lang2cxx<position>(v));
}

VALUE rb_afficher_taille_terrain(VALUE v)
{
api_afficher_taille_terrain(lang2cxx<taille_terrain>(v));
}

VALUE rb_afficher_caracs(VALUE v)
{
api_afficher_caracs(lang2cxx<caracs>(v));
}

VALUE rb_afficher_unite(VALUE v)
{
api_afficher_unite(lang2cxx<unite>(v));
}

VALUE rb_afficher_cartes(VALUE v)
{
api_afficher_cartes(lang2cxx<cartes>(v));
}


void loadCallback()
{ 
///
// Renvoie le nombre de points de commandements.
//
    rb_define_global_function("nombre_pc", (VALUE(*)(...))(rb_nombre_pc), 0);

///
// Renvoie le nombre d'unités en jeu.
//
    rb_define_global_function("nombre_unites", (VALUE(*)(...))(rb_nombre_unites), 1);

///
// Renvoie le numéro du tour actuel.
//
    rb_define_global_function("tour_actuel", (VALUE(*)(...))(rb_tour_actuel), 0);

///
// Renvoie la position du spawn (ennemi ou non).
//
    rb_define_global_function("pos_renfort", (VALUE(*)(...))(rb_pos_renfort), 1);

///
// Renvoie les caractéristiques d'un type d'unité.
//
    rb_define_global_function("caracteristiques", (VALUE(*)(...))(rb_caracteristiques), 1);

///
// Retourne une structure "cartes" contenant les informations sur les cartes que vous avez en main.
//
    rb_define_global_function("mes_cartes", (VALUE(*)(...))(rb_mes_cartes), 0);

///
// Retourne la liste des unités actuellement en jeu.
//
    rb_define_global_function("unites", (VALUE(*)(...))(rb_unites), 0);

///
// Retourne la taille actuelle du terrain et les coordonnées min/max dans une structure "taille_terrain".
//
    rb_define_global_function("taille_terrain_actuelle", (VALUE(*)(...))(rb_taille_terrain_actuelle), 0);

///
// Utilise une carte « Potion magique » que vous avez dans votre main.
//
    rb_define_global_function("potion", (VALUE(*)(...))(rb_potion), 1);

///
// Utilise une carte « Déguisement » que vous avez dans votre main.
//
    rb_define_global_function("deguisement", (VALUE(*)(...))(rb_deguisement), 2);

///
// Utilise une carte « Banzaï » que vous avez dans votre main.
//
    rb_define_global_function("banzai", (VALUE(*)(...))(rb_banzai), 1);

///
// Utilise une carte « Pacifisme » que vous avez dans votre main.
//
    rb_define_global_function("pacifisme", (VALUE(*)(...))(rb_pacifisme), 0);

///
// Déplace une unité vers une position à portée.
//
    rb_define_global_function("deplacer", (VALUE(*)(...))(rb_deplacer), 2);

///
// Relève une unité n'ayant plus de marqueurs de KO.
//
    rb_define_global_function("relever", (VALUE(*)(...))(rb_relever), 1);

///
// Attaque une autre unité.
//
    rb_define_global_function("attaquer", (VALUE(*)(...))(rb_attaquer), 2);

///
// Fait apparaitre une unité sur la case de spawn.
//
    rb_define_global_function("renfort", (VALUE(*)(...))(rb_renfort), 1);

///
// Annule l'effet de la dernière action et remet le jeu dans l'état précédent. Renvoie false s'il n'y a rien à annuler, true sinon.
//
    rb_define_global_function("annuler", (VALUE(*)(...))(rb_annuler), 0);

///
// Affiche le contenu d'une valeur de type erreur
//
    rb_define_global_function("afficher_erreur", (VALUE(*)(...))(rb_afficher_erreur), 1);

///
// Affiche le contenu d'une valeur de type type_unite
//
    rb_define_global_function("afficher_type_unite", (VALUE(*)(...))(rb_afficher_type_unite), 1);

///
// Affiche le contenu d'une valeur de type position
//
    rb_define_global_function("afficher_position", (VALUE(*)(...))(rb_afficher_position), 1);

///
// Affiche le contenu d'une valeur de type taille_terrain
//
    rb_define_global_function("afficher_taille_terrain", (VALUE(*)(...))(rb_afficher_taille_terrain), 1);

///
// Affiche le contenu d'une valeur de type caracs
//
    rb_define_global_function("afficher_caracs", (VALUE(*)(...))(rb_afficher_caracs), 1);

///
// Affiche le contenu d'une valeur de type unite
//
    rb_define_global_function("afficher_unite", (VALUE(*)(...))(rb_afficher_unite), 1);

///
// Affiche le contenu d'une valeur de type cartes
//
    rb_define_global_function("afficher_cartes", (VALUE(*)(...))(rb_afficher_cartes), 1);


}

void init(){
  static bool initialized = false;
  if (!initialized){
    initialized = true;
    std::cout << "init..." << std::endl;
    char* file = "prologin.rb";
    int status;
    ruby_init();
    ruby_init_loadpath ();
    loadCallback();
    rb_load_protect(rb_str_new2(file), 0, &status);
    std::cout << "status = " << status << std::endl;
  }
}

extern "C" {
///
// Fonction appellée au début de la partie.
//
void init_game()
{

  init();
  int status;
  printf("calling ruby function: init_game\n");
  rb_eval_string("init_game()" );
  if (status)
    fprintf(stderr, "error while calling ruby function: init_game (%d)\n", status);
}

///
// Fonction appellée pour la phase de retrait de KO.
//
position retirer_ko()
{

  init();
  int status;
  printf("calling ruby function: retirer_ko\n");
  rb_eval_string("retirer_ko()" );
  if (status)
    fprintf(stderr, "error while calling ruby function: retirer_ko (%d)\n", status);
}

///
// Fonction appellée pour la phase de jeu.
//
void jouer()
{

  init();
  int status;
  printf("calling ruby function: jouer\n");
  rb_eval_string("jouer()" );
  if (status)
    fprintf(stderr, "error while calling ruby function: jouer (%d)\n", status);
}

///
// Fonction appellée à la fin de la partie.
//
void end_game()
{

  init();
  int status;
  printf("calling ruby function: end_game\n");
  rb_eval_string("end_game()" );
  if (status)
    fprintf(stderr, "error while calling ruby function: end_game (%d)\n", status);
}

}
