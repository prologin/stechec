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

define(NOTES_LNK_NONE, 0);
define(NOTES_LNK_PRJ, 1);

define(ACL_MODUL_EDIT, ACL_ASSISTCHEF | ACL_SITEADM);

function disp_my_modules($id_profil)
{
  global $db_link;

  $list_note = array(
		    array("nom",		"Nom",		"text", ACL_ALL),
		    array("mod_nom",		"Parent",	"text", ACL_ALL),
		    array("coef",		"Coef",		"text", ACL_ALL,
			  0, array("center")),
		    array("value",		"Note",		"text", ACL_ALL,
			  0, array("center")),
		    array("moy_gen",		"Promo",	"text", ACL_ALL,
			  0, array("center")),
		    array("id",		"Classement",	"link",	ACL_ALL,
			  array("_is_modul" => 1, "_id_page" => 74),
			  array("center"), "_info_id"),
		    array("id",		"Liste",	"link",	ACL_ALL,
			  array("_is_modul" => 1, "_id_page" => 75),
			  array("center"), "_info_id"),
		    );

  $q1 = db_query("SELECT notes_mod.id, notes_mod.nom, TRUNCATE(notes_mod.coef, 1) AS coef, ".
		 "       notes_mod.valid, TRUNCATE(notes_mod_val.value, 2) AS value, ".
		 "       TRUNCATE(notes_mod.moy_gen, 2) AS moy_gen, ".
		 "       notes_modb.nom AS mod_nom, ".
		 "       (notes_mod_val.value<10)+2*(notes_mod_val.value<notes_mod.moy_gen) AS color ".
		 "FROM   notes_mod, notes_mod AS notes_modb, notes_mod_val ".
		 "WHERE  notes_mod.id_parent=notes_modb.id ".
		 "AND    notes_mod.valid>0 ".
		 "AND    notes_mod_val.id_def=notes_mod.id ".
		 "AND    notes_mod_val.id_user=".$id_profil." ".
		 "ORDER BY notes_mod.id_parent, notes_mod.id",
		 $db_link);

  list_disp($q1, $list_note, array("gray", "red", "red2", "red3"), "color");
}

function disp_my_notes($id_profil)
{
  global $db_link;

  $list_note = array(
		    array("nom",		"Nom",		"text", ACL_ALL),
		    array("mod_nom",		"Module",	"text", ACL_ALL),
		    array("coef",		"Coef",		"text", ACL_ALL,
			  0, array("center")),
		    array("value",		"Note",		"text", ACL_ALL,
			  0, array("center")),
		    array("moy_gen",		"Promo",	"text", ACL_ALL,
			  0, array("center")),
		    array("id",		"Classement",	"link",	ACL_ALL,
			  array("_id_page" => 74), array("center"), "_info_id"),
		    array("id",		"Liste",	"link",	ACL_ALL,
			  array("_id_page" => 75), array("center"), "_info_id"),
		    );

  $q1 = db_query("SELECT notes_def.id, notes_def.nom, TRUNCATE(notes_def.coef, 1) AS coef, ".
		 "       notes_def.valid, TRUNCATE(notes_val.value, 2) AS value, ".
		 "       TRUNCATE(notes_def.moy_gen, 2) AS moy_gen, ".
		 "       notes_mod.nom AS mod_nom, ".
		 "       (notes_val.value<10)+2*(notes_val.value<notes_def.moy_gen) AS color ".
		 "FROM   notes_def, notes_mod, notes_val ".
		 "WHERE  notes_def.id_modul=notes_mod.id ".
		 "AND    notes_def.valid>0 ".
		 "AND    notes_val.id_def=notes_def.id ".
		 "AND    notes_val.id_user=".$id_profil." ".
		 "ORDER BY notes_def.id_modul, notes_def.id",
		 $db_link);

  list_disp($q1, $list_note, array("gray", "red", "red2", "red3"), "color");
}

function disp_my_prj_notes($id_profil, $id_projet)
{
  global $db_link;

  $list_note = array(
		    array("value",		"Note",		"text", ACL_ALL,
			  0, array("center")),
		    array("moy_gen",		"Promo",	"text", ACL_ALL,
			  0, array("center")),
		    array("id",		"Classement",	"link",	ACL_ALL,
			  array("_id_page" => 74), array("center"), "_info_id"),
		    array("id",		"Liste",	"link",	ACL_ALL,
			  array("_id_page" => 75), array("center"), "_info_id"),
		    );

  $q1 = db_query("SELECT notes_def.id, ".
		 "       notes_def.valid, TRUNCATE(notes_val.value, 2) AS value, ".
		 "       TRUNCATE(notes_def.moy_gen, 2) AS moy_gen, ".
		 "       (notes_val.value<10)+2*(notes_val.value<notes_def.moy_gen) AS color ".
		 "FROM   notes_def, notes_val ".
		 "WHERE  notes_def.valid>0 ".
		 "AND    notes_val.id_def=notes_def.id ".
		 "AND    notes_val.id_user=".$id_profil." ".
		 "AND    notes_def.link_type=".NOTES_LNK_PRJ." ".
		 "AND    notes_def.link_id=".$id_projet." ".
		 "ORDER BY notes_def.id_modul, notes_def.id",
		 $db_link);

  list_disp($q1, $list_note, array("gray", "red", "red2", "red3"), "color");
}

?>

