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

function mlst_disp($this_id)
{
  global $db_link;
  global $user_profil;

  // get name

  $res = db_query("SELECT ".
		  "CONCAT('ml-',projet.nom,'-',projet.promo,'-',prj_jnt.id_groupe) AS name, ".
		  "projet.id,prj_jnt.id_groupe ".
 		  "FROM projet, prj_jnt, task " .
 		  "WHERE prj_jnt.id_projet=$this_id ".
		  "AND (projet.date_cloture<NOW()) ".
		  "AND prj_jnt.id_eleve='" . $user_profil["id"] .  "' " .
		  "AND projet.give_mlst=1 ".
		  "AND projet.mlst_id_task=task.id ".
		  "AND task.task_state_id=".TASK_ST_SUCCESS." ".
		  "ORDER BY id DESC", 
 		  $db_link);

  if (db_num_rows($res) < 1)
    return;
  
  $entry = db_fetch_row($res);
  style_title("Detail de la mailing-list ".$entry[0]);
  $project_grp = $entry[2];

  // get info

  $res = db_query("SELECT projet.id,prj_jnt.id_groupe, ".
		  "CONCAT('ml-',projet.nom,'-',projet.promo,'-',prj_jnt.id_groupe,'@lab-acu.epita.fr') AS address, ".
		  "CONCAT('/wws/arc/','ml-',projet.nom,'-',projet.promo,'-',prj_jnt.id_groupe,'/:SSL:') AS archives, ".
		  "projet.mlst_comment AS comment ".
 		  "FROM projet,prj_jnt " .
 		  "WHERE prj_jnt.id_projet=$this_id ".
		  "AND prj_jnt.id_eleve='" . $user_profil["id"] .  "' " .
		  "AND projet.give_mlst=1 ".
		  "ORDER BY id DESC", 
 		  $db_link);

  if (db_num_rows($res) < 1)
    return;

  list_disp($res, array(
			array("address", "Adresse",	"login", ACL_ALL, "" ),
			array("comment", "Description",	"text", ACL_ALL),
			array("archives", "Archives", "extern_link", ACL_ALL)),
	    array("gray"), "none");

  // get members
  
  $res = db_query("SELECT user.login,CONCAT(user.login,'@epita.fr') AS email, ".
		  "CONCAT(user.nom, ' ', user.prenom) AS nom, ".
		  "(prj_grp.id_chef=prj_jnt.id_eleve) AS isadmin ".
 		  "FROM prj_jnt,prj_grp,user,user_prof ".
 		  "WHERE prj_grp.id=$project_grp " .
		  "AND prj_jnt.id_groupe=$project_grp " .
		  "AND user_prof.id=prj_jnt.id_eleve ".
		  "AND user.id=user_prof.uid",
 		  $db_link);

  if (db_num_rows($res) < 1)
    return;

  list_disp($res, array(
			array("nom", "Membres", "text", ACL_ALL),
			array("email", "", "text", ACL_ALL),
			array("isadmin", "Administrateur", "bool", ACL_ALL, array("", "Administrateur")),
			array("login", "Email", "login", ACL_ALL)),
	    array("gray", "bluegray"), "isadmin");

  print "<br>";

}

function mlsts_disp()
{
  global $db_link;
  global $user_profil;

  $res = db_query("SELECT projet.id,prj_jnt.id_groupe, ".
		  "CONCAT('ml-',projet.nom,'-',projet.promo,'-',prj_jnt.id_groupe,'@lab-acu.epita.fr') AS address, ".
		  "CONCAT('/wws/arc/','ml-',projet.nom,'-',projet.promo,'-',prj_jnt.id_groupe,'/:SSL:') AS archives, ".
		  "projet.mlst_comment AS comment ".
 		  "FROM projet,prj_jnt,task " .
 		  "WHERE prj_jnt.id_projet=projet.id ".
		  "AND (projet.date_cloture<NOW()) ".
		  "AND prj_jnt.id_eleve='" . $user_profil["id"] .  "' " .
		  "AND projet.give_mlst=1 ".
		  "AND projet.mlst_id_task=task.id ".
		  "AND task.task_state_id=".TASK_ST_SUCCESS." ".
		  "ORDER BY id DESC", 
 		  $db_link);

  style_title("Liste de mes MLs");

  list_disp($res, array(
			array("address", "Adresse",	"login", ACL_ALL, "" ),
			array("comment", "Description",	"text", ACL_ALL),
			array("id", "Detail", "link", ACL_ALL, array("_id_page" => 44)),
			array("archives", "Archives", "extern_link", ACL_ALL)),

	    array("gray"), "none");
}

if ($_entry)
     mlst_disp($_entry);

     mlsts_disp();

?>
