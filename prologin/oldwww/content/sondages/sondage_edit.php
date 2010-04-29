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

function sondage_edit($sid)
{
  global $_action;
  global $_entry;
  global $user;

  $res = db_query("SELECT createur, statut, nb_choix, titre ".
		  " FROM sondage ".
		  " WHERE sondage.id=".$sid);

  if (db_num_rows($res) < 1)
    return;

  $row = db_fetch_row($res);

  style_title("Sondage ".$row[3]);

  // delex : nouveau type de sondage "Answers Visible" => les etudiants voient qui a repondu quoi
  $list = array(
		array("",		"Informations",		"title",
		      ACL_ALL, 0),
		array("titre",		"Titre",		"text",
		      ACL_ALL, ACL_ORGA),
		array("statut",		"Statut",		"bool",
		      ACL_NOT_ELEVE, ACL_ORGA,
		      array("Elaboration", "En cours", "Termine", "Invisible", "Only Orga", "Answers Visible")),
		array("question",	"Question",		"textarea",
		      ACL_ALL, ACL_ORGA),
		array("date",		"Date",			"datetime",
		      ACL_ALL, ACL_ORGA),
		array("",		"Votes",		"title",
		      ACL_ALL, 0),
		array("nb_choix",	"Votes par personne",	"text",
		      ACL_ALL, ACL_ORGA),
		array("total",		"Votes total",		"text_nodb",
		      ACL_ALL, 0),
		array("",		"Operations",		"title",
		      ACL_ORGA, 0),
		array("is_main",	"Afficher en 1ere page",	"checkbox",
		      ACL_ORGA, ACL_ORGA),
		array("Supprimer ce sondage", "Suppression",	"link",
		      ACL_ORGA, 0, array("_action" => "remove_sond", "_id_page" => 50),
		      "Voulez vous supprimer ce sondage ?"),
		array("Supprimer toutes les propositions", "Suppression",	"link",
		      ACL_ORGA, 0, array("_action" => "remove_all_props"),
		      "Voulez vous supprimer toutes les propositions ?"),
		array("Annuler tous les votes", "Annulation",	"link",
		      ACL_ORGA, 0, array("_action" => "flush_sond"),
		      "Voulez vous remettre a zero tous les votes ?"),
		);

  $listp = array(
		 array("",		"Proposition",		"title",
		       ACL_ALL, 0),
		 array("proposition", "Proposition", "text",
		       ACL_ALL, ACL_ORGA),
		 array("pvotes", "Total %", "percent",
		       ACL_ALL, 0),
		 array("votes", "Votes", "text",
		       ACL_ALL, 0),
		 array("Supprimer la proposition", "Suppression", "link",
		       ACL_ORGA, 0,
		       array("_action" => "remove_prop"),
		       "Voulez vous supprimer cette proposition ?"),
		 );

  switch ($_action)
    {
    case "edit_sond":

      if (!access_check(ACL_ORGA))
        break;

      table_update("sondage", $list, "id=".$sid);

      break;
    }

  $query = table_select("sondage", $list, "id=".$sid);

  if (db_num_rows($query) < 1)
    return;

  $data = db_fetch_array($query);

  switch ($_action)
    {
    case "flush_sond":

      if (!access_check(ACL_ORGA))
        break;

      db_query("DELETE FROM sondage_votes WHERE sondage_id=".$sid, $db_link);

      break;

    case "new_prop":

      if (!access_check(ACL_ORGA))
        break;

      table_insert("sondage_props", array(), array("sondage_id" => $sid));

      break;


    case "remove_prop":

      if (!access_check(ACL_ORGA))
        break;

      db_query("DELETE FROM sondage_props WHERE id=".$_entry, $db_link);

      break;

    case "remove_all_props":

      if (!access_check(ACL_ORGA))
        break;

      if ($_entry < 1)
	break;

      db_query(" DELETE FROM sondage_votes WHERE sondage_id=".$_entry, $db_link);
      db_query(" DELETE FROM sondage_props WHERE sondage_id=".$_entry, $db_link);

      break;

    case "edit_prop":

      if (!access_check(ACL_ORGA))
        break;

      table_update("sondage_props", $listp, "id=".$_entry);

      break;

    }

  $votes_cnt = db_fetch_row(db_query("SELECT @votes:=GREATEST(COUNT(*), 1) AS count ".
				     " FROM sondage_votes ".
				     " WHERE sondage_votes.sondage_id=".$sid));

  $data["total"] = $votes_cnt[0];

  table_disp_one($data, $list, "edit_sond", "_entry");

  style_title("Propositions");

  style_links(array(
		    array("Nouvelle proposition", ACL_ORGA,
			  array("_action" => "new_prop"))
		    ));

  $res = db_query("SELECT sondage_props.id, sondage_props.proposition, ".
		  " IF(sondage_votes.voter_id, COUNT(*), 0) AS votes, ".
		  " FLOOR(IF(sondage_votes.voter_id, COUNT(*), 0)*100/@votes)+1 AS pvotes ".
		  " FROM sondage_props LEFT JOIN sondage_votes ".
		  " ON sondage_votes.prop_id=sondage_props.id ".
		  " WHERE sondage_props.sondage_id=".$sid." ".
		  " GROUP BY sondage_props.id ORDER BY proposition");

  table_disp($res, $listp, "edit_prop", "_entry");
}

sondage_edit($_info_id);

?>

