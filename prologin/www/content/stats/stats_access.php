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

  style_title("Statistiques d'access aux pages");

  $info = db_fetch_row(db_query("SELECT COUNT(*), SUM(access_count), ".
				"@m:=MAX(access_count), TRUNCATE(AVG(access_count), 2), ".
				"MIN(access_count) FROM page ".
				"WHERE (page.access_level|128)&".$profil["access"]." ".
				"AND   page.access_count>0",
				$db_link));

  $list = array(
		array("title",		"Information",		"text", ACL_ALL),
		array("data",		"Donnee",		"text", ACL_ALL,
		      0, array("center")),
		);

  $data = array(
		array("title" => "Nombre d'acces total", "data" => $info[1]),
		array("title" => "Nombre de pages disponibles", "data" => $info[0]),
		array("title" => "Nombre d'acces moyen par page", "data" => $info[3]),
		array("title" => "Nombre d'acces a la page la moins visitee", "data" => $info[4]),
		array("title" => "Nombre d'acces a la page la plus visitee", "data" => $info[2]),
		);

  list_disp_data($data, $list, array("gray", "bluegray"), "even");

  $list = array(
		array("parent",		"Page",			"text", ACL_ALL),
		array("nom",		"",			"text", ACL_ALL),
		array("access_count",	"Nombre d'acces",	"text", ACL_ALL,
		      0, array("center")),
		array("pcent",		"",			"percent", ACL_ALL,
		      0, array("pbar")),
		);

  $res = db_query("SELECT page.nom, page.access_count, FLOOR(page.access_count*100/@m)+1 AS pcent, ".
		  "       IF(pageb.id_page, pageb.nom, '') AS parent, pageb.id_page AS idparent ".
		  "FROM page LEFT JOIN page AS pageb ".
		  "ON    page.parent=pageb.id_page ".
		  "WHERE (page.access_level|128)&".$profil["access"]." ".
		  "AND   page.access_count>0 ".
		  "ORDER BY page.parent",
		  $db_link);

  list_disp($res, $list, array("gray", "bluegray"), "idparent");

}

disp_data();

?>

