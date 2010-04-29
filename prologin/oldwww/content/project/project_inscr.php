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

include "include/project.php";
include "include/upload.php";

function disp_concerne($id_projet)
{
  global $db_link;
  global $_limit;
  global $user_profil;
  global $_action;
  global $_entry;

  if ($id_projet <= 0)
    return projet_noselect();

  if (!$_limit)
    $_limit = ord("A");

  /* droits d'acces conditionnels */
  $access = db_num_rows(db_query("SELECT * FROM prj_resp ".
				 "WHERE id_projet=".$id_projet." and ".
				 "id_profil=".$user_profil["id"], $db_link)) > 0
    ? ACL_ALL				/* acces reponsable de projet */
    : ACL_SITEADM | ACL_ASSISTCHEF;	/* acces non responsable */

  switch ($_action)
    {
    case ("add_concern_all"):
      
      if (!access_check($access))
	break;
      
      /* supprime les eventuelles anciennes infos de groupe
       * d'un projet deja valide puis invalide par SITEADM */
      project_del_groups($id_projet);
      project_del_jointure($id_projet);
      
      /* creation de la jointure pour affecter
       * le projet a chaque eleve de la promotion */
      db_query("INSERT INTO prj_jnt (id_projet, id_eleve) ".
	       "SELECT projet.id, user_prof.id ".
	       "FROM projet, user_prof ".
	       "WHERE projet.id=".$id_projet." ".
	       "AND   user_prof.promo=projet.promo ".
	       "AND   user_prof.id_profil=1 ".
	       "AND   user_prof.statut<2",
	       $db_link);

      /* les projet en groupe de 1 user sont deja inscrit */
      db_query("UPDATE prj_jnt, projet ".
	       "SET prj_jnt.etape=".PRJ_STG_INSCRIT." ".
	       "WHERE projet.id=".$id_projet." ".
	       "AND   prj_jnt.id_projet=projet.id ".
	       "AND   projet.max_user=1 ",
	       $db_query);

      break;

    case ("add_concern"):

      if (!access_check($access))
	break;

      /*
       *  FIXME: un etudiant peut etre ajoute plusieurs fois
       */
      db_query("INSERT INTO prj_jnt (id_projet, id_eleve, prj_jnt.etape) ".
	       "SELECT projet.id, user_prof.id, ".
	       /* etudiant deja inscrit si groupe de 1 */
	       "       IF(projet.max_user=1, ".PRJ_STG_INSCRIT.", ".PRJ_STG_NONINSCRIT.") ".
	       "FROM projet, user_prof, user ".
	       "WHERE projet.id=".$id_projet." ".
	       "AND   user_prof.promo=projet.promo ".
	       "AND   user_prof.id_profil=1 ".
	       "AND   user_prof.uid=user.id ".
	       "AND   user.login BETWEEN '".addslashes($_POST["_form_new_login"])."'".
	       " AND '".addslashes($_POST["_form_new_login_end"])."'",
	       $db_link);

      break;

    case ("del_concern"):

      if (!access_check($access))
	break;

      db_query("DELETE FROM prj_jnt ".
	       "WHERE prj_jnt.id_projet=".$id_projet." ".
	       "AND   prj_jnt.id_eleve=".$_entry." ".
	       "AND   prj_jnt.id_groupe=0",
	       $db_link);

      break;

    case ("flush_concern"):

      if (!access_check($access))
	break;

      project_del_groups($id_projet);
      project_del_jointure($id_projet);

      break;

      /*
       * Suppression des etudiants non-inscrits de la liste des participants 
       */
    case ("flush_non_concern"):

      if (!access_check($access))
	break;

      db_query("DELETE FROM prj_jnt ".
               "WHERE id_projet=".$id_projet." ".
               "AND etape=".PRJ_STG_NONINSCRIT,
	       $db_link);

      break;

      /*
       * Ajout de participants suivant un fichier texte contenant les logins
       */
    case("add_concern_file"):

      if (!access_check($access))
	break;

      $filename = $_FILES['_form_add_single_file']['tmp_name'];

      $fd = fopen($filename, "r");
      if ($fd == 0)
      {
        print("Impossible d'ouvrir le fichier envoye<br/>");
        break;
      }
	/*
	 *  FIXME: un etudiant peut etre ajoute plusieurs fois
	 */
      $request="INSERT INTO prj_jnt (id_projet, id_eleve, prj_jnt.etape) ".
	"SELECT projet.id, user_prof.id, ".
	"       IF(projet.max_user=1, ".PRJ_STG_INSCRIT.", ".PRJ_STG_NONINSCRIT.") ".
	"FROM projet, user_prof, user ".
	"WHERE projet.id=".$id_projet." ".
	"AND   user_prof.promo=projet.promo ".
	"AND   user_prof.id_profil=1 ".
	"AND   user_prof.uid=user.id ".
	"AND (";

      if ($entry = fgets($fd, 2048))
	$request .= "user.login='".trim($entry)."'";
      else
      {
	print("Le fichier de logins est vide<br/>");
	break;
      }
      while ($entry = fgets($fd, 2048))
	$request .= " OR user.login='".trim($entry)."'";
      $request .= ")";

      db_query($request, $db_link);

      break;

      /*
       * Suppression de participants suivant un fichier texte contenant les logins
       * 
       * Si chef de groupe :
       *    - autres membres retournent en NON_INSCRIT
       *    - effacement du groupe
       *    - effacement du chef des participants
       * SINON :
       *    - effacement de l'eleve des participants
       */
    case("del_concern_file"):

      if (!access_check($access))
	break;

      $filename = $_FILES['_form_del_single_file']['tmp_name'];

      $fd = fopen($filename, "r");
      if ($fd == 0)
      {
        print("Impossible d'ouvrir le fichier envoye<br/>");
        break;
      }

      while ($entry = fgets($fd, 2048))
      {
	$res = db_query("SELECT prj_grp.id_chef FROM prj_grp, user_prof, user ".
			"WHERE prj_grp.id_projet=".$id_projet." ".
			"AND prj_grp.id_chef=user_prof.id ".
			"AND user_prof.uid=user.id ".
			"AND user.login='".trim($entry)."'",
			$db_link);

	if (db_num_rows($res) > 0)
	{
	  $row = db_fetch_row($res);
	  $id_chef=$row[0];  // id du chef du groupe

	  $res = db_query("SELECT id_groupe FROM prj_jnt ".
			  "WHERE id_projet=".$id_projet." ".
			  "AND id_eleve=".$id_chef,
			  $db_link);
	  $row = db_fetch_row($res);
	  $id_grp=$row[0];  // id du groupe

	  /* Passage en NON_INSCRIT des autres membres du groupe */
	  db_query("UPDATE prj_jnt ".
		   "SET id_groupe=0, etape=".PRJ_STG_NONINSCRIT." ".
		   "WHERE id_projet=".$id_projet." ".
		   "AND id_groupe=".$id_grp,
		   $db_link);

	  /* Suppression du groupe */
	  db_query("DELETE FROM prj_grp ".
		   "WHERE id_projet=".$id_projet." ".
		   "AND id_chef=".$id_chef,
		   $db_link);

	  /* Suppression de l'eleve Chef du projet */
	  db_query("DELETE FROM prj_jnt ".
		   "WHERE id_projet=".$id_projet." ".
		   "AND   id_eleve=".$id_chef,
		   $db_link);
	}
	else
	{
	  /* Suppression de l'eleve */
	  db_query("DELETE FROM prj_jnt ".
		   "WHERE id_projet=".$id_projet." ".
		   "AND   id_eleve=".
		   "  (SELECT user_prof.id from user_prof, user".
		   "  WHERE user_prof.uid = user.id".
		   "  AND user.login='".trim($entry)."')",
		   $db_link);
	}
      }

      break;

    }

  /* description des donnees de la table */
  if (access_check($access))
    {
      style_title("Administration de la liste");

      // admin normale
      $list2 = array(
		     array("", "<b>Operations</b>", "title", ACL_ALL, 0),
		     array("Ajouter toute la promo", "Ajout global", "link",
			   ACL_ALL, 0, array("_action" => "add_concern_all")),
		     array("Vider la liste des etudiants concernes", "Suppression", "link",
			   ACL_ALL, 0, array("_action" => "flush_concern"),
			   "Voulez vous supprimer tous les etudiants concernes par le projet ?"),
		     array("Effacer les etudiants non-inscrits", "Suppression des non-inscrits",
			   "link", ACL_ALL, 0, array("_action" => "flush_non_concern"),
			   "Voulez vous supprimer tous les etudiants non-inscrits au projet ?"),
		     array("", "<b>Ajouter une plage d'etudiants</b>", "title", ACL_ALL, 0),
		     array("new_login", "Debut intervalle (login)",	"text",
			   ACL_ALL, ACL_ALL),
		     array("new_login_end", "Fin intervalle (login)",	"text",
			   ACL_ALL, ACL_ALL),
		     );
      table_disp_one(array("id" => $id_projet), $list2, "add_concern", "_entry");

      $list2 = array(
		     array("", "<b>Ajout de participants depuis un fichier</b>",
			   "title", ACL_ALL, 0),
		     array("add_single_file", "Fichier de logins", "upload",
			   ACL_ALL, ACL_ALL),
		     );
      table_disp_one(array("id" => $id_projet), $list2, "add_concern_file", "_entry");

      $list2 = array(
		     array("", "<b>Suppression de participants depuis un fichier</b>",
			   "title", ACL_ALL, 0),
		     array("del_single_file", "Fichier de logins", "upload",
			   ACL_ALL, ACL_ALL),
		     );
      table_disp_one(array("id" => $id_projet), $list2, "del_concern_file", "_entry");
    }

  style_title("Individus concernes par le projet");

  $list = array(
		array("nom",		"Nom",		"link_info", ACL_ALL, 200),
		array("login",		"Login",	"login", ACL_ALL, "@epita.fr"),
		array("etape",		"Inscription",	"prj_stg_enum", ACL_NOT_ELEVE,
		      0, array("", "center_red", "center_yellow", "center_green")),
		array("pid",		"Retirer",	"link",	$access,
		      array("_action" => "del_concern"), array("center")),
		);

  /* requete sql des groupes du projet */
  $query = db_query("SELECT CONCAT(user.nom, ' ', user.prenom) AS nom, prj_jnt.etape, ".
		    "       user.login, user.id, ".
		    "       IF(prj_jnt.id_groupe, '', user_prof.id) AS pid ".
		    "FROM user, user_prof, prj_jnt ".
		    "WHERE user_prof.uid=user.id ".
		    "AND   prj_jnt.id_eleve=user_prof.id ".
		    "AND   prj_jnt.id_projet=".$id_projet." ".
		    "AND ASCII(UCASE(user.nom))=".$_limit." ".
		    "ORDER BY user.nom",
		    $db_link);

  style_slice_alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ", $_limit);

  /* affichage des infos */
  list_disp($query, $list, array("bluegray", "gray"), "even");

  style_slice_alpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ", $_limit);
}

/* affiche les infos de la page */
disp_concerne($_info_id);

?>

