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

<table width="97%" border=0 cellspacing=10 cellpading=0>
<tr>
<td width="75%" valign="top">

<?php


include "include/sondage.php";

function disp_sondages_short($promo)
{
  if ($promo <= 0)
    return;


  $res = db_query("SELECT @sid:=id, titre, question ".
		  "FROM sondage ".
		  "WHERE promo=".$promo." ".
		  "AND statut=".SND_ST_ENCOUR." ".
		  "AND is_main ".
		  "ORDER BY RAND() LIMIT 1", $db_link);


  if (db_num_rows($res) <= 0)
    return;

  $snd = db_fetch_row($res);

  db_query("SELECT @votes:=GREATEST(COUNT(*), 1) ".
	   "FROM sondage_votes ".
	   "WHERE sondage_votes.sondage_id=@sid",
	   $db_link);

  $res = db_query("SELECT sondage_props.proposition, ".
		  "TRUNCATE(IF(sondage_votes.voter_id, COUNT(*), 0)*100/@votes, 2) AS pvotes ".
		  "FROM sondage_props LEFT JOIN sondage_votes ".
		  "ON sondage_votes.prop_id=sondage_props.id ".
		  "WHERE sondage_props.sondage_id=@sid ".
		  "GROUP BY sondage_props.id ORDER BY pvotes DESC", $db_link);

  print("<table class=right_cadre cols=2>");
  print("<tr><td class=right_cadre_title colspan=2><a class=links href='");
  href(array("_info_id" => $snd[0], "_id_page" => 53));
  print("'>".$snd[2]."</a></td></tr>");

  while ($prp = db_fetch_row($res))
  {
    print("<tr><td class=right_cadre_prop>".$prp[0]."</td>");
    print("<td class=right_cadre_prop>".$prp[1]."%</td></tr>");
    print("<td valign=middle colspan=2>");
    print("<table width='100%' height=3><tr><td width='".(floor($prp[1]) + 1).
	  "%' class=percent_bar_fg></td><td class=percent_bar_bg>".
	  "</td></tr></table>");
  }
  print("</table><br>");
}

function disp_classement_short($promo)
{
  global $db_link;
  global $user_profil;

  if ($promo <= 0)
    return;

  /* exec db query */
  $res = db_query("DO @i:=1, @last:=100", $db_link);

  $res = db_query("SELECT IF(FLOOR(@last*100)>FLOOR(value*100), @rank:=@i, @rank) AS rank, ".
		  "name, TRUNCATE(@last:=value, 2) AS value, id, @i:=@i+1 ".
		  "FROM (SELECT CONCAT(user.nom, ' ', user.prenom) AS name, ".
		  "notes_mod_val.value, user.id ".
		  "FROM user, user_prof, notes_mod_val, notes_mod ".
		  "WHERE user.id=user_prof.uid ".
		  "AND user_prof.id=notes_mod_val.id_user ".
		  "AND notes_mod_val.id_def=notes_mod.id ".
		  "AND notes_mod.promo=".$promo." ".
		  "AND notes_mod.is_main ".
		  "ORDER BY notes_mod_val.value DESC LIMIT 10) ".
		  "AS t ORDER BY t.value DESC, t.name",
		  $db_link);

  print("<table class=right_cadre cols=3>");
  print("<tr><td class=right_cadre_title colspan=3>Top 10</td></tr>");
  while ($row = db_fetch_row($res))
    {
      print("<tr><td width=28 align=center class=right_cadre_prop><B>".$row[0].".</B></td>".
	    "<td class=right_cadre_prop><a class=right_cadre_prop href='");
      href(array("_id_page" => 200, "_info_id" => $row[3]));
      if (access_check(ACL_ELEVE))
        print("'>".$row[1]."</a></td></tr>");
      else
        print("'>".$row[1]."</a></td>".
              "<td width=28 class=right_cadre_prop>".$row[2]."</td></tr>");
    }
  print("</table><br>");
}

function disp_candidat_of_day()
{
  global $user;


  print("<table class=right_cadre>");

  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom), ".
		    "       user.nickname, ".
		    "       user.id, user.photo, RAND() AS rank ".
		    " FROM  user ".
		    " WHERE user.id_profil=1 ".
		    " AND   DAYOFYEAR(user.date_naiss)=DAYOFYEAR(NOW()) ".
		    " ORDER BY rank LIMIT 1");

  if (db_num_rows($query) > 0)
    {
      print("<tr><td class=right_cadre_title>Birthday</td></tr>");
    }
  else
    {
      $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom), ".
			"       user.nickname, user.id, user.photo, ".
			"       RAND(UNIX_TIMESTAMP(NOW()) / 43200) AS rank ".
			" FROM   user ".
			" WHERE  user.id_profil=1 ".
			" ORDER BY rank LIMIT 1");

      print("<tr><td class=right_cadre_title>Le candidat du jour</td></tr>");
    }

  $row = db_fetch_row($query);

  print("<tr><td align=center class=right_cadre_title>".
	"<img src='".$row[3]."' class=table_info_photo></td></tr>".
	"<tr><td class=right_cadre_title><a class=links href='");
  href(array("_id_page" => 200, "_info_id" => $row[2]));
  print("'>".$row[0]."</a>");
  if (strlen($row[1]))
    print("<br>(".$row[1].")</td></tr>");
  print("</table><br>");
}

include("include/news.php");

{
    {
      global $_limit;

      $_limit = 0;
      disp_news_page(5);

      style_links(array(
			array("Archive des news", ACL_ALL, array("_id_page" => 14)),
			));
    }
}

?>

</td>
<td valign=top>

 <table width="100%" border=0 cellspacing=0 cellpading=0>
 <tr><td>


<?php

disp_candidat_of_day();
//disp_classement_short();
//disp_sondages_short();


?>


 </td></tr></table>

</td></tr></table>

