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

include "include/sondage.php";


function sondage_disp($sid)
{
  global $db_link;

  style_title("Le detail des votes");

  $res = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS auteur, ".
		  "user.id, sondage_props.proposition, COUNT(*) as count, ".
		  "LEAST(COUNT(*), 4) AS color ".
		  "FROM  sondage_votes, sondage_props, user ".
		  "WHERE sondage_votes.sondage_id=".$sid." ".
		  "AND   sondage_votes.voter_id=user.id ".
		  "AND   sondage_votes.prop_id=sondage_props.id ".
		  "GROUP BY sondage_votes.voter_id, sondage_votes.prop_id ".
		  "ORDER BY sondage_props.proposition, user.nom", $db_link);

  $list = array(
		array("proposition",	"Vote",		"text", ACL_ALL,
		      0, array("center")),
		array("count",		"Nombre",	"text", ACL_ALL,
		      0, array("center")),
		array("auteur",		"Individu",	"link_info", ACL_ALL, 200),
		);

  list_disp($res, $list, array("gray", "gray", "red", "red2", "red3"), "color");
}

sondage_disp($_info_id);

?>

