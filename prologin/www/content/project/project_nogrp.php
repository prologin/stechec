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

include "include/project.php";

function disp_nogroupes($id_projet)
{
  global $db_link;
  global $_limit;
  global $user_profil;

  if ($id_projet <= 0)
    return projet_noselect();

  /* cache les informations au eleve pour les projets non publiques */
  if (access_check(ACL_ELEVE))
    {
      if (db_num_rows(db_query("SELECT id FROM projet WHERE id=".$id_projet.
			       " AND promo=".$user_profil["promo"].
			       " AND date_distrib<NOW() AND id_statut>1")) < 1)
	{
	  style_text("Ces informations ne sont pas disponibles.");
	  return;
	}
    }

  style_title("Individus non inscrit");

  /* description des donnees de la table */
  $list = array(
		array("nom",		"Nom",		"link_info", ACL_ALL, 200),
		array("login",		"Login",	"login", ACL_ALL, "@epita.fr"),
		array("etape",		"Etat",		"prj_stg_enum", ACL_ALL,
		      0, array("", "center_red", "center_yellow", "center_green")),
		array("statut",		"Presence",	"bool", ACL_ALL,
		      array("Non", "Oui"), array("center_red", "center")),
		);

  $count = db_fetch_row(db_query("SELECT COUNT(*) FROM prj_jnt ".
				 "WHERE prj_jnt.etape<".PRJ_STG_INSCRIT." ".
				 "AND   prj_jnt.id_projet=".$id_projet,
				 $db_link));

  /* requete sql des groupes du projet */
  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS nom, prj_jnt.etape, ".
		    "       user.login, user.id, user_prof.statut<2 AS statut, ".
		    "       user_prof.id AS pid ".
		    "FROM user, user_prof, prj_jnt ".
		    "WHERE user_prof.uid=user.id ".
		    "AND   prj_jnt.id_eleve=user_prof.id ".
		    "AND   prj_jnt.etape<".PRJ_STG_INSCRIT." ".
		    "AND   prj_jnt.id_projet=".$id_projet." ".
		    "ORDER BY user.nom ".
		    "LIMIT ".($_limit * 30).", 30",
		    $db_link);

  style_slice($_limit, 30, $count[0]/30);

  /* affichage des infos */
  list_disp($query, $list, array("bluegray", "gray"), "even");

  style_slice($_limit, 30, $count[0]/30);
}

/* affiche les infos de la page */
disp_nogroupes($_info_id);

?>

