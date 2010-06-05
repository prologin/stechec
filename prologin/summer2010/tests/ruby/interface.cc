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
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template<>
erreur lang2cxx<erreur>(VALUE in)
{
if (TYPE(in) != T_STRING) TYPEERR("erreur", in);
  char* v = RSTRING_PTR(in);
  if (strcmp(v, "ok") == 0)
      return OK;
  if (strcmp(v, "position_invalide") == 0)
      return POSITION_INVALIDE;
  if (strcmp(v, "plus_de_pa") == 0)
      return PLUS_DE_PA;
  if (strcmp(v, "pas_a_porte") == 0)
      return PAS_A_PORTE;
  if (strcmp(v, "unite_ko") == 0)
      return UNITE_KO;
  if (strcmp(v, "pas_a_toi") == 0)
      return PAS_A_TOI;
  if (strcmp(v, "utilisation_impossible") == 0)
      return UTILISATION_IMPOSSIBLE;
  if (strcmp(v, "plus_d_argent") == 0)
      return PLUS_D_ARGENT;
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
    case POSITION_INVALIDE:
      return rb_str_new("position_invalide", 17);
    case PLUS_DE_PA:
      return rb_str_new("plus_de_pa", 10);
    case PAS_A_PORTE:
      return rb_str_new("pas_a_porte", 11);
    case UNITE_KO:
      return rb_str_new("unite_ko", 8);
    case PAS_A_TOI:
      return rb_str_new("pas_a_toi", 9);
    case UTILISATION_IMPOSSIBLE:
      return rb_str_new("utilisation_impossible", 22);
    case PLUS_D_ARGENT:
      return rb_str_new("plus_d_argent", 13);
  }
  abort();
}

///
// Le type d'un objet
//
template<>
type_objet lang2cxx<type_objet>(VALUE in)
{
if (TYPE(in) != T_STRING) TYPEERR("type_objet", in);
  char* v = RSTRING_PTR(in);
  if (strcmp(v, "filet") == 0)
      return FILET;
  if (strcmp(v, "marteau") == 0)
      return MARTEAU;
  if (strcmp(v, "rien") == 0)
      return RIEN;
  abort();
  TYPEERR("type_objet", in);
}
template<>
VALUE cxx2lang<type_objet>(type_objet in)
{
  switch (in)
  {
    case FILET:
      return rb_str_new("filet", 5);
    case MARTEAU:
      return rb_str_new("marteau", 7);
    case RIEN:
      return rb_str_new("rien", 4);
  }
  abort();
}

///
// Représente une position sur le terrain du jeu.
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
// les caracteristiques d'un objet
//
template<>
caracteristiques_objet lang2cxx<caracteristiques_objet>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("caracteristiques_objet", in);
  caracteristiques_objet out ;
  VALUE cout = rb_iv_get(in, "@cout");
  out.cout = lang2cxx<int>(cout);
  VALUE porte = rb_iv_get(in, "@porte");
  out.porte = lang2cxx<int>(porte);
  return out;
}
template<>
VALUE cxx2lang<caracteristiques_objet>(caracteristiques_objet in)
{
  VALUE argv[] = {cxx2lang<int>(in.cout), cxx2lang<int>(in.porte)};
  int argc = 2;
  VALUE out = rb_funcall2(rb_path2class("Caracteristiques_objet"), rb_intern("new"), argc, argv);
  return out;
}

///
// Représente une unité sur le terrain.
//
template<>
unite lang2cxx<unite>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("unite", in);
  unite out ;
  VALUE pos_unite = rb_iv_get(in, "@pos_unite");
  out.pos_unite = lang2cxx<position>(pos_unite);
  VALUE team = rb_iv_get(in, "@team");
  out.team = lang2cxx<int>(team);
  VALUE ko = rb_iv_get(in, "@ko");
  out.ko = lang2cxx<int>(ko);
  VALUE pa = rb_iv_get(in, "@pa");
  out.pa = lang2cxx<int>(pa);
  VALUE objet = rb_iv_get(in, "@objet");
  out.objet = lang2cxx<type_objet>(objet);
  VALUE id = rb_iv_get(in, "@id");
  out.id = lang2cxx<int>(id);
  return out;
}
template<>
VALUE cxx2lang<unite>(unite in)
{
  VALUE argv[] = {cxx2lang<position>(in.pos_unite), cxx2lang<int>(in.team), cxx2lang<int>(in.ko), cxx2lang<int>(in.pa), cxx2lang<type_objet>(in.objet), cxx2lang<int>(in.id)};
  int argc = 6;
  VALUE out = rb_funcall2(rb_path2class("Unite"), rb_intern("new"), argc, argv);
  return out;
}

///
// Représente le type d'une piece.
//
template<>
piece lang2cxx<piece>(VALUE in)
{
  if (TYPE(in) != T_OBJECT) TYPEERR("piece", in);
  piece out ;
  VALUE valeur = rb_iv_get(in, "@valeur");
  out.valeur = lang2cxx<int>(valeur);
  VALUE pos_piece = rb_iv_get(in, "@pos_piece");
  out.pos_piece = lang2cxx<position>(pos_piece);
  VALUE tour_apparition = rb_iv_get(in, "@tour_apparition");
  out.tour_apparition = lang2cxx<int>(tour_apparition);
  return out;
}
template<>
VALUE cxx2lang<piece>(piece in)
{
  VALUE argv[] = {cxx2lang<int>(in.valeur), cxx2lang<position>(in.pos_piece), cxx2lang<int>(in.tour_apparition)};
  int argc = 3;
  VALUE out = rb_funcall2(rb_path2class("Piece"), rb_intern("new"), argc, argv);
  return out;
}

static VALUE rb_mon_equipe(VALUE self)
{
  return cxx2lang<int>(api_mon_equipe());
}

static VALUE rb_score(VALUE self, VALUE team)
{
  return cxx2lang<int>(api_score(lang2cxx<int>( team ) ));
}

static VALUE rb_nombre_equipes(VALUE self)
{
  return cxx2lang<int>(api_nombre_equipes());
}

static VALUE rb_tour_actuel(VALUE self)
{
  return cxx2lang<int>(api_tour_actuel());
}

static VALUE rb_pieces_en_jeu(VALUE self)
{
  return cxx2lang_array<>(api_pieces_en_jeu());
}

static VALUE rb_pieces_a_vennir(VALUE self)
{
  return cxx2lang_array<>(api_pieces_a_vennir());
}

static VALUE rb_unites(VALUE self)
{
  return cxx2lang_array<>(api_unites());
}

static VALUE rb_proprietes_objet(VALUE self, VALUE to)
{
  return cxx2lang<caracteristiques_objet>(api_proprietes_objet(lang2cxx<type_objet>( to ) ));
}

static VALUE rb_deplacer(VALUE self, VALUE cible, VALUE pos)
{
  return cxx2lang<erreur>(api_deplacer(lang2cxx<position>( cible ) , lang2cxx<position>( pos ) ));
}

static VALUE rb_acheter_objet(VALUE self, VALUE cible, VALUE objet)
{
  return cxx2lang<erreur>(api_acheter_objet(lang2cxx<position>( cible ) , lang2cxx<type_objet>( objet ) ));
}

static VALUE rb_utiliser(VALUE self, VALUE attaquant, VALUE cible)
{
  return cxx2lang<erreur>(api_utiliser(lang2cxx<position>( attaquant ) , lang2cxx<position>( cible ) ));
}

static VALUE rb_achever_la_partie(VALUE self)
{
  return cxx2lang<erreur>(api_achever_la_partie());
}

static VALUE rb_afficher_erreur(VALUE self, VALUE v)
{
api_afficher_erreur(lang2cxx<erreur>( v ) );
  return Qnil;
}

static VALUE rb_afficher_type_objet(VALUE self, VALUE v)
{
api_afficher_type_objet(lang2cxx<type_objet>( v ) );
  return Qnil;
}

static VALUE rb_afficher_position(VALUE self, VALUE v)
{
api_afficher_position(lang2cxx<position>( v ) );
  return Qnil;
}

static VALUE rb_afficher_caracteristiques_objet(VALUE self, VALUE v)
{
api_afficher_caracteristiques_objet(lang2cxx<caracteristiques_objet>( v ) );
  return Qnil;
}

static VALUE rb_afficher_unite(VALUE self, VALUE v)
{
api_afficher_unite(lang2cxx<unite>( v ) );
  return Qnil;
}

static VALUE rb_afficher_piece(VALUE self, VALUE v)
{
api_afficher_piece(lang2cxx<piece>( v ) );
  return Qnil;
}


void loadCallback()
{ 
///
// Renvoie le numero de votre equipe.
//
    rb_define_global_function("mon_equipe", (VALUE(*)(ANYARGS))(rb_mon_equipe), 0);

///
// Renvoie le score d'une equipe.
//
    rb_define_global_function("score", (VALUE(*)(ANYARGS))(rb_score), 1);

///
// Renvoie le nombre d'equipes sur la map
//
    rb_define_global_function("nombre_equipes", (VALUE(*)(ANYARGS))(rb_nombre_equipes), 0);

///
// Renvoie le numéro du tour actuel.
//
    rb_define_global_function("tour_actuel", (VALUE(*)(ANYARGS))(rb_tour_actuel), 0);

///
// Renvoie les pieces qui sont sur la map
//
    rb_define_global_function("pieces_en_jeu", (VALUE(*)(ANYARGS))(rb_pieces_en_jeu), 0);

///
// Renvoie le 10 prochaines pieces a vennir
//
    rb_define_global_function("pieces_a_vennir", (VALUE(*)(ANYARGS))(rb_pieces_a_vennir), 0);

///
// Retourne la liste des unités actuellement en jeu.
//
    rb_define_global_function("unites", (VALUE(*)(ANYARGS))(rb_unites), 0);

///
// Retourne les caracteristiques de l'objet.
//
    rb_define_global_function("proprietes_objet", (VALUE(*)(ANYARGS))(rb_proprietes_objet), 1);

///
// Déplace une unité vers une position à portée.
//
    rb_define_global_function("deplacer", (VALUE(*)(ANYARGS))(rb_deplacer), 2);

///
// Achete un objet
//
    rb_define_global_function("acheter_objet", (VALUE(*)(ANYARGS))(rb_acheter_objet), 2);

///
// utilise un objet
//
    rb_define_global_function("utiliser", (VALUE(*)(ANYARGS))(rb_utiliser), 2);

///
// coupe la partie, cette action coute 100 d'or et met fin a la partie.
//
    rb_define_global_function("achever_la_partie", (VALUE(*)(ANYARGS))(rb_achever_la_partie), 0);

///
// Affiche le contenu d'une valeur de type erreur
//
    rb_define_global_function("afficher_erreur", (VALUE(*)(ANYARGS))(rb_afficher_erreur), 1);

///
// Affiche le contenu d'une valeur de type type_objet
//
    rb_define_global_function("afficher_type_objet", (VALUE(*)(ANYARGS))(rb_afficher_type_objet), 1);

///
// Affiche le contenu d'une valeur de type position
//
    rb_define_global_function("afficher_position", (VALUE(*)(ANYARGS))(rb_afficher_position), 1);

///
// Affiche le contenu d'une valeur de type caracteristiques_objet
//
    rb_define_global_function("afficher_caracteristiques_objet", (VALUE(*)(ANYARGS))(rb_afficher_caracteristiques_objet), 1);

///
// Affiche le contenu d'une valeur de type unite
//
    rb_define_global_function("afficher_unite", (VALUE(*)(ANYARGS))(rb_afficher_unite), 1);

///
// Affiche le contenu d'une valeur de type piece
//
    rb_define_global_function("afficher_piece", (VALUE(*)(ANYARGS))(rb_afficher_piece), 1);


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
// Fonction appellée au début de la partie.
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
// Fonction appellée pour la phase de jeu.
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
// Fonction appellée à la fin de la partie.
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

}
