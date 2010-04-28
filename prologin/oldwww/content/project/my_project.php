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

function my_projects_disp()
{
  global $db_link;
  global $user_profil;

  $res = db_query("SELECT projet.id, projet.id_type, projet.nom, with_upload, ".
		  db_time("projet.date_rendu", "date_rendu").", prj_jnt.etape, ".
		  "(projet.date_cloture>NOW()) AS inscrit, ".
		  "(projet.date_cloture<NOW() && prj_jnt.etape<3) AS aborted ".
		  "FROM projet, prj_jnt ".
		  "WHERE prj_jnt.id_eleve=".$user_profil["id"]." ".
		  "AND   prj_jnt.id_projet=projet.id ".
		  "AND   projet.date_distrib<=NOW() ".
		  "AND   projet.id_statut>1 ".
		  "ORDER BY projet.date_distrib DESC"
		  , $db_link);

  style_title("Mes projets");

  list_disp($res, array(
			array("nom",		"Nom",		"link_info", ACL_ALL, "32"),
			array("id_type",	"Type",		"prj_typ_enum", ACL_ALL,
			      0, array("center")),
			array("inscrit",	"Inscriptions",	"bool", ACL_ALL,
			      array("Ferme", "Ouvert"),
			      array("center_red", "center_green")),
			array("date_rendu",	"Rendu",	"datetime", ACL_ALL,
			      0, array("center")),
			array("with_upload", "Upload", "bool", ACL_ALL,
			      array("Non", "Oui"), array("center_red", "center_green")),
			array("etape",		"Etat",		"prj_stg_enum", ACL_ALL,
			      0, array("", "center_red", "center_yellow", "center_green")),
			), array("gray", "red"), "aborted");
}

my_projects_disp();

?>

