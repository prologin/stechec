<?php
/*
 * This file has been generated, if you wish to
 * modify it in a permanent way, please refer
 * to the script file : gen/generator_php.rb
 */

require('api.php');

/*
 * Fonction appell�e au d�but de la partie.
 */
function init_game()
{
  /* Place ton code ici */
}

/*
 * Fonction appell�e pour la phase de retrait de KO.
 */
function retirer_ko()
{
  $a = unites();
  foreach ($u as $a){
    if ($u["ennemi"] && $u["ko"] > 0){
      return $u["pos"];
    }
  }
}

/*
 * Fonction appell�e pour la phase de jeu.
 */
function jouer()
{
  $u = unites();
  afficher_unite($u[0]);
}

/*
 * Fonction appell�e � la fin de la partie.
 */
function end_game()
{
  /* Place ton code ici */
}

?>
