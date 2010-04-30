<?php
/*
 * This file has been generated, if you wish to
 * modify it in a permanent way, please refer
 * to the script file : gen/generator_php.rb
 */

require('api.php');

/*
 * Fonction appellée au début de la partie.
 */
function init_game()
{
  /* Place ton code ici */
}

/*
 * Fonction appellée pour la phase de retrait de KO.
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
 * Fonction appellée pour la phase de jeu.
 */
function jouer()
{
  afficher_unite(unites()[0]);
}

/*
 * Fonction appellée à la fin de la partie.
 */
function end_game()
{
  /* Place ton code ici */
}

?>
