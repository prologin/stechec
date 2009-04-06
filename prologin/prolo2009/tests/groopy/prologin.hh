///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

///
// Renvoyé par cout_achat_maison lorsque vous ne pouvez plus construire de maison pour le tour courant.
//
# define INFINI                    30000

///
// Les coordonnées (x,y) avec lesquelles vous avec appelé une fonction sont hors des bornes du terrain.
//
# define HORS_TERRAIN              -12

///
// Il n'y a pas de maison à la case indiquée.
//
# define PAS_DE_MAISON             -2

///
// Il n'y a pas de monument à la case indiquée.
//
# define PAS_DE_MONUMENT           -3

///
// Vous voulez dépenser plus d'argent que vous n'avez, ou vous ne pouvez plus construire de maison ce tour-ci.
//
# define FINANCES_DEPASSEES        -4

///
// Un blocage intervient lorsqu'aucune route n'est accessible par rapport à l'extérieur du jeu. Ce code d'erreur indique que le jeu serait bloqué si vous construisiez une maison ou un monument en (x,y).
//
# define BLOCAGE                   -5

///
// Le numéro de joueur donné est incorrect.
//
# define JOUEUR_INCORRECT          -7

///
// La case indiquée n'est pas adjacente à une route (pour la 4-connexité si vous voulez construire une route, et pour la 8-connexité si vous voulez construire un bâtiment).
//
# define NON_CONNEXE               -8

///
// La case indiquée est déjà occupée.
//
# define CASE_OCCUPEE              -9

///
// Ce n'est pas votre tour, ou l'action demandée est interdite.
//
# define ACTION_INTERDITE          -10

///
// La distance demandée est strictement supérieure à MAX_DISTANCE.
//
# define TROP_LOIN                 50

///
// L'action s'est déroulée avec succès.
//
# define SUCCES                    0

///
// La case ou la bâtiment est possédée par la mairie.
//
# define MAIRIE                    3

///
// Case de terrain vide.
//
# define VIDE                      0

///
// Case de terrain qui contient une maison.
//
# define MAISON                    1

///
// Case de terrain réservée.
//
# define RESERVATION               2

///
// Case de terrain qui contient un monument.
//
# define MONUMENT                  3

///
// Case de terrain qui contient une route.
//
# define ROUTE                     4

///
// Nombre de monuments qui seront mis aux enchères.
//
# define MAX_MONUMENTS             14

///
// La taille de la carte, en hauteur et en largeur.
//
# define TAILLE_CARTE              100

///
// Distance maximale qui peut être calculée.
//
# define MAX_DISTANCE              12


extern "C" {

///
// Renvoie le type de la case, VIDE, MAISON, ROUTE, MONUMENT ou RESERVATION.
//
extern int type_case(int x, int y);

///
// Renvoie la valeur de la case, c'est-à-dire la somme des prestiges des monuments qui ont cette case dans leur portée.
//
extern int valeur_case(int x, int y);

///
// Renvoie l'entité possédant la case, 0, 1, 2 ou MAIRIE si la case est une maison ou une réservation, et MAIRIE sinon.
//
extern int appartenance(int x, int y);

///
// Renvoie le numéro d'un monument sur une case donnée, ou PAS_DE_MONUMENT.
//
extern int type_monument(int x, int y);

///
// Renvoie la portée d'un monument.
//
extern int portee_monument(int num_monument);

///
// Renvoie le prestige d'un monument
//
extern int prestige_monument(int num_monument);

///
// Renvoie le numéro du tour
//
extern int numero_tour();

///
// Numéro du joueur qui commence lors de la phase 1. Si cette fonction est appelée pendant une phase 2 ou 3, renoie le numéro du joueur qui commencera lors de la phase 1 du tour suivant.
//
extern int commence();

///
// Donne le montant de la dernière enchère effectuée par un joueur. Renvoie 0 au premier tour.
//
extern int montant_encheres(int num_joueur);

///
// Donne le score d'un joueur.
//
extern int score(int num_joueur);

///
// Donne l'argent possédé par un joueur
//
extern int finances(int num_joueur);

///
// Donne le numéro du monument le plus récemment mis aux enchères. Renvoie PAS_DE_MONUMENT lors de la phase 1 du premier tour.
//
extern int monument_en_cours();

///
// Renvoie la distance minimale pour aller de (x1,y1) à (x2,y2). Lors du calcul de cette distance, seules les cases vides, les cases contenant une route, et les cases étant réservée par votre champion sont considérées comme franchissables. Si le résultat est supérieur strictement à MAX_DISTANCE, la fonction renvoie TROP_LOIN. Le coût d'un appel à cette fonction est nul.
//
extern int distance(int x1, int y1, int x2, int y2);

///
// Indique si vous pouvez construire une route en (x,y). Si non, renvoie HORS_TERRAIN, CASE_OCCUPEE ou NON_CONNEXE.
//
extern int route_possible(int x, int y);

///
// Indique si vous pouvez construire une maison ou un monument en (x,y). Si non, renvoie HORS_TERRAIN, CASE_OCCUPEE, BLOCAGE ou NON_CONNEXE.
//
extern int construction_possible(int x, int y);

///
// Renvoie le coût d'achat d'une maison (ou INFINI si vous ne pouvez plus en construire pour ce tour).
//
extern int cout_achat_maison();

///
// Renvoie le coût d'achat d'une route.
//
extern int cout_achat_route();

///
// Indique si votre champion doit jouer ou non. Si cette fonction renvoie false, votre champion ne pourra effectuer aucune action.
//
extern bool mon_tour();

///
// Construit si possible une route en (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES, NON_CONNEXE ou CASE_OCCUPEE.
//
extern int construire_route(int x, int y);

///
// Construit si possible une maison en (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES, NON_CONNEXE, BLOCAGE ou CASE_OCCUPEE.
//
extern int construire_maison(int x, int y);

///
// Réserve la case (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES, NON_CONNEXE, BLOCAGE ou CASE_OCCUPEE.
//
extern int reserver_case(int x, int y);

///
// Détruit la maison en (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES ou PAS_DE_MAISON.
//
extern int detruire_maison(int x, int y);

///
// Vend la maison en (x,y). Si impossible, renvoie ACTION_INTERDITE ou PAS_DE_MAISON.
//
extern int vendre_maison(int x, int y);

///
// Place une enchère. Renvoie ACTION_INTERDITE ou FINANCES_DEPASSEES si impossible.
//
extern int encherir(int montant);

///
// Construit un monument en (x,y). Si impossible, renvoie ACTION_INTERDITE, NON_CONNEXE, BLOCAGE ou CASE_OCCUPEE.
//
extern int construire_monument(int x, int y);

///
// Fonction appelee au debut de la partie
//
void init_game();

///
// Fonction appelee a la fin de la partie
//
void end_game();

///
// Fonction appelee a chaque phase de jeu
//
void jouer();

///
// Fonction appelee a chaque phase d'enchere
//
void enchere();

///
// Fonction appelee a chaque phase de placement de monument
//
void placement();

}
