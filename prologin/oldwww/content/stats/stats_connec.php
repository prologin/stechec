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

function disp_data()
{
  global $db_link;
  global $profil;
  global $user_profil;

  style_title("Connexions par mois");

  $mlist = array("Janvier", "Fevrier", "Mars", "Avril",
		 "Mai", "Juin", "Juillet", "Aout",
		 "Septembre", "Octobre", "Novembre", "Decembre");

  $q = db_query("SELECT COUNT(*), SUM(log_period), ".
		"FLOOR(AVG(log_period)), MONTH(log_date) as m, ".
		"MONTH(log_date)=MONTH(NOW()) ".
		"FROM session ".
		"WHERE log_date>DATE_SUB(NOW(), INTERVAL 1 YEAR) ".
		"GROUP BY m",
		$db_link);

  $list = array(
		array("month",		"Mois",			"text", ACL_ALL),
		array("count",		"Connexions",		"text", ACL_ALL,
		      0, array("center")),
		array("sum",		"Temps cumule",		"dayperiod", ACL_ALL,
		      0, array("center")),
		array("avg",		"Temps moyen",		"text", ACL_ALL,
		      0, array("center")),
		array("pcent",		"",			"percent", ACL_ALL,
		      0, array("pbar")),
		);

  for ($i = 1; $i <= 12; $i++)
    $data[$i][0] = 0;

  $max = 0;
  while($info = db_fetch_array($q))
    {
      $data[$info[3]] = array("sum" => $info[1],
			      "avg" => date("H:i:s", $info[2] - 3600),
			      "count" => $info[0],
			      "now" => $info[4]);
      $max = $max < $info[0] ? $info[0] : $max;
    }

  for ($i = 1; $i <= 12; $i++)
    {
      $data[$i]["pcent"] = floor($data[$i]["count"]*100/$max)+1;
      $data[$i]["month"] = $mlist[$i-1];
    }

  list_disp_data($data, $list, array("gray", "bluegray"), "now");

}

disp_data();

function disp_current()
{
  style_title("Liste des personnes connectees");

  $list = array(
		array("nom",		"Nom",			"link_info", ACL_ALL, 200),
		array("prenom",		"Prenom",		"link_info", ACL_ALL, 200),
		array("log_date",		"Date",		"text", ACL_ALL,
		      0, array("center")),
		array("period",		"Duree",	"text", ACL_ALL,
		      0, array("center")),
		array("idle",		"Idle",		"text", ACL_ALL,
		      0, array("center")),
		array("log_host",		"Host",		"text", ACL_ALL),
		);

  $res = db_query("SELECT session.log_host, user.nom, user.prenom, user.id, ".
		  db_time("log_date", "log_date").", CONCAT(FLOOR(session.log_period/60), ".
		  "':', log_period%60) AS period, UNIX_TIMESTAMP(NOW())-(UNIX_TIMESTAMP".
		  "(session.log_date)+session.log_period) AS idle ".
		  "FROM session, user ".
		  "WHERE  UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(session.log_date) ".
		  "       < (session.log_period+".LOGIN_TIMEOUT.") ".
		  "AND valid=1 ".
		  "AND session.uid=user.id ".
		  "ORDER BY idle",
		  $db_link);

  list_disp($res, $list, array("gray", "bluegray"), "even");
}

function disp_top50()
{
  style_title("Top 50 des connections");

  $list = array(
		array("nom",		"Nom",			"link_info", ACL_ALL, 200),
		array("prenom",		"Prenom",		"link_info", ACL_ALL, 200),
		array("period",		"Temps",		"dayperiod", ACL_ALL,
		      0, array("center")),
		);
  /*
  $res = db_query("SELECT user.id, user.nom, user.prenom, COUNT(*) AS count ".
		  "FROM session, user ".
		  "WHERE session.uid=user.id ".
		  "GROUP BY session.uid ORDER BY count DESC LIMIT 50",
		  $db_link);
  */
  $res = db_query("SELECT user.id, user.nom, user.prenom, SUM(session.log_period) AS period ".
		  "FROM session, user ".
		  "WHERE session.uid=user.id ".
		  "GROUP BY session.uid ORDER BY period DESC LIMIT 50",
		  $db_link);

  list_disp($res, $list, array("gray", "bluegray"), "even");
}

if (access_check(ACL_SITEADM | ACL_ASSIST_ALL))
{
disp_current();
disp_top50();
}

?>

