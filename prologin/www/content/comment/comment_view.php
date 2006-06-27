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

function comment_template($acl_edit)
{
  return array(
	       array("",		"Commentaire",		"title", ACL_ALL, 0),
	       array("name",		"Auteur",		"text", ACL_ALL, 0),
	       array("date",		"Date",			"now", ACL_ALL, ACL_SITEADM),
	       array("visibility",	"Seuil de visibilite",	"profil", ACL_ALL, $acl_edit),
	       array("text",		"Commentaire",		"textarea_big", ACL_ALL, $acl_edit),
	       array("mark",		"Tendance",		"bool", ACL_ALL, $acl_edit,
		     array("Negatif", "Neutre", "Positif")),
	       array("Supprimer ce commentaire", "Suppression", "link",
		     ACL_SITEADM, 0, array("_action" => "del_comment"),
		     "Voulez vous supprimer ce commentaire ?"),
	       );
}

function disp_comment($id_profil, $id_cmt)
{
  global $user;
  global $profil;
  global $_action;
  global $_POST;

  /* donnee par defaut */
  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS name, profil.libelle, profil.id ".
		    " FROM user, profil ".
		    " WHERE user.id_profil=profil.id ".
		    " AND user.id=".$id_profil);

  if (!$target = db_fetch_array($query))
    return;

  switch ($_action)
    {
    case ("edit_comment"):

      if (!access_check(ACL_NOT_ELEVE))
	break;

      table_update("cmt", comment_template(ACL_NOT_ELEVE), "id=".$id_cmt);

      break;

    case ("del_comment"):
      if (!access_check(ACL_SITEADM))
	break;

      db_query("DELETE FROM cmt WHERE cmt.id=".$id_cmt);

      break;

    case ("new_comment"):
      /* impossible de laisser un commentaire sur soi meme */

      if ($user_profil["id"] == $id_profil)
	break;

      /*
      $query = db_query("SELECT * FROM cmt_acl ".
			"WHERE prof_auteur=".$profil["id"]." ".
			"AND   prof_target=".$target["id"],
			$db_link);

      if (db_num_rows($query) < 1)
	break;
      */

      $tmp = array(
		   array("visibility",	"",	"profil", ACL_ALL, ACL_ALL),
		   array("date",	"",	"now", ACL_ALL, ACL_ALL),
		   array("text",	"",	"textarea_big", ACL_ALL, ACL_ALL),
		   array("mark",	"",	"bool", ACL_ALL, ACL_ALL),
		   );

      table_insert("cmt", $tmp,
		   array("id_auteur" => $user["id_profil"],
			 "id_target" => $id_profil));

      print(mysql_error());
      break;
    }

  style_links(array(
		    array("Ajouter un commentaire", ACL_ALL,
			  array("_id_page" => 201))
		    ));

  style_title("Commentaires sur ".$target["name"]." (".$target["libelle"].")");

  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom, ' (', profil.libelle, ')') AS name, cmt.id_auteur, ".
		    " cmt.visibility, cmt.text, cmt.mark, ".db_time("cmt.date", "date").", cmt.id ".
		    " FROM user, cmt, profil ".
		    " WHERE user.id_profil=profil.id ".
		    " AND   user.id=cmt.id_auteur ".
		    " AND   cmt.id_target=".$id_profil." ".
		    " AND   cmt.visibility<=".$profil["id"]." ".
		    " ORDER BY cmt.date DESC");

  if (!($data = db_fetch_array($query)))
    {
      style_text("Aucun commentaire visible");
      return;
    }

  do
    {
      $list = comment_template($data["id_auteur"] == $user_profil["id"]
			       ? ACL_ALL : ACL_SITEADM);

      table_disp_one($data, $list, "edit_comment", "_entry");
    }
  while ($data = db_fetch_array($query));

}

disp_comment($_info_id, $_entry);

?>

