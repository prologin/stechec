/*!
** This file has been generated, if you wish to
** modify it in a permanent way, please refer
** to the script file : gen/generator_c.rb
*/

/*!
** todo
*/
# define INFINI                    30000

/*!
** todo
*/
# define HORS_TERRAIN              -1

/*!
** todo
*/
# define PAS_DE_MAISON             -2

/*!
** todo
*/
# define PAS_DE_MONUMENT           -3

/*!
** todo
*/
# define FINANCES_DEPASSEES        -4

/*!
** todo
*/
# define BLOCAGE                   -5

/*!
** todo
*/
# define TROP_DE_MAISONS           -6

/*!
** todo
*/
# define JOUEUR_INCORRECT          -7

/*!
** todo
*/
# define NON_CONNEXE               -8

/*!
** todo
*/
# define CASE_OCCUPEE              -9

/*!
** todo
*/
# define ACTION_INTERDITE          -10

/*!
** todo
*/
# define TROP_LOIN                 -11

/*!
** todo
*/
# define SUCCES                    0

/*!
** todo
*/
# define MAIRIE                    3

/*!
** Case de terrain vide
*/
# define VIDE                      0

/*!
** Case de terrain qui contient une maison
*/
# define MAISON                    1

/*!
** Case de terrain réservée
*/
# define RESERVATION               2

/*!
** Case de terrain qui contient un monument
*/
# define MONUMENT                  3

/*!
** Case de terrain qui contient une route
*/
# define ROUTE                     4

/*!
** todo
*/
# define MAX_MONUMENTS             14

/*!
** todo
*/
# define TAILLE_CARTE              100

/*!
** Renvoie le type de la case
*/
extern int type_case(int x, int y);

/*!
** Renvoie la valeur de la case
*/
extern int valeur_case(int x, int y);

/*!
** todo
*/
extern int appartenance(int x, int y);

/*!
** todo
*/
extern int type_monument(int x, int y);

/*!
** todo
*/
extern int portee_monument(int num_monument);

/*!
** todo
*/
extern int prestige_monument(int num_monument);

/*!
** Renvoie le numero du tour
*/
extern int numero_tour(void);

/*!
** Numero du joueur qui commence
*/
extern int commence(void);

/*!
** todo
*/
extern int montant_encheres(int num_joueur);

/*!
** todo
*/
extern int score(int num_joueur);

/*!
** todo
*/
extern int finances(int num_joueur);

/*!
** todo
*/
extern int monument_en_cours(void);

/*!
** todo
*/
extern int distance(int x1, int y1, int x2, int y2);

/*!
** todo
*/
extern int route_possible(int x, int y);

/*!
** todo
*/
extern int construction_possible(int x, int y);

/*!
** todo
*/
extern int cout_achat_maison(void);

/*!
** todo
*/
extern int cout_achat_route(void);

/*!
** todo
*/
extern int mon_tour(void);

/*!
** todo
*/
extern int construire_route(int x, int y);

/*!
** todo
*/
extern int construire_maison(int x, int y);

/*!
** todo
*/
extern int reserver_case(int x, int y);

/*!
** todo
*/
extern int detruire_maison(int x, int y);

/*!
** todo
*/
extern int vendre_maison(int x, int y);

/*!
** todo
*/
extern int encherir(int montant);

/*!
** todo
*/
extern int construire_monument(int x, int y);

/*!
** Fonction appele au debut de la partie
*/
void init_game(void);

/*!
** Fonction appele a chaque tour 1
*/
void jouer(void);

/*!
** Fonction appele a chaque tour 2
*/
void enchere(void);

/*!
** Fonction appele a chaque tour 3
*/
void placement(void);

