///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_java.rb
//


// we want to use the CNI
#include <gcj/cni.h>
#include <gcj/array.h>
#include <java/lang/Throwable.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <stdio.h>

#include "interface.hh"


template<typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.error;
}

template<>
int lang2cxx<jint, int>(jint in)
{
  return in;
}

template<>
std::string lang2cxx<java::lang::String*, std::string>(java::lang::String *in)
{
  size_t len = in->length();
  jchar *c = _Jv_GetStringChars(in);
  std::string s((char *)c, len);
  for (int i = 0; i < len; i++){
    s[i] = c[i];
  }
  return s;
}
template<>
bool lang2cxx<jboolean, bool>(jboolean in)
{
  return in;
}

template<typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.err;
}

template<>
jint cxx2lang<jint, int>(int in)
{
  return in;
}
template<>
jboolean cxx2lang<jboolean, bool>(bool in)
{
  return in;
}


template<>
java::lang::String* cxx2lang<java::lang::String*, std::string>(std::string in)
{
  jstring s = _Jv_NewStringLatin1(in.c_str(), in.length());
  return s;
}
///
// Énumération représentant une erreur renvoyée par une des fonctions d'action.
//
template<>
erreur lang2cxx<Erreur*, erreur >(Erreur* in)
{
  return erreur(lang2cxx<jint, int>( in->ordinal() ));
}
template<>
Erreur* cxx2lang<Erreur*, erreur >(erreur in)
{
  return *(&Erreur::Ok + (int)in);
}

///
// Le type d'un objet
//
template<>
type_objet lang2cxx<Type_objet*, type_objet >(Type_objet* in)
{
  return type_objet(lang2cxx<jint, int>( in->ordinal() ));
}
template<>
Type_objet* cxx2lang<Type_objet*, type_objet >(type_objet in)
{
  return *(&Type_objet::Filet + (int)in);
}

///
// Représente une position sur le terrain du jeu.
//
template <>
position lang2cxx<Position* , position >(Position* in){
  position out;
  out.x = lang2cxx<jint, int >(in->x);
  out.y = lang2cxx<jint, int >(in->y);
  return out;
}
template <>
Position* cxx2lang<Position*, position>(position in){
  Position* klass = new Position();
  klass->x = cxx2lang<jint, int >(in.x);
  klass->y = cxx2lang<jint, int >(in.y);
  return klass;
}

///
// les caracteristiques d'un objet
//
template <>
caracteristiques_objet lang2cxx<Caracteristiques_objet* , caracteristiques_objet >(Caracteristiques_objet* in){
  caracteristiques_objet out;
  out.cout = lang2cxx<jint, int >(in->cout);
  out.porte = lang2cxx<jint, int >(in->porte);
  return out;
}
template <>
Caracteristiques_objet* cxx2lang<Caracteristiques_objet*, caracteristiques_objet>(caracteristiques_objet in){
  Caracteristiques_objet* klass = new Caracteristiques_objet();
  klass->cout = cxx2lang<jint, int >(in.cout);
  klass->porte = cxx2lang<jint, int >(in.porte);
  return klass;
}

///
// Représente une unité sur le terrain.
//
template <>
unite lang2cxx<Unite* , unite >(Unite* in){
  unite out;
  out.pos_unite = lang2cxx<Position*, position >(in->pos_unite);
  out.team = lang2cxx<jint, int >(in->team);
  out.ko = lang2cxx<jint, int >(in->ko);
  out.pa = lang2cxx<jint, int >(in->pa);
  out.objet = lang2cxx<Type_objet*, type_objet >(in->objet);
  out.id = lang2cxx<jint, int >(in->id);
  return out;
}
template <>
Unite* cxx2lang<Unite*, unite>(unite in){
  Unite* klass = new Unite();
  klass->pos_unite = cxx2lang<Position*, position >(in.pos_unite);
  klass->team = cxx2lang<jint, int >(in.team);
  klass->ko = cxx2lang<jint, int >(in.ko);
  klass->pa = cxx2lang<jint, int >(in.pa);
  klass->objet = cxx2lang<Type_objet*, type_objet >(in.objet);
  klass->id = cxx2lang<jint, int >(in.id);
  return klass;
}

///
// Représente le type d'une piece.
//
template <>
piece lang2cxx<Piece* , piece >(Piece* in){
  piece out;
  out.valeur = lang2cxx<jint, int >(in->valeur);
  out.pos_piece = lang2cxx<Position*, position >(in->pos_piece);
  out.tour_apparition = lang2cxx<jint, int >(in->tour_apparition);
  return out;
}
template <>
Piece* cxx2lang<Piece*, piece>(piece in){
  Piece* klass = new Piece();
  klass->valeur = cxx2lang<jint, int >(in.valeur);
  klass->pos_piece = cxx2lang<Position*, position >(in.pos_piece);
  klass->tour_apparition = cxx2lang<jint, int >(in.tour_apparition);
  return klass;
}

template<class Lang, class Cxx>
std::vector<Cxx> lang2cxx_array(JArray<Lang*>* in)
{
  std::vector<Cxx> vect;
  vect.resize(lang2cxx<jint, int>(in->length));
  Lang** T = elements(in);

  for (int i = 0; i < vect.size(); i++)
    vect[i] = lang2cxx<Lang*, Cxx>(T[i]);

  return vect;
}

template<class Lang, class Cxx>
JArray<Lang*>* cxx2lang_array(const std::vector<Cxx>& in)
{
  JArray<Lang*>* out = ( JArray<Lang* > * ) JvNewObjectArray(in.size(), &Lang::class$ , NULL);
  Lang** T = elements(out);

  for (int i = 0; i < in.size(); i++)
    T[i] = cxx2lang<Lang*, Cxx>(in[i]);

  return out;
}
jint Interface::mon_equipe()
{
	return cxx2lang<jint, int >(api_mon_equipe());
}

jint Interface::score(jint team)
{
	return cxx2lang<jint, int >(api_score(lang2cxx<jint, int >(team)));
}

jint Interface::nombre_equipes()
{
	return cxx2lang<jint, int >(api_nombre_equipes());
}

jint Interface::tour_actuel()
{
	return cxx2lang<jint, int >(api_tour_actuel());
}

JArray< Piece* >* Interface::pieces_en_jeu()
{
	return cxx2lang_array< Piece, piece >(api_pieces_en_jeu());
}

JArray< Piece* >* Interface::pieces_a_vennir()
{
	return cxx2lang_array< Piece, piece >(api_pieces_a_vennir());
}

JArray< Unite* >* Interface::unites()
{
	return cxx2lang_array< Unite, unite >(api_unites());
}

Caracteristiques_objet* Interface::proprietes_objet(Type_objet* to)
{
	return cxx2lang<Caracteristiques_objet*, caracteristiques_objet >(api_proprietes_objet(lang2cxx<Type_objet*, type_objet >(to)));
}

Erreur* Interface::deplacer(Position* cible, Position* pos)
{
	return cxx2lang<Erreur*, erreur >(api_deplacer(lang2cxx<Position*, position >(cible), lang2cxx<Position*, position >(pos)));
}

Erreur* Interface::acheter_objet(Position* cible, Type_objet* objet)
{
	return cxx2lang<Erreur*, erreur >(api_acheter_objet(lang2cxx<Position*, position >(cible), lang2cxx<Type_objet*, type_objet >(objet)));
}

Erreur* Interface::utiliser(Position* attaquant, Position* cible)
{
	return cxx2lang<Erreur*, erreur >(api_utiliser(lang2cxx<Position*, position >(attaquant), lang2cxx<Position*, position >(cible)));
}

Erreur* Interface::achever_la_partie()
{
	return cxx2lang<Erreur*, erreur >(api_achever_la_partie());
}

void Interface::afficher_erreur(Erreur* v)
{
	api_afficher_erreur(lang2cxx<Erreur*, erreur >(v));
}

void Interface::afficher_type_objet(Type_objet* v)
{
	api_afficher_type_objet(lang2cxx<Type_objet*, type_objet >(v));
}

void Interface::afficher_position(Position* v)
{
	api_afficher_position(lang2cxx<Position*, position >(v));
}

void Interface::afficher_caracteristiques_objet(Caracteristiques_objet* v)
{
	api_afficher_caracteristiques_objet(lang2cxx<Caracteristiques_objet*, caracteristiques_objet >(v));
}

void Interface::afficher_unite(Unite* v)
{
	api_afficher_unite(lang2cxx<Unite*, unite >(v));
}

void Interface::afficher_piece(Piece* v)
{
	api_afficher_piece(lang2cxx<Piece*, piece >(v));
}

extern void GC_disable();

struct ProloginJavaVm
{
  ProloginJavaVm()
  {
    using namespace java::lang;
    try
    {
      // create the virtual machine
      JvCreateJavaVM(NULL);
      JvAttachCurrentThread(NULL, NULL);
      // FIXME: grrrr
      // GC_disable();
      JvInitClass(&::Erreur::class$);
      JvInitClass(&::Type_objet::class$);
      c = new Prologin();
    }
    catch (Throwable *t)
    {
      System::err->println(JvNewStringLatin1("Unhandled Java exception:"));
      t->printStackTrace();
    }
  }

  ~ProloginJavaVm()
  {
    // destroy the virual machine
    JvDetachCurrentThread();
  }

  Prologin* c;

};

struct ProloginJavaVm javaVm;
///
// Fonction appellée au début de la partie.
//
extern "C" void init_game()
{
  try {
    javaVm.c->init_game();
  } catch (java::lang::Throwable *t) {
    fprintf(stderr, "Unhandled Java exception:\n");
    t->printStackTrace();
  }
}

///
// Fonction appellée pour la phase de jeu.
//
extern "C" void jouer()
{
  try {
    javaVm.c->jouer();
  } catch (java::lang::Throwable *t) {
    fprintf(stderr, "Unhandled Java exception:\n");
    t->printStackTrace();
  }
}

///
// Fonction appellée à la fin de la partie.
//
extern "C" void end_game()
{
  try {
    javaVm.c->end_game();
  } catch (java::lang::Throwable *t) {
    fprintf(stderr, "Unhandled Java exception:\n");
    t->printStackTrace();
  }
}

