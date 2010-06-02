<?php
/*
 * Taille du terrain.
 */
define('TAILLE_TERRAIN', 21);

/*
 * le prix de la fin d'une partie
 */
define('PRIX_FIN_PARTIE', 25);

/*
 * le nombre de points d'actions des unites au debut du tour.
 */
define('MAX_PA', 3);

/*
 * Le nombre d'unités par équipe.
 */
define('NBR_UNITES', 9);

/*
 * Le nombre maximum de tours.
 */
define('MAX_TURN', 100);

/*
 * Énumération représentant une erreur renvoyée par une des fonctions d'action.
 */
define('OK', 0); // aucune erreur n'est survenue
define('POSITION_INVALIDE', 1); // la position spécifiée est invalide
define('PLUS_DE_PA', 2); // vous n'avez pas assez de points d'actions
define('PAS_A_TOI', 3); // l'unite n'est pas a toi.
define('UTILISATION_IMPOSSIBLE', 4); // vous ne pouvez pas utiliser cet objet
define('PLUS_D_ARGENT', 5); // vous n'avez pas assez d'argent pour acheter l'objet en question

/*
 * Le type d'un objet
 */
define('FILET', 0); // permet d'attraper des pieces ou d'immobiliser des unités
define('MARTEAU', 1); // permet de frapper des unités.
define('RIEN', 2); // l'unite ne porte aucun objet

?>
