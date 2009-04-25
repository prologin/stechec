///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

///
// Renvoy� par cout_achat_maison lorsque vous ne pouvez plus construire de maison pour le tour courant.
//
# define INFINI                    30000

///
// Les coordonn�es (x,y) avec lesquelles vous avec appel� une fonction sont hors des bornes du terrain.
//
# define HORS_TERRAIN              -12

///
// Il n'y a pas de maison � la case indiqu�e.
//
# define PAS_DE_MAISON             -2

///
// Il n'y a pas de monument � la case indiqu�e.
//
# define PAS_DE_MONUMENT           -3

///
// Vous voulez d�penser plus d'argent que vous n'avez, ou vous ne pouvez plus construire de maison ce tour-ci.
//
# define FINANCES_DEPASSEES        -4

///
// Un blocage intervient lorsqu'aucune route n'est accessible par rapport � l'ext�rieur du jeu. Ce code d'erreur indique que le jeu serait bloqu� si vous construisiez une maison ou un monument en (x,y).
//
# define BLOCAGE                   -5

///
// Le num�ro de joueur donn� est incorrect.
//
# define JOUEUR_INCORRECT          -7

///
// La case indiqu�e n'est pas adjacente � une route (pour la 4-connexit� si vous voulez construire une route, et pour la 8-connexit� si vous voulez construire un b�timent).
//
# define NON_CONNEXE               -8

///
// La case indiqu�e est d�j� occup�e.
//
# define CASE_OCCUPEE              -9

///
// Ce n'est pas votre tour, ou l'action demand�e est interdite.
//
# define ACTION_INTERDITE          -10

///
// La distance demand�e est strictement sup�rieure � MAX_DISTANCE.
//
# define TROP_LOIN                 50

///
// L'action s'est d�roul�e avec succ�s.
//
# define SUCCES                    0

///
// La case ou la b�timent est poss�d�e par la mairie.
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
// Case de terrain r�serv�e.
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
// Nombre de monuments qui seront mis aux ench�res.
//
# define MAX_MONUMENTS             14

///
// La taille de la carte, en hauteur et en largeur.
//
# define TAILLE_CARTE              100

///
// Distance maximale qui peut �tre calcul�e.
//
# define MAX_DISTANCE              12


extern "C" {

///
// Renvoie le type de la case, VIDE, MAISON, ROUTE, MONUMENT ou RESERVATION.
//
extern int type_case(int x, int y);

///
// Renvoie la valeur de la case, c'est-�-dire la somme des prestiges des monuments qui ont cette case dans leur port�e.
//
extern int valeur_case(int x, int y);

///
// Renvoie l'entit� poss�dant la case, 0, 1, 2 ou MAIRIE si la case est une maison ou une r�servation, et MAIRIE sinon.
//
extern int appartenance(int x, int y);

///
// Renvoie le num�ro d'un monument sur une case donn�e, ou PAS_DE_MONUMENT.
//
extern int type_monument(int x, int y);

///
// Renvoie la port�e d'un monument.
//
extern int portee_monument(int num_monument);

///
// Renvoie le prestige d'un monument
//
extern int prestige_monument(int num_monument);

///
// Renvoie le num�ro du tour
//
extern int numero_tour();

///
// Num�ro du joueur qui commence lors de la phase 1. Si cette fonction est appel�e pendant une phase 2 ou 3, renoie le num�ro du joueur qui commencera lors de la phase 1 du tour suivant.
//
extern int commence();

///
// Donne le montant de la derni�re ench�re effectu�e par un joueur. Renvoie 0 au premier tour.
//
extern int montant_encheres(int num_joueur);

///
// Donne le score d'un joueur.
//
extern int score(int num_joueur);

///
// Donne l'argent poss�d� par un joueur
//
extern int finances(int num_joueur);

///
// Donne le num�ro du monument le plus r�cemment mis aux ench�res. Renvoie PAS_DE_MONUMENT lors de la phase 1 du premier tour.
//
extern int monument_en_cours();

///
// Renvoie la distance minimale pour aller de (x1,y1) � (x2,y2). Lors du calcul de cette distance, seules les cases vides, les cases contenant une route, et les cases �tant r�serv�e par votre champion sont consid�r�es comme franchissables. Si le r�sultat est sup�rieur strictement � MAX_DISTANCE, la fonction renvoie TROP_LOIN. Le co�t d'un appel � cette fonction est nul.
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
// Renvoie le co�t d'achat d'une maison (ou INFINI si vous ne pouvez plus en construire pour ce tour).
//
extern int cout_achat_maison();

///
// Renvoie le co�t d'achat d'une route.
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
// R�serve la case (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES, NON_CONNEXE, BLOCAGE ou CASE_OCCUPEE.
//
extern int reserver_case(int x, int y);

///
// D�truit la maison en (x,y). Si impossible, renvoie ACTION_INTERDITE, FINANCES_DEPASSEES ou PAS_DE_MAISON.
//
extern int detruire_maison(int x, int y);

///
// Vend la maison en (x,y). Si impossible, renvoie ACTION_INTERDITE ou PAS_DE_MAISON.
//
extern int vendre_maison(int x, int y);

///
// Place une ench�re. Renvoie ACTION_INTERDITE ou FINANCES_DEPASSEES si impossible.
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
