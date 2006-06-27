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

function disp_new_comment($id_profil)
{
  global $user;
  global $profil;
  global $post_data;

  $post_data["_id_page"] = "202";

  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom, ".
		    " ' (', profil.libelle, ')') AS name, profil.id ".
		    " FROM user, profil".
		    " WHERE user.id_profil=profil.id ".
		    " AND user.id=".$id_profil);

  if (!$row = db_fetch_array($query))
    return;

/*
  $query = db_query("SELECT * FROM cmt_acl ".
		    "WHERE prof_auteur=".$profil["id"]." ".
		    "AND   prof_target=".$row["id"],
		    $db_link);

  if (($user_profil["id"] == $id_profil)
      || (db_num_rows($query) < 1))
    {
      style_text("Vous n'etes pas authorise a laisser un commentaire sur cet utilisateur");
      return;
    }
*/

  $empty = array(
		 "name" => $row["name"],
		 "text" => "Aucun commentaire",
		 "visibility" => 2,//$profil["id"],
		 );

  $list = array(
		array("",		"Nouveau commentaire",	"title", ACL_ALL, 0),
		array("name",		"A propos de",		"text", ACL_ALL, 0),
		array("visibility",	"Seuil de visibilite",	"profil", ACL_ALL, ACL_ALL),
		array("text",		"Commentaire",		"textarea_big", ACL_ALL, ACL_ALL),
		array("mark",		"Tendance",		"bool", ACL_ALL, ACL_ALL,
		      array("Negatif", "Neutre", "Positif")),
		);

  style_title("Laisser un commentaire");


  /* affiche la table de cretaion de projets */
  table_disp_one($empty, $list, "new_comment", "_entry");
}

disp_new_comment($_info_id);

?>

