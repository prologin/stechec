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

<?

include("include/news.php");



function edit_news()
{
  global $post_data;
  global $_entry;
  global $_promo;

  if ($_entry <= 0)
    return;

  $post_data["_id_page"] = "14";
  $post_data["_promo"] = intval($_promo);

  style_title("Contenu de la news");

  $acl = news_check_right($_entry) ? ACL_NOT_ELEVE : 0;
  $list = news_edit_template($acl);

  $query = table_select("news", $list, "id=".$_entry);
  table_disp($query, $list, "edit_news", "_entry");
}

edit_news();

?>

