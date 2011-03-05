<?php
/*
 * Taille de départ du terrain.
 */
define('TAILLE_DEPART', 19);

/*
 * Le nombre maximal d'unités pouvant appartenir à une équipe.
 */
define('NBR_MAX_UNITES', 10);

/*
 * Le temps, en nombre de tours, entre deux rétrécissement du terrain.
 */
define('TEMPS_RETRECISSEMENT', 5);

/*
 * Le nombre maximum en jeu de chaque carte.
 */
define('MAX_CARTES', 2);

/*
 * Énumération représentant une erreur renvoyée par une des fonctions d'action.
 */
define('OK', 0); // aucune erreur n'est survenue
define('POSITION_INVALIDE', 1); // la position spécifiée est invalide
define('CASE_OCCUPEE', 2); // la case sur laquelle vous tentez de vous déplacer est occupée
define('PAS_A_PORTEE', 3); // la cible n'est pas à portée
define('UNITE_KO', 4); // l'unité que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
define('UNITE_DEBOUT', 5); // l'unité que vous essayez de relever est déjà debout
define('QUOTA_DEPASSE', 6); // nombre maximal d'unites, de spawn ou de relevages par tour dépassé
define('PLUS_DE_PA', 7); // cette unité a réalisé toutes ses actions
define('DEJA_ATTAQUE', 8); // cette unité a déjà attaqué
define('UNITE_INTERDITE', 9); // cette unité ne peut pas être amenée en renfort
define('RENFORT_IMPOSSIBLE', 10); // une unité est déjà présente sur le spawn
define('PAS_A_MOI', 11); // l'unité ciblée n'est pas à vous
define('PLUS_DE_CARTES', 12); // il n'y a plus de cartes du type spécifié dans votre main
define('PHASE_CARTES_TERMINEE', 13); // vous ne pouvez plus poser de cartes car vous avez fait une action

/*
 * Le type d'une unité sur le terrain
 */
define('PERROQUET', 0); // 1 PA et portée de 1
define('SINGE', 1); // 2 PA et portée de 3
define('CHAT', 2); // 5 PA et portée de 1
define('KANGOUROU', 3); // 3 PA et portée de 2 (explose !)

?>
