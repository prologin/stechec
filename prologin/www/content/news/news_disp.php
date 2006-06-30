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

include("include/news.php");


style_title("Liste des news ".$promo);

style_links(array(
  	array("Poster une news", ACL_NOT_ELEVE, array("_id_page" => 140)),
	));

$count = db_fetch_row(db_query("SELECT COUNT(*) FROM news ".
			       " WHERE visible"));

style_slice($_limit, 10, $count[0] / 10);

news_action();
disp_news_page(10);

style_slice($_limit, 10, $count[0] / 10);


?>
