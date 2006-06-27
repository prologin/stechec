<?php
/*

    ACU Intranet project is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ACU Intranet project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ACU Intranet project; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Copyright ACU EPITA Promo 2004

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>

*/

include "include/game.php";

function disp_new_game()
{
  global $user;
  global $post_data;

  $post_data["_id_page"] = "30";

  /* donnee par defaut */
  $empty = array("nom" => "Sans nom",
                 "comment" => "Sans description",
                 "lib_name" => "Sans lib",
                 "repertoire" => "Sans repertoire",
                 "max_champion" => "1",
		 "arg_server" => "--jeu <name>",
		 );

  style_title("Nouveau jeu");

  /* affiche la table de cretaion de projets */
  table_disp_one($empty, game_new_template(), "new_game", "_entry");
}

disp_new_game();

?>

