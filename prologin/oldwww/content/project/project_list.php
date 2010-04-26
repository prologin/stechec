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

<?php

include "include/project.php";

function projects_action($promo)
{
  global $db_link;
  global $user_profil;
  global $_action;

  /* effectue l'action requise */
  switch ($_action)
    {
      /*
       * creation d'un nouveau projet
       */
    case ("new_project"):

      /* insertion de la nouvelle entree dans la table */
      if (!table_insert("projet", project_new_template(),
			array("promo" => $promo)))
	break;

      /* ajoute le createur du projet comme responsable */
      db_query("INSERT INTO prj_resp (id_profil, id_projet) ".
	       "VALUES ('".$user_profil["id"]."', LAST_INSERT_ID())", $db_link);

      break;
    }
}

function projects_disp($promo)
{
  global $db_link;
  global $user_profil;

  $and_pub = "";
  if (    $user_profil["id_profil"] == 9) {
    $only_pub = "AND id_statut > 1 ";
  }

  $res = db_query("SELECT id, id_type, nom, id_statut, with_upload, ".
		  "(date_rendu<NOW()) AS rendu, ".
		  "(date_distrib<NOW()) AS distrib, ".
		  "(date_cloture<NOW()) AS cloture, ".
		  db_date("date_distrib", "date_distrib").", ".
		  db_date("date_rendu", "date_rendu")." ".
		  "FROM projet ".
		  "WHERE promo=".$promo." ".
		  $only_pub .
		  "ORDER BY projet.date_rendu DESC, id_type",
		  $db_link);

  style_title("Les projets ".$promo);

  list_disp($res, array(
			array("date_distrib",	"Date pub.",	"date", ACL_ALL,
			      0, array("center")),
			array("date_rendu",	"Date rendu",	"date", ACL_ALL,
			      0, array("center")),
			array("nom",		"Nom",		"link_info", ACL_ALL, "32"),
			array("id_type",	"Type",		"prj_typ_enum", ACL_ALL,
			      0, array("center")),
			array("id_statut",	"Etat",	"bool", ACL_ALL,
			      array("", "Elabor.", "Valide"), array("center_green", "center_red")),
			array("distrib",	"Publication","bool", ACL_ALL,
			      array("Non", "Publie"), array("center_red", "center_green")),
			array("cloture",	"Inscriptions","bool", ACL_ALL,
			      array("Ouvert", "Ferme"), array("center_red", "center_green")),
			array("with_upload",	"Upload","bool", ACL_ALL,
			      array("Non", "Oui"), array("center_red", "center_green")),
			array("rendu",		"Rendu","bool", ACL_ALL,
			      array("Non", "Rendu"), array("center_red", "center_green")),
			), array("gray"), "none");
}


{
  $promo = promo_list_disp();

  if ($promo > 0)
  {
    projects_action($promo);
    projects_disp($promo);
  }
}

?>
