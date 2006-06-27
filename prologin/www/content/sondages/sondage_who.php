<?php


include "include/sondage.php";


function sondage_disp($sid)
{
  global $db_link;

  // verifier que le sondage est bien de type VIEW_ANSWERS
  $res = db_query("SELECT statut FROM sondage WHERE id=".$sid);
  if (db_num_rows($res) != 1)
    return;
  $my_tab = db_fetch_array($res);
  if ($my_tab[statut] != SND_ST_VIEW_ANSWERS)
    return;



  style_title("Le detail des votes");

  $res = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS auteur, ".
		  "user.id, sondage_props.proposition, COUNT(*) as count, ".
		  "LEAST(COUNT(*), 4) AS color ".
		  "FROM  sondage_votes, sondage_props, user, user_prof ".
		  "WHERE sondage_votes.sondage_id=".$sid." ".
		  "AND   sondage_votes.voter_id=user_prof.id ".
		  "AND   user.id=user_prof.uid ".
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

