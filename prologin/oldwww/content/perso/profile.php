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

if (access_check(ACL_NOT_ELEVE))
{
  include "include/download.php";
}

function disp_user_info($id_profil)
{
  global $user;

  style_title("Informations utilisateur");

  $list = array(
		array("",		"Identit&eacute;",	"title", ACL_ALL, 0),
		array("photo",		"Photo",		"photo", ACL_ALL, 0),
		array("photo",		"Photo",		"photo", ACL_ORGA, ACL_ORGA),
		array("login",		"Login",		"text", ACL_ALL, 0),
		array("prenom",		"Prenom",		"text", ACL_ALL, ACL_ORGA),
		array("nom",		"Nom",			"text", ACL_ALL, ACL_ORGA),
		array("nickname",	"Nickname",		"text", ACL_ALL, ACL_ORGA),
		array("email",		"E-mail",		"text", ACL_ORGA, ACL_ORGA),
		array("",		"Donn&eacute;es personnelles",	"title", ACL_ORGA, 0),
		array("date_naiss",	"Date de naissance",	"date", ACL_ORGA, ACL_ORGA),
		array("sexe",		"Sexe",			"bool", ACL_ORGA, ACL_ORGA,
		      array("Masculin", "Feminin")),
		array("adresse",	"Adresse",		"text", ACL_ORGA, ACL_ORGA),
		array("codep",		"Code postal",		"text", ACL_ORGA, ACL_ORGA),
		array("ville",		"Ville",		"text", ACL_ORGA, ACL_ORGA),
		array("phone",		"Telephone",		"text", ACL_ORGA, ACL_ORGA),
		array("gsm",		"GSM",			"text", ACL_ORGA, ACL_ORGA),
		);

  /* exec db from list query */
  $query = table_select("user", $list, "id=".$id_profil);

  if (db_num_rows($query) < 1)
    {
      print("<div class=text>Aucune donnee disponible</div>");
      return;
    }

  $data = db_fetch_array($query);


  /* set field ACLs according to data visibility */
 // if ($data["visibility"] <= $user_profil["id_profil"])
 //   for ($i = 5; $i <= 12; $i++)
 //     $list[$i][3] = ACL_ALL;

  /* display table */
  table_disp_one($data, $list, "edit_user", "_entry");

  if ($id_profil != $user["id"])
    db_query("UPDATE user SET visit_count=visit_count+1 WHERE id=".$data["id"], $db_link);
}

function disp_comments($id_profil)
{
  global $profil;

  $list = array(
		array("date",		"Date",			"date", ACL_ALL,
		      0, array("center")),
		array("auteur",		"Auteur",		"text", ACL_ALL),
		array("auteurp",	"",			"profil", ACL_ALL,
		      0, $profil_color),
		array("mark",		"Tendance",		"bool", ACL_ALL,
		      array("Negatif", "Neutre", "Positif"),
		      array("center_red", "center", "center_green")),
		array("id",		"Detail",		"link", ACL_ALL,
		      array("_id_page" => 202), array("center"), "_info_id"),
		);

  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS auteur, ".
		    "       user_prof.id_profil AS auteurp, ".
		    "       ".db_date("cmt.date", "date").", ".
		    "       cmt.id_target AS id, cmt.mark ".
		    "FROM user, user_prof, cmt ".
		    "WHERE user.id=user_prof.uid ".
		    "AND   user_prof.id=cmt.id_auteur ".
		    "AND   cmt.id_target=".$id_profil." ".
		    "AND   cmt.visibility<=".$profil["id"]." ".
		    "ORDER BY cmt.date DESC ",
		    $db_link);

  list_disp($query, $list, array("gray", "bluegray"), "even");
}

function disp_absents($id_profil)
{
  global $profil;

  $list = array(
		array("name",		"Evenement",		"link_info", ACL_ALL, 151),
		array("date",		"Date",			"datetime",	ACL_ALL,
		      0, array("center")),
		array("is_oblig",	"Obligation",		"bool", ACL_ALL,
		      array("Facultatif", "Obligatoire"), array("center_green", "center_red")),
		array("id",		"Liste promo",	"link", ACL_ALL,
		      array("_id_page" => 17), array("center"), "_info_id"),
		);

  $query = db_query("SELECT events.name, events.date, events.id, events.is_oblig ".
		    "FROM events, events_abs ".
		    "WHERE events_abs.id_evnt=events.id ".
		    "AND   events_abs.id_user=".$id_profil." ".
		    "ORDER BY events.date",
		    $db_link);

  list_disp($query, $list, array("gray", "red3"), "is_oblig");
}


function disp_traces($id_profil)
{
  global $profil;
  global $user_profil;

  $res = db_query("SELECT projet.nom AS nom, projet.id AS proj_id, ".
		  "       user.login AS login, user.id AS uid ".
		  "FROM   projet, user, user_prof, prj_jnt ".
		  "WHERE user_prof.uid=user.id ".
		  "AND   user_prof.id=".$id_profil." ".
		  "AND   prj_jnt.id_eleve=user_prof.id ".
		  "AND   prj_jnt.id_projet=projet.id ".
#
# On peut voir les traces mes avant la fin du proj
#		  "AND   projet.date_distrib<=NOW() ".
#		  "AND   projet.date_rendu<=NOW() ".
		  "AND   projet.id_statut>1 ".
		  "ORDER BY projet.date_distrib DESC"
		  , $db_link);

  $list = array(
		array("name", "Projet", "text", ACL_ALL, 0, array("center")),
		array("num", "Numero", "text", ACL_ALL, 0, array("center")),
		array("desc", "Description", "text", ACL_ALL),
		array("id", "Visualiser", "link", ACL_ALL,
                  array("_id_page" => 666), array("center"), "_entry", "_info_id"),
		);

  $table = array();

  $color = 0;
  while ($data = db_fetch_array($res))
    {
      $path = PRJ_DATA_PATH.$data["proj_id"]."/traces";

      $data["name"] = "<b>".$data["nom"]."</b>";

      $data["color"] = $color++;
      for ($i = 1; is_dir($path."/t".$i); $i++)
	{
	  if (!is_file($path."/t".$i."/".$data["login"].".trace"))
	    continue;

          /* trace must be visible for the user */
          $res_trace = db_query("SELECT visibility FROM trace_list ".
                                " WHERE id_proj=".$data["proj_id"].
                                " AND num_trace=".$i);
          if (db_num_rows($res_trace) < 1)
	    continue;

          $row_res_trace = db_fetch_row($res_trace);
          if ($row_res_trace[0] > $user_profil["id_profil"])
	    continue;

	  if (is_file($path."/t".$i."/desc"))
	    $data["desc"] = file_get_contents_($path."/t".$i."/desc");
	  else
	    $data["desc"] = "Aucune description";

	  $data["id"] = $data["uid"] * 1000 + $i;
	  $data["id2"] = $data["proj_id"];

	  $data["num"] = $i;

	  array_push($table, $data);

	  $data["name"] = "";
	}
    }

  list_disp_data($table, $list, array("gray", "bluegray"), "color");
}


function disp_access($uid)
{
  global $db_link;
  global $user;

  style_title("Logs de connections");

  $list_note = array(
		    array("log_date",		"Date",		"text", ACL_ALL,
			  0, array("center")),
		    array("period",		"Duree",	"text", ACL_ALL,
			  0, array("center")),
		    array("log_host",		"Host",		"text", ACL_ALL),
		    array("valid",		"Deconnection",	"bool", ACL_ALL,
			  array("Logout", ""), array("center")),
		    );

  $q1 = db_query("SELECT ".db_time("log_date", "log_date").", CONCAT(FLOOR(log_period/60), ".
		 "':', log_period%60) AS period, log_host, valid ".
		 "FROM   session ".
		 "WHERE  session.uid=".$uid." ".
		 "ORDER BY UNIX_TIMESTAMP(log_date) DESC LIMIT 50",
		 $db_link);

  list_disp($q1, $list_note, array("gray", "bluegray"), "even");
}



disp_user_info($_info_id);

if (access_check(ACL_ORGA))
     disp_access($_info_id);


?>

