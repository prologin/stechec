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

function my_promo_disp()
{
  global $db_link;
  global $user_profil;
  global $_limit;
  global $_action;

  $list = array(
		array("",		"Identit&eacute;",	"title",
		      ACL_ALL, 0),
		array("prenom",		"Prenom",		"text",
		      ACL_ALL, ACL_ALL),
		array("nom",		"Nom",			"text",
		      ACL_ALL, ACL_ALL),
		array("nickname",	"Nickname",		"text",
		      ACL_ALL, ACL_ALL),
		array("",		"Donn&eacute;es personnelles",	"title",
		      ACL_SITEADM | ACL_ASSISTCHEF, 0),
		array("sexe",		"Sexe",			"bool",
		      ACL_SITEADM | ACL_ASSISTCHEF, ACL_SITEADM | ACL_ASSISTCHEF,
		      array("", "Masculin", "Feminin")),
		array("ville",		"Ville",		"text",
		      ACL_SITEADM | ACL_ASSISTCHEF, ACL_SITEADM | ACL_ASSISTCHEF),
		array("adresse",	"Adresse",		"text",
		      ACL_SITEADM | ACL_ASSISTCHEF, ACL_SITEADM | ACL_ASSISTCHEF),
		);


  switch ($_action)
    {
    case ("search_user"):
      if ($res = table_search("user", $list, " ORDER BY nom ", 50))
	{
	  style_title("Resultat de la recherche");

	  if (db_num_rows($res) >= 50)
	    style_text("Seulement les 50 premieres reponses ont ete affichees.");

	  list_disp($res, array(
		array("nom", "Nom", "link_info", ACL_ALL, 200),
		array("prenom",	"Prenom", "link_info", ACL_ALL, 200),
		array("nickname", "Nickname", "text", ACL_ALL, 0, array("center")),
		), 
                array("gray", "bluegray"), "even");
	  break;
	}

    default:

      $data = array();

      style_title("Recherche d'utilisateurs");

      table_disp_one($data, $list, "search_user", "_entry");
    }
}



my_promo_disp();

?>

