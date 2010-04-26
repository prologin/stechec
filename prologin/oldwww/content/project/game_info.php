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

//include "include/download.php";
//include "include/project.php";
//include "include/notes.php";

include "include/game.php";


function do_action($id_game)
{
  global $_action;
  global $_entry;


  /* effectue l'action requise */
    switch ($_action)
      {
	/*
	 * modification des donnees relatives au projet
	 */
      case ("edit_game"):


	  /* description des donnees de la table */
	  $list = array(
		array("", "Informations generales", "title", ACL_ALL, 0),
		array("nom", "Nom", "text", ACL_ALL, ACL_ORGA),
		array("comment", "Description", "text", ACL_ALL, ACL_ORGA),
		array("status",	"Statut", "bool", ACL_NOT_ELEVE, ACL_ORGA, array("Elaboration", "Valide")),
                      
		array("", "Configuration", "title", ACL_ORGA, 0),
		array("lib_name", "Nom de la lib", "text", ACL_ORGA, ACL_ORGA),
		array("repertoire", "Nom du repertoire", "text", ACL_ORGA, ACL_ORGA),
		array("max_champion", "Nombre maximum de champion", "text", ACL_ALL, ACL_ORGA),
		array("arg_server", "Serveur: argument constant", "text", ACL_ORGA, ACL_ORGA),
                
		array("", "Actions", "title", ACL_ORGA, 0),
		array("Supprimer ce jeu", "Suppression", "link",
		      ACL_ORGA, 0, array("_action" => "remove_game"),
		      "Voulez vous supprimer ce jeu ?")
		);




	table_update("game", $list, "id=".$id_game);
	break;


	/* 
	 * suppression d'un projet
	 */
      case ("remove_game"):
      
	if (access_check(ACL_SITEADM))
	  game_del($id_game);
	break;

      case ("delete_opt"):
      
	db_query ("DELETE FROM matchs_opt WHERE id = $_entry");
        break;

      case ("switch_opt"):

	$data = db_fetch_array(db_query("SELECT use_competition FROM matchs_opt WHERE id = $_entry"));
	$data["use_competition"]++;
	$data["use_competition"] %= 2;
	db_query ("UPDATE matchs_opt".
		  " SET use_competition = ".$data["use_competition"].
		  " WHERE id = $_entry");
	break;

      case ("add_opt"):

        if ($_POST['_form_opt_opt'] == ""
            || $_POST['_form_opt_comment'] == ""
            || $_POST['_form_opt_min'] == ""
            || $_POST['_form_opt_max'] == "")
        {
          style_alert("Tous les champs doivent etre remplit");
          break;
        }
        
        db_query("INSERT INTO matchs_opt (id_game, opt, comment, min, max, use_competition)".
                 " VALUES ($id_game, '".$_POST['_form_opt_opt']."', '".$_POST['_form_opt_comment']."', ".
                           $_POST['_form_opt_min'].", ".$_POST['_form_opt_max'].", ".
                           $_POST['_form_opt_use'].")");
        
        
        break;

      }
}



function disp_game($id_projet)
{
  global $db_link;
  global $user_profil;

  /* description des donnees de la table */
  $list = array(
		array("", "Informations generales", "title", ACL_ALL, 0),
		array("nom", "Nom", "text", ACL_ALL, ACL_ORGA),
		array("comment", "Description", "text", ACL_ALL, ACL_ORGA),
		array("status",	"Statut", "bool", ACL_NOT_ELEVE, ACL_ORGA, array("Elaboration", "Valide")),
                      
		array("", "Configuration", "title", ACL_ORGA, 0),
		array("lib_name", "Nom de la lib", "text", ACL_ORGA, ACL_ORGA),
		array("repertoire", "Nom du repertoire", "text", ACL_ORGA, ACL_ORGA),
		array("max_champion", "Nombre maximum de champion", "text", ACL_ALL, ACL_ORGA),
		array("arg_server", "Serveur: argument constant", "text", ACL_ORGA, ACL_ORGA),
                
		array("", "Actions", "title", ACL_ORGA, 0),
		array("Supprimer ce jeu", "Suppression", "link",
		      ACL_ORGA, 0, array("_action" => "remove_game"),
		      "Voulez vous supprimer ce jeu ?")
		);



  /* requete sql pour afficher
   * les information relatives au projet */
  $query = db_query("SELECT *". 
		    " FROM game ".
		    " WHERE id=".$id_projet);

  /* lit les informations sur le projet */
  $data = db_fetch_array($query);

  /* affiche les infos du projet */
  style_title("Information projet");
  table_disp_one($data, $list, "edit_game", "_entry");


}

/* Print table of differnent option */
function disp_all_opt_match($id_game)
{

  $query = db_query("SELECT *".
                    " FROM matchs_opt".
                    " WHERE id_game=$id_game".
		    " ORDER BY use_competition");

  $list = array(
		array("opt", "Option", "text", ACL_ALL, 0, array("center")),
		array("comment", "Description", "text", ACL_ALL, 0, array("center")),
		array("min", "Nb min player", "text", ACL_ALL, 0, array("center")),
		array("max", "Nb max player", "text", ACL_ALL, 0, array("center")),
		array("use_competition", "En competition", "bool", ACL_ORGA, 
			array("Non", "Oui"), array("center")),
		array("id", "Switch", "link", ACL_ORGA,
                      array("_action" => "switch_opt"), array("center")),
              	array("id", "Delete", "link", ACL_ORGA, 
		      array("_action" => "delete_opt"), array("center")),
	);

  list_disp($query, $list, array("gray", "bluegray"), "even");



}


/* Print table to add option */
function disp_add_opt_match($id_game)
{
  $list = array(
                array("", "<b>Ajout d'une option</b>", "title", ACL_ASSIST_ALL, 0),
                array("opt_opt", "Option effective", "text", ACL_ALL, ACL_ALL),
                array("opt_comment", "Commentaire", "text", ACL_ALL, ACL_ALL),
                array("opt_min", "Nombre minimum de player (0: infinit)", "text", ACL_ALL, ACL_ALL),               
                array("opt_max", "Nombre maximum de player (0: infinit)", "text", ACL_ALL, ACL_ALL),               
		array("opt_use", "En competition", "bool", ACL_ORGA, ACL_ORGA, array("Non", "Oui")),
               );


  table_disp_one(array(), $list, "add_opt", "_entry");


}


if ($_info_id <= 0)
  return game_noselect();

/* cache les informations au eleve pour les projets non publiques */
if (access_check(ACL_CANDIDAT))
  {
    if (db_num_rows(db_query("SELECT id, status FROM game WHERE id=".$_info_id.
                             " AND status=1")) < 1)
      {
        style_text("Ces informations ne sont pas disponibles.");
	return;
      }
  }

do_action($_info_id);

/* affiche les infos de la page */
disp_game($_info_id);

style_title("Gestion des options du serveur");
  
/* Print table of differnent option */
disp_all_opt_match($_info_id);

/* Print table to add option */
disp_add_opt_match($_info_id)

?>

