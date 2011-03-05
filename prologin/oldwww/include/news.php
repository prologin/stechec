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

function news_disp($news)
{
  print("<table class=table_news>\n");

  print("<tr>\n");
  print("<td class=table_news_head>\n");
  print("<table><tr><td class=news_logo>&nbsp</td>");
  print("<td class=news_title>".$news["title"]);
  print("</td></tr></table></td>\n");
  print("</tr>\n");

  print("<tr>\n");
  print("<td class=table_news_back>\n");
  print(nl2br2($news["body"]));
  print("</td>\n");
  print("</tr>\n");

  print("<tr>\n");
  print("<td class=table_news_foot>\n");
  if (intval($news["visible"]) == 0)
    printf("[Retiree] ");
  print("Postee le (".$news["date"].") - Par (".$news["auteur"].")");
  print("</td>\n");
  print("</tr>\n");

  print("</table>\n\n");
}

function news_list_disp($query)
{
  if (db_num_rows($query) <= 0)
    style_text("Aucune news disponible");

  while ($data = db_fetch_array($query))
    {
      news_disp($data);

      $id_news = $data[id];
		       
      style_links(array(
			array("Voir le detail", ACL_NOT_ELEVE,
			      array("_id_page" => 141, "_entry" => $id_news)),
			array("Retirer", ACL_NOT_ELEVE,
			      array("_id_page" => 14, "_entry" => $id_news, "_action" => "hide_news"),
			      "Voulez vous retirer cette news ?"),
			array("Detruire", ACL_SITEADM,
			      array("_id_page" => 14, "_entry" => $id_news, "_action" => "del_news"),
			      "Voulez vous detruire cette news ?"),
		    ));
    }
}

function disp_news_page($count)
{
  global $_limit;

  $_limit = intval($_limit);

  if (!access_check(ACL_ORGA))
    $hide_news = "AND   visible=1 ";

  $res = db_query("SELECT news.id, news.title, news.body, ".db_time("news.date", "date").", news.visible, ".
		  "       CONCAT(user.nom, ' ', user.prenom) AS auteur ".
		  " FROM news, user ".
		  " WHERE user.id=news.author_id ".
		  $hide_news.
		  " ORDER BY news.date DESC ".
		  " LIMIT ".($_limit * $count).", ".$count);

  news_list_disp($res);
}

function news_check_right($news_id)
{
  global $user;

  if (access_check(ACL_ORGA))
    return (1);

  return db_num_rows(db_query("SELECT id FROM news ".
			      " WHERE id=".$news_id." ".
			      " AND   author_id=".$user[id],
			      $db_query)) > 0;
}

function news_new_template()
{
  return (array(
		array("",	"Nouvelle news", "title", ACL_ALL, 0),
		array("title", "Titre", "text", ACL_ALL, ACL_ALL),
		array("body", "Corps", "textarea_big", ACL_ALL, ACL_ALL)
		));
}

function news_edit_template($acl)
{
  return(array(
	       array("",	"News",		"title", ACL_ALL, 0),
	       array("title",	"Titre",	"text",	ACL_ALL, $acl),
	       array("body",	"Corps",	"textarea_big",	ACL_ALL, $acl),
	       array("date",	"Date",		"datetime",	ACL_ALL, ACL_ORGA),
	       array("visible",	"Visible",	"checkbox", ACL_ORGA, ORGA)
	       ));
}

function news_action()
{
  global $user;
  global $_entry;
  global $_action;


  if (access_check(ACL_CANDIDAT))
    return;


  switch ($_action)
    {
    case "new_news":

      table_insert("news", news_new_template(), 
		   array("date" => "NOW()",
			 "author_id" => $user["id"]));
      break;

    case "edit_news":

      if (!news_check_right($_entry))
        break;

      table_update("news", news_edit_template(ACL_NOT_ELEVE), "id=".$_entry);
      break;

    case "hide_news":
      
      if (!news_check_right($_entry))
	{
	  style_alert("Vous n'etes pas l'auteur de cette news");
	  break;
	}

      db_query("UPDATE news SET visible=visible^1 WHERE id=".$_entry);
      break;

    case "del_news":
      
      if (!access_check(ACL_SITEADM))
        break;

      db_query("DELETE FROM news WHERE id=".$_entry);
      break;
    }
}


?>

