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

function disp_candidat()
{
  global $_limit;

  style_title("Liste des candidats");

  if (!$_limit)
    $_limit = ord("A");

  /* exec db query */
  $res = db_query("SELECT * FROM user ".
		  " WHERE user.id_profil=1 ".
		  " AND ASCII(UCASE(user.nom))=".$_limit." ".
		  " ORDER BY user.nom ");

  style_slice_alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ", $_limit);

  list_disp($res, array(
			array("nom",		"Nom",		"link_info", ACL_ALL, 200),
			array("prenom",		"Prenom",	"link_info", ACL_ALL, 200),
			array("nickname",	"Nickname",	"text", ACL_ALL),
			), array("gray", "bluegray"), "even");

  style_slice_alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ", $_limit);

}


disp_candidat();

?>

