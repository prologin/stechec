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

    Copyright ACU EPITA Promo 2004-2005

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>
        * Hesling Fabrice <fabecc@epita.fr>
        * Delecroix Nicolas <delecr_n@epita.fr>

*/

include "include/sondage.php";

function votes_disp($sid)
{
  global $_action;
  global $_entry;
  global $user;

  /* donnees sur le sondage */
  $res = db_query("SELECT createur, statut, @nc:=nb_choix, ".
		  "       titre, question ".
		  " FROM sondage ".
		  " WHERE sondage.id=".$sid);

  if (db_num_rows($res) < 1)
    return;

  $snd = db_fetch_row($res);

  /* nombre de votes laisses par l'utilisateur */
  $ucnt = db_fetch_row(db_query("SELECT @uc:=COUNT(*) FROM sondage_votes ".
			       " WHERE voter_id=".$user["id"]." ".
			       " AND   sondage_id=".$sid));

  /* droit de vote ? */
  // new delex : rajoute view_answers
  $acl_vote = ($snd[1] == SND_ST_ENCOUR || $snd[1] == SND_ST_ONLY_ASSIST) ? ACL_ALL : 0;

  switch ($_action)
    {
    case "flush":

      if (!access_check(ACL_ORGA))
	break;

      db_query("DELETE FROM sondage_votes WHERE prop_id=".$_entry);

      break;

    case "dec":

      if (!access_check(ACL_ORGA))
	break;

      db_query("DELETE FROM sondage_votes WHERE prop_id=".$_entry." LIMIT 1");

      break;

    case "inc":

      if (!access_check(ACL_ORGA))
	break;

      db_query("INSERT INTO sondage_votes (prop_id, voter_id, sondage_id) ".
	       " SELECT ".$_entry.", user.id, ".$sid." ".
	       " FROM user".
               " WHERE user.id_profil=1 ".
	       " ORDER BY RAND() LIMIT 1");

      break;

    case "unvote":

      db_query("DELETE FROM sondage_votes WHERE voter_id=".$user["id"]." ".
	       " AND   prop_id=".$_entry." LIMIT 1");
      db_query("DO @uc:=GREATEST(@uc-1, 0)");

      break;

    case "vote":

      /* check max vote count */
      if ($ucnt[0] >= $snd[3])
	break;

      db_query("INSERT INTO sondage_votes (prop_id, voter_id, sondage_id) ".
	       " VALUES (".$_entry.", ".$user["id"].", ".$sid.")");
      db_query("DO @uc:=@uc+1");

      break;
    }

  db_query("DO @i:=1, @last:=100000");

  /* nombre de votes de la proposition la plus haute et total de votes */
  db_query("SELECT @votes:=GREATEST(MAX(t.count), 1), @total:=SUM(t.count) ".
	   " FROM (SELECT COUNT(*) AS count FROM sondage_votes ".
	   " WHERE sondage_votes.sondage_id=".$sid." ".
	   " GROUP BY sondage_votes.prop_id) AS t");

  /* taux de participation */
  $part = db_fetch_row(db_query("SELECT TRUNCATE((@total*100)/(COUNT(*)*@nc), 2) ".
				" FROM user"));

  $res = db_query(
		  /* calcul du classement */

		  //  AND (t.answers_max != 0 OR votes < t.answers_max)

		  "SELECT IF(@last>FLOOR(t.votes), @rank:=@i, @rank) AS rank, t.uvotes, ".
		  " @i:=@i+1, @last:=FLOOR(t.votes) AS votes, IF(@uc<@nc AND (t.answers_max = 0 OR votes < t.answers_max) , t.id, '') AS vid, ".
		  " IF(t.uvotes, t.id, '') AS rid, t.id, t.proposition, t.answers_max AS ans_max, t.pvotes ".

		  " FROM (SELECT sondage_props.id, sondage_props.proposition, ".
		  /* nombre de votes */
		  " IF(sondage_votes.voter_id, COUNT(*), 0) AS votes, ".
		  /* delex */
		  " 42 AS tmp, ".
		  " sondage_props.answers_max AS answers_max, ".
		  /* % de votes par rapport au max */
		  " FLOOR(IF(sondage_votes.voter_id, COUNT(*), 0)*100/@votes)+1 AS pvotes, ".
		  /* nombre de vote personnels */
		  " IF(sondage_votes.voter_id, SUM(sondage_votes.voter_id=".$user["id"]."), 0) AS uvotes ".

		  " FROM sondage_props LEFT JOIN sondage_votes ".
		  " ON sondage_votes.prop_id=sondage_props.id ".
		  " WHERE sondage_props.sondage_id=".$sid." ".
		  " GROUP BY sondage_props.id ORDER BY votes DESC, proposition) AS t");


  style_title("Sondage: ".$snd[3]);

  style_text("<b>Question: </b>".$snd[4]."<br>");

  if ($snd[2] > 1)
    style_text("Vous avez droit a <b>".$snd[2]."</b> votes pour ce sondage.<br>");

  style_text("Le taux de participation a ce sondage est de <b>".$part[0]."%</b>.<p>");

  // on ne peut voir les resultats que si c'est un view_answer
  if ($snd[1] == SND_ST_VIEW_ANSWERS) {
    style_links(array(
		    array("Voir les resultats", ACL_ALL,
			  array("_id_page" => 244))
		    ));
  }




  $list = array(
		array("proposition",	"Proposition",	"text", ACL_ALL),
		array("votes",		"Votes",	"text", ACL_ALL,
		      0, array("center")),
		array("uvotes",		"Perso",	"text", ACL_ALL,
		      0, array("center")),
		array("vid",		"Voter",	"link", $acl_vote,
		      array("_action" => "vote"), array("center")),
		array("rid",		"Retirer",	"link", $acl_vote,
		      array("_action" => "unvote"), array("center")),
		array("id",		"Zero",		"link", ACL_SITEADM,
		      array("_action" => "flush"), array("center")),
		array("id",		"Moins",	"link", ACL_SITEADM,
		      array("_action" => "dec"), array("center")),
		array("id",		"Plus",		"link", ACL_SITEADM,
		      array("_action" => "inc"), array("center")),
		);

  list_disp($res, $list, array("gray", "bluegray"), "even");
}


votes_disp($_info_id);

?>

