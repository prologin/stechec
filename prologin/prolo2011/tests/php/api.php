<?php
/*
 * Taille du terrain
 */
define('TAILLE_TERRAIN', 100);

/*
 * Nombre de tours par partie
 */
define('FIN_PARTIE', 1500);

/*
 * Nombre de points d'action par tour
 */
define('MAX_PA', 3);

/*
 * Taille des traînées de moto
 */
define('TAILLE_TRAINEE', 900);

/*
 * Longueur maximale de l'allongement
 */
define('MAX_ALLONGEMENT', 5);

/*
 * Énumération représentant une erreur renvoyée par une des fonctions d'action
 */
define('OK', 0); // aucune erreur n'est survenue
define('ID_INVALIDE', 1); // identifiant invalide
define('POSITION_INVALIDE', 2); // la position spécifiée est invalide
define('PLUS_DE_PA', 3); // vous n'avez pas assez de points d'action
define('BONUS_INVALIDE', 4); // vous n'avez pas ce bonus
define('PAS_A_TOI', 5); // l'unité n'est pas a vous

/*
 * Énumération représentant les différents types de case
 */
define('VIDE', 0); // rien n'est présent sur la case
define('OBSTACLE', 1); // cette case est inaccessible
define('BONUS', 2); // cette case cotient un bonus
define('POINT_CROISEMENT', 3); // point de croisement de traînées

/*
 * Énumération représentant les différents types de bonii
 */
define('PAS_BONUS', 0); // ceci n'est pas un bonus :-)
define('BONUS_CROISEMENT', 1); // bonus permettant de croiser deux traînées de moto sur une case
define('PLUS_LONG', 2); // bonus permettant d'agrandir une traînée de moto
define('PLUS_PA', 3); // bonus permettant d'avoir plus de points d'action
define('BONUS_REGENERATION', 4); // bonus permettant de regenerer une source d'energie

?>
