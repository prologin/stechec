///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <vector>

# include <string>

///
// Taille du terrain
//
# define TAILLE_TERRAIN            30

///
// Nombre de tours par partie
//
# define FIN_PARTIE                1500

///
// Nombre de points d'action par tour
//
# define MAX_PA                    3

///
// Taille des tra�n�es de moto
//
# define TAILLE_TRAINEE            120

///
// Longueur maximale de l'allongement
//
# define MAX_ALLONGEMENT           5

///
// Nombre de points d'action � rajouter avec bonus
//
# define AJOUT_PA                  5

///
// �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action
//
typedef enum erreur {
  OK, /* <- aucune erreur n'est survenue */
  ID_INVALIDE, /* <- identifiant invalide */
  POSITION_INVALIDE, /* <- la position sp�cifi�e est invalide */
  PLUS_DE_PA, /* <- vous n'avez pas assez de points d'action */
  BONUS_INVALIDE, /* <- vous n'avez pas ce bonus */
  PAS_A_TOI, /* <- l'unit� n'est pas a vous */
} erreur;


///
// �num�ration repr�sentant les diff�rents types de case
//
typedef enum type_case {
  VIDE, /* <- rien n'est pr�sent sur la case */
  OBSTACLE, /* <- cette case est inaccessible */
  POINT_CROISEMENT, /* <- point de croisement de tra�n�es */
  SOURCE, /* <- source ou consommateur d'energie */
} type_case;


///
// �num�ration repr�sentant les diff�rents types de bonii
//
typedef enum type_bonus {
  PAS_BONUS, /* <- ceci n'est pas un bonus :-) */
  BONUS_CROISEMENT, /* <- bonus permettant de croiser deux tra�n�es de moto sur une case */
  PLUS_LONG, /* <- bonus permettant d'agrandir une tra�n�e de moto */
  PLUS_PA, /* <- bonus permettant d'avoir plus de points d'action */
  BONUS_REGENERATION, /* <- bonus permettant de regenerer une source d'energie */
} type_bonus;


///
// Repr�sente une position sur le terrain du jeu
//
typedef struct position {
  int x;  /* <- coordonn�e en X */
  int y;  /* <- coordonn�e en Y */
} position;


///
// Caracteristiques d'une source d'�nergie
//
typedef struct source_energie {
  int id;  /* <- identifiant de la source d'�nergie */
  position pos;  /* <- position de la source d'�nergie */
  int capacite;  /* <- coefficient repr�sentant les points d'�nergie que la source va vous apporter */
  int capacite_max;  /* <- coefficient repr�sentant la capacit� de la source lorsqu'elle est charg�e au maximum */
} source_energie;


///
// Repr�sente une tra�n�e de moto sur le terrain
//
typedef struct trainee_moto {
  int id;  /* <- identifiant de la tra�nee */
  std::vector<position> emplacement;  /* <- position de chaque composant de la tra�n�e de moto */
  int team;  /* <- identifiant de l'�quipe qui poss�de cette tra�n�e de moto */
  int longueur;  /* <- taille maximale de la tra�n�e */
} trainee_moto;


///
// Retourne le num�ro de votre �quipe
//
extern "C" int api_mon_equipe();
static inline int mon_equipe()
{
  return api_mon_equipe();
}


///
// Retourne les scores de chaque �quipe
//
extern "C" std::vector<int> api_scores();
static inline std::vector<int> scores()
{
  return api_scores();
}


///
// Retourne le nombre d'�quipes sur le terrain
//
extern "C" int api_nombre_equipes();
static inline int nombre_equipes()
{
  return api_nombre_equipes();
}


///
// Retourne le num�ro du tour actuel
//
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


///
// Retourne la liste des sources d'�nergie
//
extern "C" std::vector<source_energie> api_sources_energie();
static inline std::vector<source_energie> sources_energie()
{
  return api_sources_energie();
}


///
// Retourne la liste des tra�n�es de moto
//
extern "C" std::vector<trainee_moto> api_trainees_moto();
static inline std::vector<trainee_moto> trainees_moto()
{
  return api_trainees_moto();
}


///
// Retourne le type d'une case
//
extern "C" type_case api_regarder_type_case(position pos);
static inline type_case regarder_type_case(position pos)
{
  return api_regarder_type_case(pos);
}


///
// Retourne le type de bonus d'une case
//
extern "C" type_bonus api_regarder_type_bonus(position pos);
static inline type_bonus regarder_type_bonus(position pos)
{
  return api_regarder_type_bonus(pos);
}


///
// Retourne la liste des bonus d'une �quipe
//
extern "C" std::vector<type_bonus> api_regarder_bonus(int equipe);
static inline std::vector<type_bonus> regarder_bonus(int equipe)
{
  return api_regarder_bonus(equipe);
}


///
// Retourne la liste des id des tra�n�es pr�sentes sur une case
//
extern "C" std::vector<int> api_regarder_trainee_case(position pos);
static inline std::vector<int> regarder_trainee_case(position pos)
{
  return api_regarder_trainee_case(pos);
}


///
// Retourne si une case peut �tre travers�e par une tra�n�e de plus
//
extern "C" bool api_case_traversable(position pos);
static inline bool case_traversable(position pos)
{
  return api_case_traversable(pos);
}


///
// Renvoie les points que vous allez gagner a la fin du tour
//
extern "C" int api_diff_score();
static inline int diff_score()
{
  return api_diff_score();
}


///
// Renvoie le chemin le plus court entre deux points (fonction lente)
//
extern "C" std::vector<position> api_chemin(position p1, position p2);
static inline std::vector<position> chemin(position p1, position p2)
{
  return api_chemin(p1, p2);
}


///
// D�place une moto
//
extern "C" erreur api_deplacer(int id, position de, position vers);
static inline erreur deplacer(int id, position de, position vers)
{
  return api_deplacer(id, de, vers);
}


///
// Coupe une tra�n�e de moto en deux nouvelles tra�n�es. � entre � et � et � doivent �tre deux positions adjacentes occup�es par une m�me tra�n�e de moto.
//
extern "C" erreur api_couper_trainee_moto(int id, position entre, position et);
static inline erreur couper_trainee_moto(int id, position entre, position et)
{
  return api_couper_trainee_moto(id, entre, et);
}


///
// Annuler l'action pr�c�dente
//
extern "C" erreur api_cancel();
static inline erreur cancel()
{
  return api_cancel();
}


///
// Enrouler la tra�n�e de moto en un point
//
extern "C" erreur api_enrouler(int id, position point);
static inline erreur enrouler(int id, position point)
{
  return api_enrouler(id, point);
}


///
// R�g�n�re une source d'�nergie � son maximal
//
extern "C" erreur api_regenerer_source_energie(int id);
static inline erreur regenerer_source_energie(int id)
{
  return api_regenerer_source_energie(id);
}


///
// Allonge le tour en rajoutant des points d'action
//
extern "C" erreur api_allonger_pa();
static inline erreur allonger_pa()
{
  return api_allonger_pa();
}


///
// Allonge une tra�n�e de moto. L'allongement se fera aux prochains d�placements. La longueur du prolongement doit �tre comprise entre 0 et MAX_ALLONGEMENT (inclus).
//
extern "C" erreur api_agrandir_trainee_moto(int id, int longueur);
static inline erreur agrandir_trainee_moto(int id, int longueur)
{
  return api_agrandir_trainee_moto(id, longueur);
}


///
// Pose un point de croisement sur une case du terrain. La case doit ne pas d�j� �tre un point de croisement.
//
extern "C" erreur api_poser_point_croisement(position point);
static inline erreur poser_point_croisement(position point)
{
  return api_poser_point_croisement(point);
}


///
// Fusionner deux tra�n�es de moto. Les deux doivent appartenir � la m�me �quipe, mais doivent �tre deux tra�n�es distinctes. � pos1 � et � pos2 � doivent �tre adjacentes et occup�es respectivement par � id1 � et � id2 �.
//
extern "C" erreur api_fusionner(int id1, position pos1, int id2, position pos2);
static inline erreur fusionner(int id1, position pos1, int id2, position pos2)
{
  return api_fusionner(id1, pos1, id2, pos2);
}


///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);
static inline void afficher_erreur(erreur v)
{
  api_afficher_erreur(v);
}


///
// Affiche le contenu d'une valeur de type type_case
//
extern "C" void api_afficher_type_case(type_case v);
static inline void afficher_type_case(type_case v)
{
  api_afficher_type_case(v);
}


///
// Affiche le contenu d'une valeur de type type_bonus
//
extern "C" void api_afficher_type_bonus(type_bonus v);
static inline void afficher_type_bonus(type_bonus v)
{
  api_afficher_type_bonus(v);
}


///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(position v);
static inline void afficher_position(position v)
{
  api_afficher_position(v);
}


///
// Affiche le contenu d'une valeur de type source_energie
//
extern "C" void api_afficher_source_energie(source_energie v);
static inline void afficher_source_energie(source_energie v)
{
  api_afficher_source_energie(v);
}


///
// Affiche le contenu d'une valeur de type trainee_moto
//
extern "C" void api_afficher_trainee_moto(trainee_moto v);
static inline void afficher_trainee_moto(trainee_moto v)
{
  api_afficher_trainee_moto(v);
}



extern "C" {

///
// Fonction appell�e au d�but de la partie
//
void init_game();

///
// Fonction appell�e pour la phase de jeu
//
void jouer();

///
// Fonction appell�e � la fin de la partie
//
void end_game();

}
#endif
