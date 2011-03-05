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
// Énumération représentant une erreur renvoyée par une des fonctions d'action
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
// Énumération représentant les différents types de case
//
template<>
type_case lang2cxx<Type_case*, type_case >(Type_case* in)
{
  return type_case(lang2cxx<jint, int>( in->ordinal() ));
}
template<>
Type_case* cxx2lang<Type_case*, type_case >(type_case in)
{
  return *(&Type_case::Vide + (int)in);
}

///
// Énumération représentant les différents types de bonii
//
template<>
bonus lang2cxx<Bonus*, bonus >(Bonus* in)
{
  return bonus(lang2cxx<jint, int>( in->ordinal() ));
}
template<>
Bonus* cxx2lang<Bonus*, bonus >(bonus in)
{
  return *(&Bonus::Vide + (int)in);
}

///
// Représente une position sur le terrain du jeu
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
// Caracteristiques d'une source d'énergie
//
template <>
source_energie lang2cxx<Source_energie* , source_energie >(Source_energie* in){
  source_energie out;
  out.id = lang2cxx<jint, int >(in->id);
  out.pos = lang2cxx<Position*, position >(in->pos);
  out.coef = lang2cxx<jint, int >(in->coef);
  return out;
}
template <>
Source_energie* cxx2lang<Source_energie*, source_energie>(source_energie in){
  Source_energie* klass = new Source_energie();
  klass->id = cxx2lang<jint, int >(in.id);
  klass->pos = cxx2lang<Position*, position >(in.pos);
  klass->coef = cxx2lang<jint, int >(in.coef);
  return klass;
}

///
// Représente une traînée de moto sur le terrain
//
template <>
trainee_moto lang2cxx<Trainee_moto* , trainee_moto >(Trainee_moto* in){
  trainee_moto out;
  out.id = lang2cxx<jint, int >(in->id);
  out.emplacement = lang2cxx_array< Position, position >(in->emplacement);
  out.team = lang2cxx<jint, int >(in->team);
  return out;
}
template <>
Trainee_moto* cxx2lang<Trainee_moto*, trainee_moto>(trainee_moto in){
  Trainee_moto* klass = new Trainee_moto();
  klass->id = cxx2lang<jint, int >(in.id);
  klass->emplacement = cxx2lang_array< Position, position >(in.emplacement);
  klass->team = cxx2lang<jint, int >(in.team);
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
JArray< Int* >* Interface::scores()
{
	return cxx2lang_array< Int, int >(api_scores());
}
jint Interface::nombre_equipes()
{
	return cxx2lang<jint, int >(api_nombre_equipes());
}
jint Interface::tour_actuel()
{
	return cxx2lang<jint, int >(api_tour_actuel());
}
JArray< Source_energie* >* Interface::sources_energie()
{
	return cxx2lang_array< Source_energie, source_energie >(api_sources_energie());
}
JArray< Trainee_moto* >* Interface::trainees_moto()
{
	return cxx2lang_array< Trainee_moto, trainee_moto >(api_trainees_moto());
}
Type_case* Interface::regarder_type_case(Position* pos)
{
	return cxx2lang<Type_case*, type_case >(api_regarder_type_case(lang2cxx<Position*, position >(pos)));
}
Bonus* Interface::regarder_type_bonus(Position* pos)
{
	return cxx2lang<Bonus*, bonus >(api_regarder_type_bonus(lang2cxx<Position*, position >(pos)));
}
JArray< Bonus* >* Interface::regarder_bonus(jint equipe)
{
	return cxx2lang_array< Bonus, bonus >(api_regarder_bonus(lang2cxx<jint, int >(equipe)));
}
Erreur* Interface::deplacer(jint id, Position* de, Position* vers)
{
	return cxx2lang<Erreur*, erreur >(api_deplacer(lang2cxx<jint, int >(id), lang2cxx<Position*, position >(de), lang2cxx<Position*, position >(vers)));
}
Erreur* Interface::couper_trainee_moto(jint id, Position* entre, Position* et)
{
	return cxx2lang<Erreur*, erreur >(api_couper_trainee_moto(lang2cxx<jint, int >(id), lang2cxx<Position*, position >(entre), lang2cxx<Position*, position >(et)));
}
Erreur* Interface::cancel()
{
	return cxx2lang<Erreur*, erreur >(api_cancel());
}
Erreur* Interface::enrouler(jint id, Position* point)
{
	return cxx2lang<Erreur*, erreur >(api_enrouler(lang2cxx<jint, int >(id), lang2cxx<Position*, position >(point)));
}
Erreur* Interface::regenerer_source_energie(jint id)
{
	return cxx2lang<Erreur*, erreur >(api_regenerer_source_energie(lang2cxx<jint, int >(id)));
}
Erreur* Interface::allonger_pa()
{
	return cxx2lang<Erreur*, erreur >(api_allonger_pa());
}
Erreur* Interface::agrandir_trainee_moto(jint id, jint longueur)
{
	return cxx2lang<Erreur*, erreur >(api_agrandir_trainee_moto(lang2cxx<jint, int >(id), lang2cxx<jint, int >(longueur)));
}
Erreur* Interface::poser_point_croisement(Position* point)
{
	return cxx2lang<Erreur*, erreur >(api_poser_point_croisement(lang2cxx<Position*, position >(point)));
}
Erreur* Interface::fusionner(jint id1, Position* pos1, jint id2, Position* pos2)
{
	return cxx2lang<Erreur*, erreur >(api_fusionner(lang2cxx<jint, int >(id1), lang2cxx<Position*, position >(pos1), lang2cxx<jint, int >(id2), lang2cxx<Position*, position >(pos2)));
}
void Interface::afficher_erreur(Erreur* v)
{
	api_afficher_erreur(lang2cxx<Erreur*, erreur >(v));
}
void Interface::afficher_type_case(Type_case* v)
{
	api_afficher_type_case(lang2cxx<Type_case*, type_case >(v));
}
void Interface::afficher_bonus(Bonus* v)
{
	api_afficher_bonus(lang2cxx<Bonus*, bonus >(v));
}
void Interface::afficher_position(Position* v)
{
	api_afficher_position(lang2cxx<Position*, position >(v));
}
void Interface::afficher_source_energie(Source_energie* v)
{
	api_afficher_source_energie(lang2cxx<Source_energie*, source_energie >(v));
}
void Interface::afficher_trainee_moto(Trainee_moto* v)
{
	api_afficher_trainee_moto(lang2cxx<Trainee_moto*, trainee_moto >(v));
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
      JvInitClass(&::Type_case::class$);
      JvInitClass(&::Bonus::class$);
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
// Fonction appellée au début de la partie
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
// Fonction appellée pour la phase de jeu
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
// Fonction appellée à la fin de la partie
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

