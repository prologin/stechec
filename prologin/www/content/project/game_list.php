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

function game_action()
{
  global $_action;

  /* effectue l'action requise */
  switch ($_action)
    {
      // creation d'un nouveau projet
      case ("new_game"):
        // insertion de la nouvelle entree dans la table
        if (!table_insert("game", game_new_template(), array()))
  	  break;
        break;

    }
}

function game_disp()
{
  $res = db_query("SELECT id, nom, comment".
		  " FROM game ".
		  " ORDER BY nom");

  style_title("Les jeux ouverts");

  list_disp($res, array(
			array("nom", "Nom", "link_info", ACL_ALL, "32"),
			array("comment", "Description", "text", ACL_ALL, "center")
			), 
            array("gray"), "none");
                        
}



game_action();
game_disp();


?>
