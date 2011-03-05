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

function comment_new_template()
{
  /* donnees du formulaire de creation de projet */
  return (array(
		array("id_target",	"Destinataire",		"text", ACL_ALL, ACL_ASSIST),
		array("id_auteur",	"Auteur",		"text", ACL_ALL, ACL_ASSIST),
		array("date",		"Date",			"datetime", ACL_ALL, ACL_ASSIST),
		array("max_groupe",	"Nombre de groupes max","text", ACL_ALL, ACL_ASSIST),
		array("max_user",	"Nombre d'eleves max",	"text", ACL_ALL, ACL_ASSIST),
		array("coef",		"Coefficient",		"text", ACL_ALL, ACL_ASSIST),
		));
}

?>

