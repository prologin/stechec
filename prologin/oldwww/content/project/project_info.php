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

    Copyright ACU EPITA

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>
	* Jacob Olivier <jacob_o@epita.fr>
	* Hesling Fabrice <fabecc@epita.fr>
*/

include "include/download.php";
include "include/project.php";
include "include/notes.php";

function disp_projet($id_projet)
{
  global $db_link;
  global $user_profil;

  global $_action;
  global $_entry;

  if ($id_projet <= 0)
    return projet_noselect();

  /* cache les informations au eleve pour les projets non publiques */
  if (access_check(ACL_ELEVE))
    {
      if (db_num_rows(db_query("SELECT id FROM projet WHERE id=".$id_projet.
			       " AND promo=".$user_profil["promo"].
			       " AND date_distrib<NOW() AND id_statut>1")) < 1)
	{
	  style_text("Ces informations ne sont pas disponibles.");
	  return;
	}
    }

  /* droits d'acces conditionnels */
  $access = db_num_rows(db_query("SELECT * FROM prj_resp ".
				 "WHERE id_projet=".$id_projet." and ".
				 "id_profil=".$user_profil["id"], $db_link)) > 0
    ? ACL_ALL				/* acces reponsable de projet */
    : ACL_SITEADM | ACL_ASSISTCHEF;	/* acces non responsable */

  /* description des donnees de la table */
  $list = array(
		array("", "Informations generales", "title", ACL_ALL, 0),
		array("nom",		"Nom",			"text",
		      ACL_ALL, ACL_ASSISTCHEF | ACL_SITEADM),
		array("id_type",	"Type",			"prj_typ_enum",
		      ACL_ALL, $access),
		array("id_statut",	"Statut",		"bool",
		      ACL_NOT_ELEVE, ACL_ASSISTCHEF | ACL_SITEADM,
		      array("---", "Elaboration", "Valide")),
		array("", "Mode de rendu", "title", ACL_ALL, 0),
		array("with_upload",	"Rendu Upload",		"checkbox",
		      ACL_ALL | ACL_SITEADM, ACL_ASSIST_ALL | ACL_SITEADM),
		array("", "Dates", "title", ACL_ALL, 0),
		array("date_distrib",	"Date de publication",	"datetime",
		      ACL_ALL, $access),
		array("date_cloture",	"Date de cloture des inscriptions",	"datetime",
		      ACL_ALL, $access),
		array("date_rendu",	"Date de rendu",	"datetime",
		      ACL_ALL, $access),
		array("", "Groupes", "title", ACL_ALL, 0),
		array("max_groupe",	"Nombre de groupes max","text",
		      ACL_ALL, $access),
		array("max_user",	"Nombre de users max","text",
		      ACL_ALL, $access),
		array("Telecharger la liste des participants", "Telechargement", "link",
		      ACL_ASSIST_ALL | ACL_SITEADM, 0, array("_dl_type" => "prj_inscr")),
		array("Telecharger la liste par groupes", "Telechargement", "link",
		      ACL_ASSIST_ALL | ACL_SITEADM, 0, array("_dl_type" => "prj_grp")),
		array("", "Actions", "title", ACL_SITEADM, 0),
		array("Supprimer ce projet", "Suppression", "link",
		      ACL_SITEADM, 0, array("_action" => "remove_projet"),
		      "Voulez vous supprimer ce projet ?")
		);

  $confUpload = array(
		      array("", "Configuration du rendu Upload", "title", 
			    ACL_ASSIST_ALL | ACL_SITEADM, 0),
		      array("max_upload", "Nombre maximum de tarball", "text",
			    ACL_ASSIST_ALL | ACL_SITEADM, $access),
		      array("max_upload_size", "Taille max. de l'upload (Octets)", "text",
			    ACL_ASSIST_ALL | ACL_SITEADM, $access),
		      array("name_regexp", "Regexp (Perl)", "text",
			    ACL_ASSIST_ALL | ACL_SITEADM, $access),
		      );

  $list2 = array(
		array("", "Operations", "title", $access, 0),
		array("Generer les ML pour le projet", "Generation", "link",
		      $access, 0, array("_action" => "projet_gen_ml")),
		array("Supprimer les ML du projet", "Suppression", "link",
		      $access, 0, array("_action" => "projet_del_ml"),
		      "Voulez vous supprimer les mailing listes du projet ?"),
		array("Supprimer les processus inutiles", "Nettoyage", "link",
		      ACL_SITEADM, 0, array("_action" => "projet_clean_ml")),
		array("", "Etat", "title", ACL_ALL, 0),
		array("mlst_comment",	"Description des ML",	"textarea",
		      ACL_ALL, $access),
		array("give_mlst",	"Les ML sont affectees",	"checkbox",
		      ACL_ALL, 0),
		array("mlst_st",	"Statut du processus",	"text",
		      ACL_ALL, 0),
		);

  $list3 = array(
		array("", "Operations", "title", $access, 0),
		array("Generer les CVS pour le projet", "Generation", "link",
		      $access, 0, array("_action" => "projet_gen_cvs")),
		array("Supprimer les CVS du projet", "Suppression", "link",
		      $access, 0, array("_action" => "projet_del_cvs"),
		      "Voulez vous supprimer les CVS etudiants du projet"),
		array("Supprimer les processus inutiles", "Nettoyage", "link",
		      ACL_SITEADM, 0, array("_action" => "projet_clean_ml")),
		array("", "Etat", "title", ACL_ALL, 0),
		array("cvs_give",	"Les CVS sont affectees",	"checkbox",
		      ACL_ALL, 0),
		array("cvs_st",		"Statut du processus",	"text",
		      ACL_ALL, 0),
		);


  $list5 = array(
		array("", "Operations", "title", $access, 0),
		array("Generer le CVS de gestion", "Generation", "link",
		      $access, 0, array("_action" => "projet_gen_cvs_mgr")),
		array("Mettre a jour les fichier depuis le CVS de gestion", "Mise a jour", "link",
		      $access, 0, array("_action" => "projet_cvs_mgr_fetch")),
		array("Supprimer le CVS de gestion", "Suppression", "link",
		      ACL_ASSISTCHEF | ACL_SITEADM, 0, array("_action" => "projet_del_cvs_mgr",),
		      "Voulez vous supprimer le CVS de gestion du projet ?"),
		array("Supprimer les processus inutiles", "Nettoyage", "link",
		      ACL_SITEADM, 0, array("_action" => "projet_clean_ml")),
		array("", "Etat", "title", ACL_ALL, 0),
		array("cvs_mgr_give",	"Le CVS est cree",	"checkbox",
		      ACL_ALL, 0),
		array("cvs_mgr_st",	"Statut du processus de creation/suppression",
		      "text", ACL_ALL, 0),
		array("cvs_mgr_fetch_st",	"Statut du processus de mise a jour des fichiers",
		      "text", ACL_ALL, 0),
		);

  /* liste pour la visibilite des traces */
  $list6 = array(array("", "Visibilité des traces", "title", ACL_ASSIST_ALL | ACL_SITEADM, 0));

  $trace_query = db_query("SELECT num_trace, visibility".
                          " FROM trace_list".
                          " WHERE id_proj=$id_projet".
                          " ORDER BY num_trace");

  $bd_trace = array();
  $trace_path = PRJ_DATA_PATH.$id_projet."/traces/";
  while ($row_trace = db_fetch_row($trace_query))
    {
      if (!is_dir($trace_path . "t" . $row_trace[0]))
         continue;
        
      $bd_trace[$row_trace[0]] = $row_trace[1];

      if (!is_file($trace_path . "t" . $row_trace[0] . "/desc"))
        $trace_title = "<B>".$row_trace[0].":</B> Aucune description";
      else
        $trace_title = "<B>".$row_trace[0].":</B> " . 
          file_get_contents_($trace_path . "t" . $row_trace[0] . "/desc");
              
      array_push($list6, array($row_trace[0], $trace_title, "profil", 
                               ACL_ASSIST_ALL | ACL_SITEADM, $access));
    }


  /* effectue l'action requise */
    switch ($_action)
      {
	/*
	 * modification des donnees relatives au projet
	 */
      case ("edit_projet"):

	table_update("projet", $list, "id=".$id_projet);
	break;

      case ("conf_upload"):
	if (!access_check(ACL_ASSIST_ALL | ACL_SITEADM))
	  break;
	project_upload_conf_set($id_projet, 
                                $_POST['_form_max_upload_size'], 
				$_POST['_form_name_regexp'], 
                                $_POST['_form_max_upload']);
	break;

      case ("edit_projet_ml"):
	table_update("projet", $list2, "id=".$id_projet);
	break;

      case ("edit_projet_cvs"):
	table_update("projet", $list3, "id=".$id_projet);
	break;

      case ("edit_projet_cvs_mgr"):
	table_update("projet", $list4, "id=".$id_projet);
	break;

	/* 
	 * suppression d'un projet
	 */
      case ("remove_projet"):
	
	if (access_check(ACL_SITEADM))
	  project_del($id_projet);
	
	break;
	
	/* Gestion des mailing lists */
      case ("projet_gen_ml"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATEMLST.
		 ", '".$id_projet." 0')", $db_link);

	db_query("UPDATE projet SET give_mlst=1, mlst_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);
	
	break;
	
      case ("projet_del_ml"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATEMLST.
		 ", '".$id_projet." 1')", $db_link);

	db_query("UPDATE projet SET give_mlst=0, mlst_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);
	break;

	/* Gestion des cvs etudiants */

      case ("projet_gen_cvs"):

	if (!access_check($access))
	  break;
	
	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATECVS.
		 ", '".$id_projet." 0')", $db_link);

	db_query("UPDATE projet SET cvs_give=1, cvs_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);

	break;
	

      case ("projet_del_cvs"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATECVS.
		 ", '".$id_projet." 1')", $db_link);

	db_query("UPDATE projet SET cvs_give=0, cvs_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);
	break;

	/* Gestion des cvs manager */

      case ("projet_gen_cvs_mgr"):

	if (!access_check($access))
	  break;
	
	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATECVSMGR.
		 ", '".$id_projet." 0')", $db_link);

	db_query("UPDATE projet SET cvs_mgr_give=1, cvs_mgr_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);

	break;
	

      case ("projet_del_cvs_mgr"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATECVSMGR.
		 ", '".$id_projet." 1')", $db_link);

	db_query("UPDATE projet SET cvs_mgr_give=0, cvs_mgr_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);
	break;

      case ("projet_cvs_mgr_fetch"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
		 "VALUES (".$user_profil['id'].", ".TASK_TYP_FETCHCVSMGR.
		 ", '".$id_projet."')", $db_link);

	db_query("UPDATE projet SET cvs_mgr_fetch_id_task=LAST_INSERT_ID() ".
		 "WHERE id=".$id_projet, $db_link);
	break;

      case ("prj_note_add"):

	if (!access_check($access))
	  break;

	db_query("INSERT INTO notes_def (nom, link_id, link_type, promo) ".
		 "SELECT nom, id, ".NOTES_LNK_PRJ.", promo ".
		 "FROM projet WHERE projet.id=".$id_projet, $db_link);

	break;

      case("note_del"):

	if (!access_check($access))
	  break;

	/* recuperation de l'id du projet courant */
	$number = db_fetch_row(db_query("SELECT id FROM notes_def WHERE link_id=".$id_projet,
					$db_link));

	/* Supprime la note du projet */
	db_query("DELETE FROM notes_def ".
		 "WHERE id=".$number[0],
		 $db_link);

	/* Supprime les notes des etudiants */
	db_query("DELETE FROM notes_val ".
		 "WHERE id_def=".$number[0],
		 $db_link);

      /* Gestion de la visibilite des traces du projet */
      case("update_trace_visibility"): 
       
        foreach ($list6 as $row_list6) 
        {
          if (isset($_POST["_form_" . $row_list6[0]]))
          {
            // Tiens a jour la mini-base pour le table_disp_one
            $bd_trace[$row_list6[0]] = $_POST["_form_" . $row_list6[0]];
            // Requete de mise-a-jour
            db_query("UPDATE trace_list".
                   " SET visibility='".addslashes($_POST["_form_" . $row_list6[0]])."'". 
                   " WHERE id_proj=".$id_projet. " AND num_trace=" . $row_list6[0]);
          }
        }
	break;

	/* Mise a jour des cles SSH*/
      case("update_ssh_keys"):
	update_ssh_keys($id_projet, 0);
	break;
      }

  /* requete sql pour afficher
   * les information relatives au projet */
  $query = db_query("SELECT projet.id, projet.nom, projet.id_type, projet.with_upload, ".
		    "       ".db_time("projet.date_distrib", "date_distrib").", ".
		    "       ".db_time("projet.date_cloture", "date_cloture").", ".
		    "       ".db_time("projet.date_rendu", "date_rendu").", ".
		    "       projet.max_groupe, projet.max_user, projet.promo, ".
		    "       projet.give_mlst, projet.cvs_give, projet.cvs_mgr_give, ".
		    "	    projet.id_statut, projet.mlst_comment, ".
		    "       (projet.date_cloture<NOW()) AS clotured, ".

		    "       COALESCE(task_st_enum.libelle, 'Non defini') AS mlst_st, ".
		    "       COALESCE(task_st_enumb.libelle, 'Non defini') AS cvs_st, ".
		    "       COALESCE(task_st_enumc.libelle, 'Non defini') AS cvs_mgr_st, ".
		    "       COALESCE(task_st_enumd.libelle, 'Non defini') AS cvs_mgr_fetch_st ".

		    "FROM projet ".

		    "  LEFT JOIN task ON projet.mlst_id_task=task.id ".
		    "  LEFT JOIN task_st_enum ON task.task_state_id=task_st_enum.id ".

		    "  LEFT JOIN task AS taskb ON projet.cvs_id_task=taskb.id ".
		    "  LEFT JOIN task_st_enum AS task_st_enumb ON taskb.task_state_id=task_st_enumb.id ".

		    "  LEFT JOIN task AS taskc ON projet.cvs_mgr_id_task=taskc.id ".
		    "  LEFT JOIN task_st_enum AS task_st_enumc ON taskb.task_state_id=task_st_enumc.id ".

		    "  LEFT JOIN task AS taskd ON projet.cvs_mgr_fetch_id_task=taskd.id ".
		    "  LEFT JOIN task_st_enum AS task_st_enumd ON taskb.task_state_id=task_st_enumd.id ".

		    "WHERE projet.id=".$id_projet
		    , $db_query);

  /* lit les informations sur le projet */
  $data = db_fetch_array($query);

  /* affiche les infos du projet */
  style_title("Information projet");
  table_disp_one($data, $list, "edit_projet", "_entry");

  /* affiche l'interface de configuration de l'upload */
  if (access_check(ACL_ASSIST_ALL | ACL_SITEADM) && $data['with_upload'])
  {
    $_conf = db_query("SELECT * FROM prj_upload_conf WHERE id_projet=".$id_projet,
		      $db_link);
    $_conf = db_fetch_array($_conf);
    table_disp_one($_conf, $confUpload, "conf_upload", "_entry");
  }

  /* affiche la liste des notes du projet */
  $list_note = array(
		    array("nom",		"Nom",	"text", ACL_ALL),
		    array("mod_nom",		"Module",	"text", ACL_ALL),
		    array("coef",		"Coef",	"text", ACL_ALL,
			  0, array("center")),
		    array("valid",		"Active","bool", ACL_MODUL_EDIT,
			  array("Non", "Oui"), array("center_red", "center_green")),
		    array("id",		"Classement",	"link",	ACL_ALL,
			  array("_id_page" => 74), array("center"), "_info_id"),
		    array("id",		"Liste",	"link",	ACL_ALL,
			  array("_id_page" => 75), array("center"), "_info_id"),
		    array("id",		"Telecharger",	"link",	ACL_NOT_ELEVE,
			  array("_dl_type" => "list_notes"), array("center"), "_entry"),
		    array("id",		"Editer",	"link",	ACL_MODUL_EDIT,
			  array("_id_page" => 72), array("center")),
		    array("id",		"Supprimer",	"link",	ACL_MODUL_EDIT,
			  array("_action" => "note_del"), array("center")),
		    );

  $hide_invalid = access_check(ACL_NOT_ELEVE) ? "" : " AND notes_def.valid>0 ";

  $q1 = db_query("SELECT notes_def.id, notes_def.nom, TRUNCATE(notes_def.coef, 1) AS coef, ".
		 "       notes_def.valid, notes_mod.nom AS mod_nom ".
		 "FROM   notes_def, notes_mod ".
		 "WHERE  notes_def.link_type=".NOTES_LNK_PRJ." ".
		 "AND    notes_def.link_id=".$id_projet." ".
		 $hide_invalid.
		 "AND    notes_def.id_modul=notes_mod.id",
		 $db_link);

  /* affichage de la liste */
  style_title("Notes liees au projet");
  style_links(array(
		    array("Creer une note liee au projet", $access,
			  array("_action" => "prj_note_add"))
		    ));
  list_disp($q1, $list_note, array("gray"), "none");

  /* affiche les info de mailing listes */

  if (intval($data["id_statut"]))
    {    
      style_title("Mailing-listes du projet"); 
      if(intval($data["clotured"]))
	table_disp_one($data, $list2, "edit_projet_ml", "_entry");
      else
	style_text("Les inscriptions ne sont pas terminees");

      style_title("CVS etudiants du projet");
      if(intval($data["clotured"]))
	table_disp_one($data, $list3, "edit_projet_cvs", "_entry");
      else
	style_text("Les inscriptions ne sont pas terminees");
    }

  if (access_check($access))
    {
      style_title("CVS de gestion du projet");
      table_disp_one($data, $list5, "edit_projet_cvs_mgr", "_entry");
    }

  /* Affichage du controle de visibilite des traces */
  if (access_check(ACL_NOT_ELEVE))
  {
    style_title("Traces du projet");
    if (count($bd_trace) == 0)
      style_text("Pas de trace pour le projet");
    else   
      table_disp_one($bd_trace, $list6, "update_trace_visibility", "_entry");
  }
 
  style_title("Responsables du projet");

  switch ($_action)
    {
    case ("add_projet_resp"):

      if (!access_check($access))
	break;

      db_query("INSERT INTO prj_resp (id_projet, id_profil) ".
	       "SELECT ".$id_projet.", user_prof.id ".
	       "FROM   user, user_prof ".
	       "WHERE  user_prof.uid=user.id ".
	       "AND    user_prof.id_profil>1 ".
	       "AND    user_prof.id_profil<8 ".
	       "AND    user_prof.promo=".$data["promo"]." ".
	       "AND    user.login='".addslashes($_POST["_form_resp_login"])."'",
	       $db_link);

      break;
    
    case ("del_projet_resp"):

      if (!access_check($access))
	break;

      db_query("DELETE FROM prj_resp WHERE id_profil=".$_entry.
	       " AND prj_resp.id_projet=".$id_projet);
   }

  /* description des donnees de la table */
  $list4 = array(
		 array("nom",		"Nom",		"text", ACL_ALL),
		 array("prenom",	"Prenom",	"text", ACL_ALL),
		 array("login",		"Login",	"login", ACL_ALL, "@epita.fr"),
		 array("id_profil",	"Fonction",	"profil", ACL_ALL),
		 array("id",		"Retirer",	"link",	$access,
		       array("_action" => "del_projet_resp"), array("center")),
		);

  /* requete sql des responsable du projet */
  $query = db_query("SELECT user.nom, user.prenom, ".
		    "       user.login, user_prof.id, user_prof.id_profil ".
		    "FROM user, user_prof, prj_resp ".
		    "WHERE user_prof.uid=user.id ".
		    "AND   prj_resp.id_profil=user_prof.id ".
		    "AND   prj_resp.id_projet=$id_projet",
		    $db_link);

  /* affichage des infos */
  list_disp($query, $list4, array("gray"), "none");

 
  /* Ne donne acces qu'aux responsables du sujet et aux assistants chef */
  if (access_check($access))
    {
      /* Affichage de la zone: Ajouter un responsable */
      $list5 = array(
		     array("",			"Ajouter un responsable",	"title",
			   ACL_ALL, 0),
		     array("resp_login",	"login",			"text",
			   ACL_ALL, ACL_ALL),
		     array("Updater les cles SSH", "Mise a jour des acces", "link", ACL_ALL, 0, array("_action" => "update_ssh_keys","_entry"))
		     );

      table_disp_one(array("id" => $id_projet), $list5, "add_projet_resp", "_entry");

     
    }
}

/* affiche les infos de la page */
disp_projet($_info_id);

?>

