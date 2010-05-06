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

function disp_profil($id)
{
  style_title("Mes informations");

  $list = array(
		array("",		"Identit&eacute;",	"title", ACL_ALL, 0),
		array("photo",		"Photo",		"photo", ACL_ALL, 0),
		array("photo",		"Photo",		"photo", ACL_ORGA, ACL_ORGA),
		array("login",		"Login",		"text", ACL_ALL, 0),
		array("prenom",		"Prenom",		"text", ACL_ALL, ACL_ORGA),
		array("nom",		"Nom",			"text", ACL_ALL, ACL_ORGA),
		array("nickname",	"Nickname",		"text", ACL_ALL, ACL_ALL),
		array("email",	        "E-mail",		"text", ACL_ALL, ACL_ALL),
		array("",		"Donn&eacute;es personnelles",	"title", ACL_ALL, 0),
		array("date_naiss",	"Date de naissance",	"date", ACL_ALL, ACL_ALL),
		array("sexe",		"Sexe",			"bool", ACL_ALL, ACL_ALL,
		      array("Masculin", "Feminin")),
		array("adresse",	"Adresse",		"text", ACL_ALL, ACL_ALL),
		array("codep",		"Code postal",		"text", ACL_ALL, ACL_ALL),
		array("ville",		"Ville",		"text", ACL_ALL, ACL_ALL),
		array("phone",		"Telephone",		"text", ACL_ALL, ACL_ALL),
		array("gsm",		"GSM",			"text", ACL_ALL, ACL_ALL),
		);

  table_disp(table_select("user", $list, "id=$id"), $list, "edit_user", "_entry");
}

disp_profil($user["id"]);

?>

