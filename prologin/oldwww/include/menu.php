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

function href_var($list, $nossl = 0)
{
  global $post_data;
  $data = $post_data;

  for (reset($list);
       $key = key($list);
       next($list))
    $data[$key] = pos($list);

  for (reset($data);
       $key = key($data);
       next($data))
    {
      if ($str)
	$str .= "&";
      $str .= "$key=" . pos($data);
    }

//  if ($nossl == 1)
//    $str = "./index.php:NOSSL:?".$str;
//  else
    $str = "index.php?".$str;

  return $str;
}

function href($list, $nossl = 0)
{
  print(href_var($list, $nossl));
}

function page_get($id_page)
{
  global $db_link;
  global $profil;

  $res = db_query("SELECT * FROM page ".
		  "WHERE id_page='".$id_page."'",
		  $db_link);

  if (db_num_rows($res) != 1)
    die("Error: 1.page_get()");

  $array = db_fetch_array($res);

  while ($array["file"] == "")
    {
      $res = db_query("SELECT * FROM page ".
		      "WHERE parent=".$array["id_page"]." ".
		      "AND access_level&".$profil["access"].
		      " AND hidden=0 ORDER BY sort, id_page",
		      $db_link);

      if (db_num_rows($res) < 1)
	die("Error: 2.page_get()");

      $array = db_fetch_array($res);
    }

  db_query("UPDATE page SET access_count=access_count+1 ".
	   "WHERE id_page=".$array["id_page"], $db_link);

  return $array;
}

function menu_bar($page)
{
  global $post_data;
  global $_info_id;

  function disp_menu_bar($name, $list, $selected)
    {
      global $_login;

      print("<div class=".$name."_bar>\n");
      print("<table cellspacing=0 cellpadding=0><tr>\n");

      for ($j = 0; $j < count($list); $j++)
	{
	  /* print("<td width=40>"); */
	  /* print("<td class=".$name."_space>&nbsp</td>\n"); */
	  /*print("</td>"); */


	  $sel = $selected == $list[$j][id_page] ? "_selected" : "";
	  print("<td><a class=".$name."_button$sel href='");
	  href(array("_id_page" => $list[$j]["id_page"]), 1);
	  print("'>".$list[$j]["nom"]."</a></td>\n");
	}
      print("</tr></table>\n");

      print("</div>\n");
    }

  global $db_link;
  global $profil;

  $res1 = db_query("SELECT * FROM page ".
		   "WHERE level=0 AND hidden=0 ".
		   "AND access_level&".$profil["access"].
		   " ORDER BY sort, id_page",
		   $db_link);

  $id_page1 = $page[id_page];

  if ($page[level] > 0)
    {
      $res2 = db_query("SELECT * FROM page ".
		       "WHERE parent='".$page[parent]."' AND hidden=0 ".
		       "AND access_level&".$profil["access"].
		       " ORDER BY sort, id_page",
		       $db_link);

      while ($array = db_fetch_array($res2))
	$list2[] = $array;

      $id_page1 = $page[parent];
      $id_page2 = $page[id_page];
    }

  while ($array = db_fetch_array($res1))
    $list1[] = $array;

  disp_menu_bar("menu1", $list1, $id_page1);

  $post_data["_info_id"] = $_info_id;

  disp_menu_bar("menu2", $list2, $id_page2);
}

?>

