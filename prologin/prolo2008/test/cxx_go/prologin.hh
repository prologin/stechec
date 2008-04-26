///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

///
// Un des arguments passés à la fonction est incorrect.
//
# define BAD_ARGUMENT              -1

///
// Vous avez envoyé trop d'ordres à votre hamster.
//
# define TOO_MANY_ORDERS           -2

///
// La commande s'est correctement effectuée.
//
# define SUCCESS                   0

///
// La distance est infinie
//
# define INFINI                    10000

///
// Case de terrain qui n'est ni une tranchée, ni un obstacle
//
# define NORMAL                    0

///
// Tranchée du terrain
//
# define TRANCHEE                  1

///
// Obstacle du terrain
//
# define OBSTACLE                  2

# define HAUT                      0

# define GAUCHE                    1

# define BAS                       2

# define DROITE                    3

///
// Cette constante n'est utilisée que pour la fonction lacher\_pomme
//
# define ICI                       4


extern "C" {

///
// Renvoie la largeur de la carte
//
extern int taille_carte_x();

///
// Renvoie la hauteur de la carte
//
extern int taille_carte_y();

///
// Renvoie le numero du tour
//
extern int numero_tour();

///
// Renvoie le nombre de tours d'une partie
//
extern int nombre_tours();

///
// Fonction pour savoir si on joue en premier ce tour-ci
//
extern bool commence();

///
// Renvoie la position x du hamster id
//
extern int pos_x(int id);

///
// Renvoie la position y du hamster id
//
extern int pos_y(int id);

///
// Indique si le hamster id porte une pomme
//
extern int porte_pomme(int id);

///
// Renvoie la distance minimale pour aller de (x1,y1) à (x2,y2). ce est calculée en tenant compte des déplacements possibles d'un seul hamster. Cette fonction a un temps de calcul nul. Lorsque sort_tranchee vaut false, la distance est calculée en tenant compte des déplacements possibles d'un seul hamster. Lorsque sort_tranchee vaut true, la distance est calculée comme s'il n'y avait pas de tranchees.
//
extern int distance(int x1, int y1, int x2, int y2, bool sort_tranchee);

///
// Renvoie le type de la case (x,y) (NORMAL, TRANCHEE ou OBSTACLE)
//
extern int type_case(int x, int y);

///
// Indique s'il y a une pomme non portée sur la case (x,y)
//
extern int pomme(int x, int y);

///
// Renvoie le nombre de projectile restants du hamster id
//
extern int projectiles_restants(int id);

///
// Demande un déplacement du hamster
//
extern int deplacer(int id, int direction);

///
// Incrémente le turbo d'un hamster
//
extern int turbo(int id);

///
// Fait lacher la pomme à votre hamster, dans l'une des cinq directions
//
extern int lacher_pomme(int id, int direction);

///
// Demande à votre hamster de ramasser la pomme sur sa case courante
//
extern int ramasser_pomme(int id);

///
// Demande à votre hamster de lancer le grappin
//
extern int grappin(int id, int direction);

///
// Demande à votre hamster id de lancer un projectile dans une direction
//
extern int projectile(int id, int direction);

///
// Ordonne à votre hamster de ne rien faire
//
extern int attendre(int id);

///
// Fonction appele au debut de la partie
//
void init_game();

///
// Fonction appele a chaque tour
//
void play_turn();

}
