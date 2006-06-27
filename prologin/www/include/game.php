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

/*
**
*/
function game_upload_conf_set($id_projet, $max_size, $regexp, $max_upload)
{
  global $db_link;

  $regexp = addslashes($regexp);

  $res = db_query("SELECT * FROM prj_upload_conf WHERE id_projet=$id_projet",
		  $db_link);
  if (db_num_rows($res))
    db_query("UPDATE prj_upload_conf SET max_upload_size=$max_size,".
	     " name_regexp='$regexp', max_upload=$max_upload".
             " WHERE id_projet=$id_projet");
  else
    db_query("INSERT INTO prj_upload_conf".
             " (id_projet, max_upload_size, name_regexp, max_upload)".
             " VALUES($id_projet, $max_size, '$regexp', $max_upload)");
}



/*
** supprime toute les donnees relatives au projet dans
** la base de donnees (groupes, chefs de groupe, responsables
** du projet, projet)
*/
function game_del($id_projet)
{
  /* le projet */
  db_query("DELETE FROM game WHERE id=".$id_projet);
}

/*
** Fonction appelee quand on veut afficher des donnees relatives
** a un jeu mais qu'aucun projet n'a ete selectionne.
** Affiche un lien vers la liste des projets
*/
function game_noselect()
{
  style_text("Veuillez selectionner un jeu dans la liste...");
  style_links(array(
		    array("Acceder a la liste des jeux", ACL_ELEVE,
			  array("_id_page" => 30))
		    ));
  return (1);
}

/*
** Retourne un tableau template pour la creation de projet
** (definition de l'interface de creation de projet)
*/
function game_new_template()
{
  /* donnees du formulaire de creation de projet */
  return (array(
		array("", "Informations jeux", "title", ACL_ALL, 0),
		array("nom", "Nom", "text", ACL_ALL, ACL_ORGA),
		array("comment", "Description", "text", ACL_ALL, ACL_ORGA),
		array("repertoire", "Nom du repertoire", "text", ACL_ALL, ACL_ORGA),
		array("lib_name", "Nom de la lib", "text", ACL_ALL, ACL_ORGA),
		array("max_champion", "Nombre de champion max",	"text", ACL_ALL, ACL_ORGA),
		array("arg_server", "Server: argument constant", "text", ACL_ALL, ACL_ORGA),
		));
}




?>


