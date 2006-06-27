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

function candidat_photo()
{
  global $user;
  global $_limit;

  $count = db_fetch_row(db_query("SELECT COUNT(*) FROM user".
				 " WHERE user.id_profil=1"));

  global $_pervers;
  if (intval($_pervers))
    $pervers = " AND sexe ";

  /* exec db query */
  $res = db_query("SELECT CONCAT(nom, '<br>', prenom) AS name, id, photo, id_profil ".
		  " FROM user ".
		  " WHERE id_profil=1 ".
		  $pervers.
		  " ORDER BY nom, prenom ".
		  " LIMIT ".($_limit * 25).", 25");

  style_title("Photos des candidats");

  style_slice($_limit, 20, $count[0]/25);

  $i = 0;
  print("<table class=table_list cols=5>");
    for ($i = 0; $i < 25;)
    {
      if (!($i % 5))
	print("<tr class=table_list_gray width=20%>");

      print("<td class=table_list_center>");

      if ($row = db_fetch_row($res))
	{
	  print("<img class=table_info_photo src='".$row[2]."'>".
		"<br><a href='");
	  href(array("_id_page" => 200, "_info_id" => $row[1]));
	  print("'>".$row[0]."</a>");
	}
      else
	{
	  print("<img class=table_info_photo src='photo/none.jpg'><br>".
		"Aucun");
	}
      print("</td>");

      $i++;
      if (!($i % 5))
	print("</tr>");
    }
  
  if ($i % 5)
    print("<tr>");

  print("</table>");

  style_slice($_limit, 20, $count[0]/25);
}

include "content/individus/photo_menu.php";

candidat_photo();

?>

