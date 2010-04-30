<?php
/*
 * Taille de d�part du terrain.
 */
define('TAILLE_DEPART', 19);

/*
 * Le nombre maximal d'unit�s pouvant appartenir � une �quipe.
 */
define('NBR_MAX_UNITES', 10);

/*
 * Le temps, en nombre de tours, entre deux r�tr�cissement du terrain.
 */
define('TEMPS_RETRECISSEMENT', 5);

/*
 * Le nombre maximum en jeu de chaque carte.
 */
define('MAX_CARTES', 2);

/*
 * �num�ration repr�sentant une erreur renvoy�e par une des fonctions d'action.
 */
define('OK', 0); // aucune erreur n'est survenue
define('POSITION_INVALIDE', 1); // la position sp�cifi�e est invalide
define('CASE_OCCUPEE', 2); // la case sur laquelle vous tentez de vous d�placer est occup�e
define('PAS_A_PORTEE', 3); // la cible n'est pas � port�e
define('UNITE_KO', 4); // l'unit� que vous essayez de faire agir ou sur laquelle vous essayez d'agir  est KO
define('UNITE_DEBOUT', 5); // l'unit� que vous essayez de relever est d�j� debout
define('QUOTA_DEPASSE', 6); // nombre maximal d'unites, de spawn ou de relevages par tour d�pass�
define('PLUS_DE_PA', 7); // cette unit� a r�alis� toutes ses actions
define('DEJA_ATTAQUE', 8); // cette unit� a d�j� attaqu�
define('UNITE_INTERDITE', 9); // cette unit� ne peut pas �tre amen�e en renfort
define('RENFORT_IMPOSSIBLE', 10); // une unit� est d�j� pr�sente sur le spawn
define('PAS_A_MOI', 11); // l'unit� cibl�e n'est pas � vous
define('PLUS_DE_CARTES', 12); // il n'y a plus de cartes du type sp�cifi� dans votre main
define('PHASE_CARTES_TERMINEE', 13); // vous ne pouvez plus poser de cartes car vous avez fait une action

/*
 * Le type d'une unit� sur le terrain
 */
define('PERROQUET', 0); // 1 PA et port�e de 1
define('SINGE', 1); // 2 PA et port�e de 3
define('CHAT', 2); // 5 PA et port�e de 1
define('KANGOUROU', 3); // 3 PA et port�e de 2 (explose !)

?>
