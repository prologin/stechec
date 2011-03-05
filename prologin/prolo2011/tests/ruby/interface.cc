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

void TYPEERR(char *t, VALUE in) {
  char err_str[256];
  sprintf(err_str, "erreur de types : %s expected, got : 0x%02x.\n", t, TYPE(in));
  rb_fatal( err_str);
}

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
  if (TYPE(in) != T_FIXNUM) TYPEERR("integer", in);
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
  if (TYPE(*l) != T_ARRAY) TYPEERR("array", l);

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
// Énumération représentant une erreur renvoyée par une des fonctions d'action
//
template<>
erreur lang2cxx<erreur>(VALUE in)
{
if (TYPE(in) != T_STRING) TYPEERR("erreur", in);
  char* v = RSTRING_PTR(in);
  if (strcmp(v, "ok") == 0)
      return OK;
  if (strcmp(v, "id_invalide") == 0)
      return ID_INVALIDE;
  if (strcmp(v, "position_invalide") == 0)
      return POSITION_INVALIDE;
  if (strcmp(v, "plus_de_pa") == 0)
      return PLUS_DE_PA;
  if (strcmp(v, "bonus_invalide") == 0)
      return BONUS_INVALIDE;
  if (strcmp(v, "pas_a_toi") == 0)
      return PAS_A_TOI;
  abort();
  TYPEERR("erreur", in);
}
template<>
VALUE cxx2lang<erreur>(erreur in)
{
  switch (in)
  {
    case OK:
      return rb_str_new("ok", 2);
    case ID_INVALIDE:
      return rb_str_new("id_invalide", 11);
    case POSITION_INVALIDE:
      return rb_str_new("position_invalide", 17);
    case PLUS_DE_PA:
      return rb_str_new("plus_de_pa", 10);
    case BONUS_INVALIDE:
      return rb_str_new("bonus_invalide", 14);
    case PAS_A_TOI:
      return rb_str_new("pas_a_toi", 9);
  }
  abort();
}

///
// Énumération représentant les différents types de case
//
template<>
type_case lang2cxx<type_case>(VALUE in)
{
if (TYPE(in) != T_STRING) TYPEERR("type_case", in);
  char* v = RSTRING_PTR(in);
  if (strcmp(v, "vide") == 0)
      return VIDE;
  if (strcmp(v, "obstacle") == 0)
      return OBSTACLE;
  if (strcmp(v, "bonus") == 0)
      return BONUS;
  if (strcmp(v, "point_croisement") == 0)
      return POINT_CROISEMENT;
  abort();
  TYPEERR("type_case", in);
}
template<>
VALUE cxx2lang<type_case>(type_case in)
{
  switch (in)
  {
    case VIDE:
      return rb_str_new("vide", 4);
    case OBSTACLE:
      return rb_str_new("obstacle", 8);
    case BONUS:
      return rb_str_new("bonus", 5);
    case POINT_CROISEMENT:
      return rb_str_new("point_croisement", 16);
  }
  abort();
}

///
// Énumération représentant les différents types de bonii
//
template<>
bonus lang2cxx<bonus>(VALUE in)
{
if (TYPE(in) != T_STRING) TYPEERR("bonus", in);
  char* v = RSTRING_PTR(in);
  if (strcmp(v, "vide") == 0)
      return VIDE;
  if (strcmp(v, "bonus_croisement") == 0)
      return BONUS_CROISEMENT;
  if (strcmp(v, "plus_long") == 0)
      return PLUS_LONG;
  if (strcmp(v, "plus_pa") == 0)
      return PLUS_PA;
  if (strcmp(v, "bonus_regeneration") == 0)
      return BONUS_REGENERATION;
  abort();
  TYPEERR("bonus", in);
}
template<>
VALUE cxx2lang<bonus>(bonus in)
{
  switch (in)
  {
    case VIDE:
      return rb_str_new("vide", 4);
    case BONUS_CROISEMENT:
      return rb_str_new("bonus_croisement", 16);
    case PLUS_LONG:
      return rb_str_new("plus_long", 9);
    case PLUS_PA:
      return rb_str_new("plus_pa", 7);
    case BONUS_REGENERATION:
      return rb_str_new("bonus_regeneration", 18);
  }
  abort();
}

///
// Représente une position sur le terrain du jeu
//
template<>
position lang2cxx<position>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("position", in);
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
// Caracteristiques d'une source d'énergie
//
template<>
source_energie lang2cxx<source_energie>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("source_energie", in);
  source_energie out ;
  VALUE id = rb_iv_get(in, "@id");
  out.id = lang2cxx<int>(id);
  VALUE pos = rb_iv_get(in, "@pos");
  out.pos = lang2cxx<position>(pos);
  VALUE coef = rb_iv_get(in, "@coef");
  out.coef = lang2cxx<int>(coef);
  return out;
}
template<>
VALUE cxx2lang<source_energie>(source_energie in)
{
  VALUE argv[] = {cxx2lang<int>(in.id), cxx2lang<position>(in.pos), cxx2lang<int>(in.coef)};
  int argc = 3;
  VALUE out = rb_funcall2(rb_path2class("Source_energie"), rb_intern("new"), argc, argv);
  return out;
}

///
// Représente une traînée de moto sur le terrain
//
template<>
trainee_moto lang2cxx<trainee_moto>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("trainee_moto", in);
  trainee_moto out ;
  VALUE id = rb_iv_get(in, "@id");
  out.id = lang2cxx<int>(id);
  VALUE emplacement = rb_iv_get(in, "@emplacement");
  out.emplacement = lang2cxx_array<position>(emplacement);
  VALUE team = rb_iv_get(in, "@team");
  out.team = lang2cxx<int>(team);
  return out;
}
template<>
VALUE cxx2lang<trainee_moto>(trainee_moto in)
{
  VALUE argv[] = {cxx2lang<int>(in.id), cxx2lang_array<position>(in.emplacement), cxx2lang<int>(in.team)};
  int argc = 3;
  VALUE out = rb_funcall2(rb_path2class("Trainee_moto"), rb_intern("new"), argc, argv);
  return out;
}

static VALUE rb_mon_equipe(VALUE self)
{
  return cxx2lang<int>(api_mon_equipe());
}
static VALUE rb_scores(VALUE self)
{
  return cxx2lang_array<>(api_scores());
}
static VALUE rb_nombre_equipes(VALUE self)
{
  return cxx2lang<int>(api_nombre_equipes());
}
static VALUE rb_tour_actuel(VALUE self)
{
  return cxx2lang<int>(api_tour_actuel());
}
static VALUE rb_sources_energie(VALUE self)
{
  return cxx2lang_array<>(api_sources_energie());
}
static VALUE rb_trainees_moto(VALUE self)
{
  return cxx2lang_array<>(api_trainees_moto());
}
static VALUE rb_regarder_type_case(VALUE self, VALUE pos)
{
  return cxx2lang<type_case>(api_regarder_type_case(lang2cxx<position>( pos ) ));
}
static VALUE rb_regarder_type_bonus(VALUE self, VALUE pos)
{
  return cxx2lang<bonus>(api_regarder_type_bonus(lang2cxx<position>( pos ) ));
}
static VALUE rb_regarder_bonus(VALUE self, VALUE equipe)
{
  return cxx2lang_array<>(api_regarder_bonus(lang2cxx<int>( equipe ) ));
}
static VALUE rb_deplacer(VALUE self, VALUE id, VALUE de, VALUE vers)
{
  return cxx2lang<erreur>(api_deplacer(lang2cxx<int>( id ) , lang2cxx<position>( de ) , lang2cxx<position>( vers ) ));
}
static VALUE rb_couper_trainee_moto(VALUE self, VALUE id, VALUE entre, VALUE et)
{
  return cxx2lang<erreur>(api_couper_trainee_moto(lang2cxx<int>( id ) , lang2cxx<position>( entre ) , lang2cxx<position>( et ) ));
}
static VALUE rb_cancel(VALUE self)
{
  return cxx2lang<erreur>(api_cancel());
}
static VALUE rb_enrouler(VALUE self, VALUE id, VALUE point)
{
  return cxx2lang<erreur>(api_enrouler(lang2cxx<int>( id ) , lang2cxx<position>( point ) ));
}
static VALUE rb_regenerer_source_energie(VALUE self, VALUE id)
{
  return cxx2lang<erreur>(api_regenerer_source_energie(lang2cxx<int>( id ) ));
}
static VALUE rb_allonger_pa(VALUE self)
{
  return cxx2lang<erreur>(api_allonger_pa());
}
static VALUE rb_agrandir_trainee_moto(VALUE self, VALUE id, VALUE longueur)
{
  return cxx2lang<erreur>(api_agrandir_trainee_moto(lang2cxx<int>( id ) , lang2cxx<int>( longueur ) ));
}
static VALUE rb_poser_point_croisement(VALUE self, VALUE point)
{
  return cxx2lang<erreur>(api_poser_point_croisement(lang2cxx<position>( point ) ));
}
static VALUE rb_fusionner(VALUE self, VALUE id1, VALUE pos1, VALUE id2, VALUE pos2)
{
  return cxx2lang<erreur>(api_fusionner(lang2cxx<int>( id1 ) , lang2cxx<position>( pos1 ) , lang2cxx<int>( id2 ) , lang2cxx<position>( pos2 ) ));
}
static VALUE rb_afficher_erreur(VALUE self, VALUE v)
{
api_afficher_erreur(lang2cxx<erreur>( v ) );
  return Qnil;
}
static VALUE rb_afficher_type_case(VALUE self, VALUE v)
{
api_afficher_type_case(lang2cxx<type_case>( v ) );
  return Qnil;
}
static VALUE rb_afficher_bonus(VALUE self, VALUE v)
{
api_afficher_bonus(lang2cxx<bonus>( v ) );
  return Qnil;
}
static VALUE rb_afficher_position(VALUE self, VALUE v)
{
api_afficher_position(lang2cxx<position>( v ) );
  return Qnil;
}
static VALUE rb_afficher_source_energie(VALUE self, VALUE v)
{
api_afficher_source_energie(lang2cxx<source_energie>( v ) );
  return Qnil;
}
static VALUE rb_afficher_trainee_moto(VALUE self, VALUE v)
{
api_afficher_trainee_moto(lang2cxx<trainee_moto>( v ) );
  return Qnil;
}

void loadCallback()
{ 
///
// Retourne le numéro de votre équipe
//
    rb_define_global_function("mon_equipe", (VALUE(*)(ANYARGS))(rb_mon_equipe), 0);

///
// Retourne les scores de chaque équipe
//
    rb_define_global_function("scores", (VALUE(*)(ANYARGS))(rb_scores), 0);

///
// Retourne le nombre d'équipes sur le terrain
//
    rb_define_global_function("nombre_equipes", (VALUE(*)(ANYARGS))(rb_nombre_equipes), 0);

///
// Retourne le numéro du tour actuel
//
    rb_define_global_function("tour_actuel", (VALUE(*)(ANYARGS))(rb_tour_actuel), 0);

///
// Retourne la liste des sources d'énergie
//
    rb_define_global_function("sources_energie", (VALUE(*)(ANYARGS))(rb_sources_energie), 0);

///
// Retourne la liste des traînées de moto
//
    rb_define_global_function("trainees_moto", (VALUE(*)(ANYARGS))(rb_trainees_moto), 0);

///
// Retourne le type d'une case
//
    rb_define_global_function("regarder_type_case", (VALUE(*)(ANYARGS))(rb_regarder_type_case), 1);

///
// Retourne le type de bonus d'une case
//
    rb_define_global_function("regarder_type_bonus", (VALUE(*)(ANYARGS))(rb_regarder_type_bonus), 1);

///
// Retourne la liste des bonus d'une équipe
//
    rb_define_global_function("regarder_bonus", (VALUE(*)(ANYARGS))(rb_regarder_bonus), 1);

///
// Déplace une moto
//
    rb_define_global_function("deplacer", (VALUE(*)(ANYARGS))(rb_deplacer), 3);

///
// Coupe une traînée de moto en deux nouvelles traînées. « entre » et « et » doivent être deux positions adjacentes occupées par une même traînée de moto.
//
    rb_define_global_function("couper_trainee_moto", (VALUE(*)(ANYARGS))(rb_couper_trainee_moto), 3);

///
// Annuler l'action précédente
//
    rb_define_global_function("cancel", (VALUE(*)(ANYARGS))(rb_cancel), 0);

///
// Enrouler la traînée de moto en un point
//
    rb_define_global_function("enrouler", (VALUE(*)(ANYARGS))(rb_enrouler), 2);

///
// Régénère une source d'énergie à son maximal
//
    rb_define_global_function("regenerer_source_energie", (VALUE(*)(ANYARGS))(rb_regenerer_source_energie), 1);

///
// Allonge le tour en rajoutant des points d'action
//
    rb_define_global_function("allonger_pa", (VALUE(*)(ANYARGS))(rb_allonger_pa), 0);

///
// Allonge une traînée de moto. L'allongement se fera aux prochains déplacements. La longueur du prolongement doit être comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
    rb_define_global_function("agrandir_trainee_moto", (VALUE(*)(ANYARGS))(rb_agrandir_trainee_moto), 2);

///
// Pose un point de croisement sur une case du terrain. La case doit ne pas déjà être un point de croisement.
//
    rb_define_global_function("poser_point_croisement", (VALUE(*)(ANYARGS))(rb_poser_point_croisement), 1);

///
// Fusionner deux traînées de moto. Les deux doivent appartenir à la même équipe, mais doivent être deux traînées distinctes. « pos1 » et « pos2 » doivent être adjacentes et occupées respectivement par « id1 » et « id2 ».
//
    rb_define_global_function("fusionner", (VALUE(*)(ANYARGS))(rb_fusionner), 4);

///
// Affiche le contenu d'une valeur de type erreur
//
    rb_define_global_function("afficher_erreur", (VALUE(*)(ANYARGS))(rb_afficher_erreur), 1);

///
// Affiche le contenu d'une valeur de type type_case
//
    rb_define_global_function("afficher_type_case", (VALUE(*)(ANYARGS))(rb_afficher_type_case), 1);

///
// Affiche le contenu d'une valeur de type bonus
//
    rb_define_global_function("afficher_bonus", (VALUE(*)(ANYARGS))(rb_afficher_bonus), 1);

///
// Affiche le contenu d'une valeur de type position
//
    rb_define_global_function("afficher_position", (VALUE(*)(ANYARGS))(rb_afficher_position), 1);

///
// Affiche le contenu d'une valeur de type source_energie
//
    rb_define_global_function("afficher_source_energie", (VALUE(*)(ANYARGS))(rb_afficher_source_energie), 1);

///
// Affiche le contenu d'une valeur de type trainee_moto
//
    rb_define_global_function("afficher_trainee_moto", (VALUE(*)(ANYARGS))(rb_afficher_trainee_moto), 1);


}

void init(){
  static bool initialized = false;
  if (!initialized){
    initialized = true;
    std::cout << "init..." << std::endl;
    std::string file;
    char * path = getenv("CHAMPION_PATH");
    file = (path == NULL) ? ".":path;
    std::cout << "directory is " << file << std::endl;
    setenv("RUBYLIB", file.c_str(), 1);
    file += "prologin.rb";
    int status;
    ruby_init();
    ruby_script("");
    std::cout << "load path..." << std::endl;
    ruby_init_loadpath ();
    std::cout << "load callback..." << std::endl;
    loadCallback();
    std::cout << "load file..." << std::endl;
    rb_load_protect(rb_str_new2(file.c_str()), 0, &status);
    std::cout << "status = " << status << std::endl;
    if (status){
       rb_p (rb_errinfo()); 
       abort();
    }
  }
}

void my_ruby_end(){
  ruby_cleanup(0);
}

extern "C" {
///
// Fonction appellée au début de la partie
//
VALUE init_game_unwrap(VALUE args)
{

  printf("calling ruby function: init_game\n");
  VALUE v = rb_eval_string("init_game()"); 
  return v;
}
void init_game()
{

  int status;
  init();
  rb_protect(&init_game_unwrap, Qnil, &status);
  if (status){
    fprintf(stderr, "error while calling ruby function: init_game (%d)\n", status);
    rb_p (rb_errinfo());
    abort();
  }else {
    
  }
}

///
// Fonction appellée pour la phase de jeu
//
VALUE jouer_unwrap(VALUE args)
{

  printf("calling ruby function: jouer\n");
  VALUE v = rb_eval_string("jouer()"); 
  return v;
}
void jouer()
{

  int status;
  init();
  rb_protect(&jouer_unwrap, Qnil, &status);
  if (status){
    fprintf(stderr, "error while calling ruby function: jouer (%d)\n", status);
    rb_p (rb_errinfo());
    abort();
  }else {
    
  }
}

///
// Fonction appellée à la fin de la partie
//
VALUE end_game_unwrap(VALUE args)
{

  printf("calling ruby function: end_game\n");
  VALUE v = rb_eval_string("end_game()"); 
  return v;
}
void end_game()
{

  int status;
  init();
  rb_protect(&end_game_unwrap, Qnil, &status);
  if (status){
    fprintf(stderr, "error while calling ruby function: end_game (%d)\n", status);
    rb_p (rb_errinfo());
      my_ruby_end();
abort();
  }else {
      my_ruby_end();

  }
}

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type type_case
//

///
// Affiche le contenu d'une valeur de type bonus
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

}
