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

function sondages_disp()
{
  global $user;
  global $_action;
  global $_entry;

  style_title("Liste des sondages");

  switch ($_action)
    {
    case "new_sond":

      if (!access_check(ACL_ORGA))
        break;

      table_insert("sondage", array(), array("date" => "NOW()",
					     "createur" => $user["id"],
					     "titre" => "'Nouveau sondage'"));
      break;

    case "remove_sond":

      if (!access_check(ACL_SITEADM | ACL_ASSIST_ALL))
        break;

      if ($_entry < 1)
	break;

      db_query("DELETE FROM sondage_votes WHERE sondage_id=".$_entry, $db_link);
      db_query("DELETE FROM sondage_props WHERE sondage_id=".$_entry, $db_link);
      db_query("DELETE FROM sondage WHERE id=".$_entry, $db_link);

      break;
    }

  // new delex : le statut peut aussi etre "view answers"
  if (!access_check(ACL_SITEADM | ACL_ASSIST_ALL))
    $chk_statut =
      " AND (sondage.statut=".SND_ST_ENCOUR.
      "     OR sondage.statut=".SND_ST_TERMIN.") ";

  $res = db_query("SELECT sondage.id, sondage.titre, sondage.statut, ".db_time("sondage.date", "date").", ".
		  "       CONCAT(user.nom, ' ', user.prenom) AS auteur ".
		  " FROM sondage, user ".
                  " WHERE user.id=sondage.createur".
	      	  $chk_statut.
		  " ORDER BY sondage.date DESC", $db_link);

  style_links(array(
		    array("Nouveau sondage", ACL_ORGA, array("_action" => "new_sond"))
		    ));

  list_disp($res, array(
			array("titre",	"Titre",	"link_info",	ACL_ALL, 53),
			array("date",	"Date",		"datetime",	ACL_ALL,
			      0, array("center")),
			array("auteur",	"Auteur",	"text",		ACL_ALL),
			array("statut",	"Statut",	"bool",	ACL_ALL,
			      array("Elaboration", "En cours", "Termine", "Invisible", "Only Orga", "Answers visible"),
			      array("center_red", "center_green", "center_yellow", "center_yellow", "center_green", "center_green")),
			array("id",	"Editer",	"link",		ACL_ORGA,
			      array("_id_page" => 51), array("center"), "_info_id"),
			array("id",	"Details",	"link",		ACL_ORGA,
			      array("_id_page" => 52), array("center"), "_info_id"),
			),
	    array("gray"), "none");

}

sondages_disp();

?>
