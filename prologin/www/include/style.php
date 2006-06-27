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

function style_alert($str)
{
  print("<script language='javascript'>");
  print("alert('".addslashes($str)."');");
  print("</script>");
}

function style_percent($n, $size)
{
  print("<table width=".$size."% class=percent_bar><tr><td width='".$n.
	"%' class=percent_bar_fg></td><td class=percent_bar_bg>".
	"</td></tr></table>");
}

function style_init($id_style)
{
  global $db_link;

  /* get profile css file name */

  $res = db_query("SELECT file FROM style ".
		  "WHERE id='".$id_style."'",
		  $db_link);

  if (db_num_rows($res) < 1)
    {
      $res = db_query("SELECT file FROM style ".
		      "WHERE id=1 ", $db_link);

      if (db_num_rows($res) < 1)
	die("Error: style_init()");
    }

  $row = mysql_fetch_row($res);
  $file = $row[0];

  /* write css include link */

  print("<link rel=StyleSheet href='$file' type='text/css'>");
}

function style_title($title)
{
	print("<table class=text_title_table><tr>");
	print("<td class=text_title>$title</td>");
	print("</tr></table><p>");
}

function style_text($text)
{
	print("<div class=text>".$text."</div>");
}

function style_slice($limit, $count, $max = 1000)
{
  $first = $limit - ($limit % $count);

  print("<table class=table_links><tr>\n");

  print("<td class=table_links_cell>");
  if ($first > 0)
    {
      print("<a class=links href='");
      href(array("_limit" => max($first - 1, 0)));
      print("'><<</a></td>\n");
    }
  else
    print("<<</td>\n");

  for ($i = $first; $i < $first + $count; $i++)
    {
      if ($i >= $max)
	{
	  print("<td class=table_links_cell>>></td></tr></table>\n");
	  return;
	}
      print("<td class=table_links_cell>");
      if ($limit != $i)
	{
	  print("<a class=links href='");
	  href(array("_limit" => $i));
	  print("'>".($i + 1)."</a></td>\n");
	}
      else
	print(($i + 1)."</td>\n");
    }

  print("<td class=table_links_cell>".
	"<a class=links href='");
  href(array("_limit" => $first + $count));
  print("'>>></a></td>\n");

  print("</tr></table>\n");
}

function style_slice_alpha($str, $limit)
{
  print("<table class=table_links><tr>\n");

  for ($i = 0; $i < strlen($str); $i++)
    {
      print("<td class=table_links_cell>");
      if ($limit != ord($str[$i]))
	{
	  print("<a class=links href='");
	  href(array("_limit" => ord($str[$i])));
	  print("'>".$str[$i]."</a></td>\n");
	}
      else
	print($str[$i]."</td>\n");
    }

  print("</tr></table>\n");
}


/*
** IN
** list (title, ACL, postdata list (name, value), [confirm message] )
** or
** list (title, ACL, link string, [confirm message] )
*/

function style_links($list)
{
  $j = 1;

  for ($i = 0; $i < count($list); $i++)
    {
      /* check access level */
      if (access_check($list[$i][1]))
	{
	  if ($j)
	    {
	      print("<table class=table_links><tr>\n");
	      $j--;
	    }

	  print("<td class=table_links_cell><a class=links ");
	  if ($list[$i][3])
	    print("onclick=\"return (confirm('".$list[$i][3]."'))\" ");
	  print("href='");
	  if (is_array($list[$i][2]))
	    href($list[$i][2]);
	  else
	    print($list[$i][2]);
	  print("'>".$list[$i][0]."</td>\n");
	}
    }

  if ($j == 0)
    print("</tr></table>\n");
}

?>