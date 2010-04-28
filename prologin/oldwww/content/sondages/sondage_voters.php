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
?>
<?

include(dirname(__FILE__) . "/sondage_misc.php");

function sondages_disp()
{
  global $db_link;
  global $user_profil;

  $promo = $user_profil[promo];
  $res = db_query("select `id`, `titre`, `question` from `" . SOND_TABLE .
		  "` where `promo`='$promo' order by `date` desc", $db_link);

  list_disp($res, array(
			array("titre", "Titre", "text", ACL_ALL),
			array("question", "Question", "text", ACL_ALL, 50),
			array("titre", "Editer participants", "link_info", ACL_ASSIST, 52),
			), array("gray"), "none");
}

function voters_disp($sid)
{
  global $_limit;
  global $db_link;

  if (intval($sid) <= 0)
    return;

  style_title("Liste des participants");


  $list = array(
                array("id",		"UId",          "text", ACL_ALL,),
                array("nom",            "Nom",          "text", ACL_ALL),
                array("prenom",         "Prenom",       "text", ACL_ALL),
                array("login",          "supprimer",	"link", ACL_ALL),
		);

  $query = db_query("SELECT user.nom, user.prenom, user.login, user.id ".
		    "FROM user, user_prof, ".SOND_VOTES." ".
                    "WHERE ".SOND_VOTES.".id=user_prof.id ".
		    "AND ".SOND_VOTES.".sondage=$sid ".
		    "AND user_prof.uid=user.id ".
                    "ORDER BY user.login ".
                    "LIMIT ".($_limit * 30).", 30",
                    $db_link);
  
  style_slice($_limit, 15);
  
  /* affichage des infos */
  list_disp($query, $list, array("gray"), "none");
  
  style_slice($_limit, 15);
}


voters_disp($_info_id);
style_title("Les sondages");
sondages_disp($_info_id);

?>
