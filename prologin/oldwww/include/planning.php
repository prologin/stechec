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

$mlist = array("Janvier", "Fevrier", "Mars", "Avril",
	       "Mai", "Juin", "Juillet", "Aout",
	       "Septembre", "Octobre", "Novembre", "Decembre");
$dlist = array("Lundi", "Mardi", "Mercredi", "Jeudi",
	       "Vendredi", "Samedi", "Dimanche");

function style_planning($list, $month)
{
  global $mlist, $dlist;
  global $db_link;

  $year = floor($month / 12);
  $month = $month % 12 + 1 ;

  $first_monday = mktime(0, 0, 0, $month, 1, $year);
  $last_sunday = $first_monday + date("t", $first_monday) * 86400;
  $first_cell = $first_monday - ((date("w", $first_monday) + 6) % 7) * 86400;
  $last_cell = ($last_sunday - $first_cell) <= 5*86400*7
    ? $first_cell + 5*86400*7
    : $first_cell + 6*86400*7;

  print("<table class=table_plan cols=7>");

  print("<tr><td class=table_plan_title colspan=7>".
	$mlist[$month - 1]." ".
	date("Y", $first_monday)."</td></tr>");

  print("<tr>");
  for ($j = 0; $j < 7; $j++)
    print("<td class=table_plan_title2>".$dlist[$j]."</td>");
  print("</tr>");

  for ($w = $first_cell; $w < $last_cell; $w += 86400*7)
    {
      print("<tr>");
      for ($d = $w; $d < $w + 86400*7; $d += 86400)
	{
	  $i = date("j", $d + 43200);
	  if (($d >= $first_monday) && ($d < $last_sunday))
	    {
	      if (time() >= $d && time() < $d + 86400)
		print("<td class=table_plan_today>");
	      else
		print("<td class=table_plan_cell>");
	      print("<div class=table_plan_num>".$i."</div>");
	      for ($j = 0; $j < count($list[$i - 1]); $j++)
		print("<li>".$list[$i - 1][$j]."</li>");
	    }
	  else
	    print("<td class=table_plan_cell2>&nbsp");
	  print("</td>");
	}
      print("</tr>");
    }
  print("</table>");

}

function planning_add_project(&$events, $begin, $end, $promo)
{
  global $db_link;

  $res = db_query("SELECT UNIX_TIMESTAMP(date_distrib), UNIX_TIMESTAMP(date_cloture), ".
		  "UNIX_TIMESTAMP(date_rendu), nom, id ".
		  "FROM projet ".
		  "WHERE promo=".$promo." AND id_statut>1", $db_link);

  while ($row = db_fetch_row($res))
    for ($i = 0; $i < 3 ; $i++)
      {
	$label = array("Publication", "Fin inscriptions", "Rendu");

	if (($row[$i] > $begin) && ($row[$i] < $end))
	  $events[date("j", $row[$i]) - 1][] = "<div class=event_project>".
	    $label[$i].": </div><a class=links href='".
	    href_var(array("_id_page" => 32,
			   "_info_id" => $row[4])).
	    "'>".$row[3]."</a>";
      }
}

function planning_add_event(&$events, $begin, $end, $promo)
{
  global $db_link;

  $res = db_query("SELECT UNIX_TIMESTAMP(date), UNIX_TIMESTAMP(date_fin), ".
		  "name, is_oblig, id, TO_DAYS(date)=TO_DAYS(date_fin) ".
		  "FROM events ".
		  "WHERE (promo=".$promo." AND is_global=0) OR is_global ".
		  "AND UNIX_TIMESTAMP(IF(date_fin, date_fin, date))>=".$begin." ".
		  "AND UNIX_TIMESTAMP(date)<=".$end." ".
		  "ORDER BY date", $db_link);

  while ($row = db_fetch_row($res))
    {
      $div = "<div class=".(intval($row[3]) ? "event_oblig" : "event_nooblig").">";
      $link = "<a  class=links href='".href_var(array("_id_page" => 151,
					 "_info_id" => $row[4]))."'>".$row[2]."</a>";
      if ($row[1] && !$row[5])
	{
	  if (($row[1] > $begin) && ($row[1] < $end))
	    $events[date("j", $row[1]) - 1][] = $div."Fin :</div> ".$link;
	  $start = "Debut";
	}
      else
	$start = "Evenement";

      if (($row[0] > $begin) && ($row[0] < $end))
	$events[date("j", $row[0]) - 1][] = $div.$start." :</div> ".$link;
    }
}

function planning_add_birthday(&$events, $begin, $end, $promo)
{
  global $db_link;

  $res = db_query("SELECT DISTINCT UNIX_TIMESTAMP(CONCAT(YEAR(FROM_UNIXTIME(".
		  $begin.")),'-',MONTH(user.date_naiss),'-',DAYOFMONTH(user.date_naiss)))+43200, ".
		  "CONCAT(user.nom, ' ', user.prenom) AS name, user.id ".
		  "FROM user, user_prof ".
		  "WHERE (user_prof.promo=".$promo." OR user_prof.promo=0) ".
		  "AND   user.id=user_prof.uid ".
		  "AND   MONTH(user.date_naiss)=MONTH(FROM_UNIXTIME(".$begin.")) ",
		  $db_link);

  while ($row = db_fetch_row($res))
    {
      $div = "<div class=event_nooblig>";
      $link = "<a class=links href='".href_var(array("_id_page" => 200,
					 "_info_id" => $row[2]))."'>".$row[1]."</a>";

      if (($row[0] > $begin) && ($row[0] < $end))
	$events[date("j", $row[0]) - 1][] = "<div class=event_nooblig>Birthday:</div> ".$link;
    }
}

function event_check_access($event_id)
{
  global $user_profil;

  if (access_check(ACL_SITEADM))
    return (1);

  return db_num_rows(db_query("SELECT id FROM events ".
			      "WHERE id=".$event_id." ".
			      "AND   id_auteur=".$user_profil[id],
			      $db_query)) > 0;
}

function event_template($acl)
{
  return(array(
	       array("",		"Description",		"title", ACL_ALL, 0),
	       array("name",		"Nom",			"text",	ACL_ALL, $acl),
	       array("descrip",		"Description",		"textarea_big",	ACL_ALL, $acl),
	       array("is_oblig",	"Activite obligatoire",	"checkbox", ACL_ALL, $acl),
	       array("",		"Dates",		"title", ACL_ALL, 0),
	       array("date",		"Date",			"datetime",	ACL_ALL, $acl),
	       array("date_fin",	"Date fin",		"datetime", ACL_ALL, $acl),
	       array("",		"Promotions concernees","title", ACL_ALL, 0),
	       array("promo",		"Concernant la promo",	"text", ACL_ALL, $acl),
	       array("is_global",	"Toutes promos",	"checkbox", ACL_ALL, $acl),
	       ));
}

?>

