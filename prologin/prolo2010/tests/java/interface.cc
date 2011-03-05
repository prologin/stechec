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
// Le type d'une unité sur le terrain
//
template<>
type_unite lang2cxx<Type_unite*, type_unite >(Type_unite* in)
{
  return type_unite(lang2cxx<jint, int>( in->ordinal() ));
}
template<>
Type_unite* cxx2lang<Type_unite*, type_unite >(type_unite in)
{
  return *(&Type_unite::Perroquet + (int)in);
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
// Contient les informations sur la taille du terrain du jeu.
//
template <>
taille_terrain lang2cxx<Taille_terrain* , taille_terrain >(Taille_terrain* in){
  taille_terrain out;
  out.taille = lang2cxx<jint, int >(in->taille);
  out.min_coord = lang2cxx<jint, int >(in->min_coord);
  out.max_coord = lang2cxx<jint, int >(in->max_coord);
  return out;
}
template <>
Taille_terrain* cxx2lang<Taille_terrain*, taille_terrain>(taille_terrain in){
  Taille_terrain* klass = new Taille_terrain();
  klass->taille = cxx2lang<jint, int >(in.taille);
  klass->min_coord = cxx2lang<jint, int >(in.min_coord);
  klass->max_coord = cxx2lang<jint, int >(in.max_coord);
  return klass;
}

///
// Donne les caractéristiques d'un type d'unité.
//
template <>
caracs lang2cxx<Caracs* , caracs >(Caracs* in){
  caracs out;
  out.pa_init = lang2cxx<jint, int >(in->pa_init);
  out.portee = lang2cxx<jint, int >(in->portee);
  return out;
}
template <>
Caracs* cxx2lang<Caracs*, caracs>(caracs in){
  Caracs* klass = new Caracs();
  klass->pa_init = cxx2lang<jint, int >(in.pa_init);
  klass->portee = cxx2lang<jint, int >(in.portee);
  return klass;
}

///
// Représente une unité sur le terrain.
//
template <>
unite lang2cxx<Unite* , unite >(Unite* in){
  unite out;
  out.pos = lang2cxx<Position*, position >(in->pos);
  out.ennemi = lang2cxx<jboolean, bool >(in->ennemi);
  out.type_unite_actuel = lang2cxx<Type_unite*, type_unite >(in->type_unite_actuel);
  out.vrai_type_unite = lang2cxx<Type_unite*, type_unite >(in->vrai_type_unite);
  out.ko = lang2cxx<jint, int >(in->ko);
  out.pa = lang2cxx<jint, int >(in->pa);
  out.attaques = lang2cxx<jint, int >(in->attaques);
  out.attaques_gratuites = lang2cxx<jint, int >(in->attaques_gratuites);
  out.id = lang2cxx<jint, int >(in->id);
  return out;
}
template <>
Unite* cxx2lang<Unite*, unite>(unite in){
  Unite* klass = new Unite();
  klass->pos = cxx2lang<Position*, position >(in.pos);
  klass->ennemi = cxx2lang<jboolean, bool >(in.ennemi);
  klass->type_unite_actuel = cxx2lang<Type_unite*, type_unite >(in.type_unite_actuel);
  klass->vrai_type_unite = cxx2lang<Type_unite*, type_unite >(in.vrai_type_unite);
  klass->ko = cxx2lang<jint, int >(in.ko);
  klass->pa = cxx2lang<jint, int >(in.pa);
  klass->attaques = cxx2lang<jint, int >(in.attaques);
  klass->attaques_gratuites = cxx2lang<jint, int >(in.attaques_gratuites);
  klass->id = cxx2lang<jint, int >(in.id);
  return klass;
}

///
// Représente l'ensemble des cartes que vous pouvez utiliser.
//
template <>
cartes lang2cxx<Cartes* , cartes >(Cartes* in){
  cartes out;
  out.potion = lang2cxx<jint, int >(in->potion);
  out.deguisement = lang2cxx<jint, int >(in->deguisement);
  out.banzai = lang2cxx<jint, int >(in->banzai);
  out.pacifisme = lang2cxx<jint, int >(in->pacifisme);
  return out;
}
template <>
Cartes* cxx2lang<Cartes*, cartes>(cartes in){
  Cartes* klass = new Cartes();
  klass->potion = cxx2lang<jint, int >(in.potion);
  klass->deguisement = cxx2lang<jint, int >(in.deguisement);
  klass->banzai = cxx2lang<jint, int >(in.banzai);
  klass->pacifisme = cxx2lang<jint, int >(in.pacifisme);
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
jint Interface::nombre_pc()
{
	return cxx2lang<jint, int >(api_nombre_pc());
}

jint Interface::nombre_unites(jboolean ennemi)
{
	return cxx2lang<jint, int >(api_nombre_unites(lang2cxx<jboolean, bool >(ennemi)));
}

jint Interface::tour_actuel()
{
	return cxx2lang<jint, int >(api_tour_actuel());
}

Position* Interface::pos_renfort(jboolean ennemi)
{
	return cxx2lang<Position*, position >(api_pos_renfort(lang2cxx<jboolean, bool >(ennemi)));
}

Caracs* Interface::caracteristiques(Type_unite* tu)
{
	return cxx2lang<Caracs*, caracs >(api_caracteristiques(lang2cxx<Type_unite*, type_unite >(tu)));
}

Cartes* Interface::mes_cartes()
{
	return cxx2lang<Cartes*, cartes >(api_mes_cartes());
}

JArray< Unite* >* Interface::unites()
{
	return cxx2lang_array< Unite, unite >(api_unites());
}

Taille_terrain* Interface::taille_terrain_actuelle()
{
	return cxx2lang<Taille_terrain*, taille_terrain >(api_taille_terrain_actuelle());
}

Erreur* Interface::potion(Position* cible)
{
	return cxx2lang<Erreur*, erreur >(api_potion(lang2cxx<Position*, position >(cible)));
}

Erreur* Interface::deguisement(Position* cible, Type_unite* nouveau_type)
{
	return cxx2lang<Erreur*, erreur >(api_deguisement(lang2cxx<Position*, position >(cible), lang2cxx<Type_unite*, type_unite >(nouveau_type)));
}

Erreur* Interface::banzai(Position* cible)
{
	return cxx2lang<Erreur*, erreur >(api_banzai(lang2cxx<Position*, position >(cible)));
}

Erreur* Interface::pacifisme()
{
	return cxx2lang<Erreur*, erreur >(api_pacifisme());
}

Erreur* Interface::deplacer(Position* cible, Position* pos)
{
	return cxx2lang<Erreur*, erreur >(api_deplacer(lang2cxx<Position*, position >(cible), lang2cxx<Position*, position >(pos)));
}

Erreur* Interface::relever(Position* cible)
{
	return cxx2lang<Erreur*, erreur >(api_relever(lang2cxx<Position*, position >(cible)));
}

Erreur* Interface::attaquer(Position* attaquant, Position* cible)
{
	return cxx2lang<Erreur*, erreur >(api_attaquer(lang2cxx<Position*, position >(attaquant), lang2cxx<Position*, position >(cible)));
}

Erreur* Interface::renfort(Type_unite* quoi)
{
	return cxx2lang<Erreur*, erreur >(api_renfort(lang2cxx<Type_unite*, type_unite >(quoi)));
}

jboolean Interface::annuler()
{
	return cxx2lang<jboolean, bool >(api_annuler());
}

void Interface::afficher_erreur(Erreur* v)
{
	api_afficher_erreur(lang2cxx<Erreur*, erreur >(v));
}

void Interface::afficher_type_unite(Type_unite* v)
{
	api_afficher_type_unite(lang2cxx<Type_unite*, type_unite >(v));
}

void Interface::afficher_position(Position* v)
{
	api_afficher_position(lang2cxx<Position*, position >(v));
}

void Interface::afficher_taille_terrain(Taille_terrain* v)
{
	api_afficher_taille_terrain(lang2cxx<Taille_terrain*, taille_terrain >(v));
}

void Interface::afficher_caracs(Caracs* v)
{
	api_afficher_caracs(lang2cxx<Caracs*, caracs >(v));
}

void Interface::afficher_unite(Unite* v)
{
	api_afficher_unite(lang2cxx<Unite*, unite >(v));
}

void Interface::afficher_cartes(Cartes* v)
{
	api_afficher_cartes(lang2cxx<Cartes*, cartes >(v));
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
      JvInitClass(&::Type_unite::class$);
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
// Fonction appellée pour la phase de retrait de KO.
//
extern "C" position retirer_ko()
{
  try {
return lang2cxx<Position*, position >(    javaVm.c->retirer_ko());
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

