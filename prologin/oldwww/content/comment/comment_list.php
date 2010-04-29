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

function disp_comment_list()
{
  global $profil;
  global $user;
  global $_limit;


  $count = db_fetch_row(db_query("SELECT COUNT(*) FROM user, cmt".
				 " WHERE user.id=cmt.id_target".
				 " AND cmt.visibility<=".$profil["id"]));

  style_title("Historique des commentaires");

  style_slice($_limit, 10, $count[0]/60);

  $profil_color = array("", "", "red", "red", "red",
			"red2", "red2", "red2", "red3");

  $list = array(
		array("date",		"Date",			"date", ACL_ALL,
		      0, array("center")),
		array("mark",		"Tendance",		"bool", ACL_ALL,
		      array("Negatif", "Neutre", "Positif"),
		      array("center_red", "center", "center_green")),
		array("auteur",		"Auteur",		"text", ACL_ALL, 200),
		array("target",		"Concernant",		"link_info", ACL_ALL, 200),
		array("cid",		"Lire",			"link",	ACL_ALL,
		      array("_id_page" => 202), array("center"), "_info_id"),
		);

  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS auteur, ".
		    "       user.id_profil AS auteurp, userb.id, ".
		    "       CONCAT(userb.nom, ' ', userb.prenom) AS target, cmt.id_target AS cid, ".
		    "       userb.id_profil AS targetp, ".
		    "       ".db_date("cmt.date", "date").", cmt.mark, ".
		    "       cmt.id_target=".$user["id_profil"].
		    "       || cmt.id_auteur=".$user["id_profil"]." AS concerned ".
		    " FROM user, cmt, user AS userb ".
    	            " WHERE user.id=cmt.id_auteur ".
		    " AND   userb.id=cmt.id_target ".
		    " AND   cmt.visibility<=".$profil["id"]." ".
		    " ORDER BY cmt.date DESC ".
		    " LIMIT ".($_limit * 60).", 60");

  list_disp($query, $list, array("gray", "bluegray"), "concerned");

  style_slice($_limit, 10, $count[0]/60);
}

disp_comment_list();

?>

