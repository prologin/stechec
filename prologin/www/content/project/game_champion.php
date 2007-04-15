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

include "include/game.php";
include "include/download.php";

function makeDirs($path, $mode = 0777)
{
  return is_dir($path) or 
    (makeDirs(dirname($path), $mode) and mkdir ($path, $mode));
}

/*
** Applique les differentes action
*/
function check_action($entry)
{
  global $_action;
  global $_info_id;
  global $user;

  switch ($_action)
  {
    case "delete_champion":
      // FIXME: do a best clean 
      // Ca chie moux. Il faut vider les matchs correspondant si
      // plus aucun competiteur ne pointe dessus. Au passage un
      // clean des fichiers est bien.
      db_query("DELETE FROM competiteur WHERE id_champion=".$entry);
      db_query("DELETE FROM champion WHERE id=".$entry);
      break;

    case "switch_mode_champion":
      $res = db_fetch_array(db_query("SELECT level FROM champion WHERE id=".$entry));
      if ($res["level"] != 2 && $res["level"] != 3)
       {
        style_alert("Le champion doit être au niveau \"valide\" ou \"championnat\" pour être switché");
        break;
      }
      else
      {
        $res["level"] += ($res["level"] == 2 ? 1 : -1);
        db_query("UPDATE champion SET level=".$res["level"]." WHERE id=".$entry);
      }

      break;
  
    case "add_champion":

      // Verification des champs et BD
      if ($_POST['_form_champ_name'] == "" 
          || $_POST['_form_champ_comment'] == "")
      {
        style_alert("Tous les champs doivent <EA>tre remplit");
        break;
      }

      // Verifier le nombre de champion
      $game = db_fetch_array(db_query("SELECT repertoire, max_champion".
                                      " FROM game".
                                      " WHERE id=".$_info_id));
      
      $res = db_query("SELECT id".
                      " FROM champion".
                      " WHERE id_candidat=".$user["id"]);
      if (access_check(ACL_CANDIDAT) && $game["max_champion"] <= db_num_rows($res))
      {
         style_alert("Vous avez trop de champion. Il faut en supprimer.");
         break;
      }


      // Requete d'insertion dans la base
      $result = db_query("INSERT INTO champion (id_candidat, id_game, name, comment)".
                         " VALUES(".$user["id"].", $_info_id, '".$_POST['_form_champ_name']."',".
                         "        '".$_POST['_form_champ_comment']."')");

      // Save tarball
      $id_champ = db_insert_id();
				      
      $champion_path = GAME_PATH."/".$game["repertoire"]."/private/candidat_".$user["id"];
      makeDirs($champion_path, 0777);
      chmod($champion_path, 0777);
      move_uploaded_file($_FILES["_form_champ_tarball"]["tmp_name"], 
        $champion_path."/champion_".$id_champ.".tgz"); 

      if (!is_file($champion_path."/champion_".$id_champ.".tgz"))
      {
      	style_alert("Echec: Problem with your tarball");
	db_query("DELETE FROM champion WHERE id=$id_champ");
	return;
      }
	
      chmod($champion_path."/champion_".$id_champ.".tgz", 0777);

      // Create match of compilation
      db_query("INSERT INTO matchs (id_game, status, type, is_competition)".
               " VALUES($_info_id, 1, 1, 0)");
      $id_match = db_insert_id();
                                       
      db_query("INSERT INTO competiteur (id_champion, id_match)".
               " VALUES($id_champ, $id_match)");
      db_query("UPDATE matchs SET status=0 WHERE id=$id_match");

    break;
  }
}

function add_champion($id_game, $id_candidat)
{
  style_title("Ajouter un champion");


  $list = array(
                array("", "<b>Ajout d'un champion</b>", "title", ACL_ASSIST_ALL, 0),
                array("champ_name", "Nom du champion", "text", ACL_ALL, ACL_ALL),
                array("champ_comment", "Commentaire sur le champion", "text", ACL_ALL, ACL_ALL),
                array("champ_tarball", "Chemin tarball", "upload", ACL_ALL, ACL_ALL),               
               );


  table_disp_one(array(), $list, "add_champion", "_entry");
}

/*
 *  Affiche la liste des champions pour le jeux en cours
 */
function disp_champion_candidat($id_game, $id_candidat)
{
  global $_limit;

  style_title("Mes champions");

  $query = db_query("SELECT champion.*, SUM(competiteur.score) AS score".
                    " FROM champion, competiteur".
                    " WHERE champion.id_candidat = $id_candidat".
                    " AND champion.id_game = $id_game".
                    " AND competiteur.id_champion = champion.id".
                    " GROUP BY champion.id".
		    " ORDER BY champion.level, score DESC".
                    " LIMIT ".($_limit * 20).", 20");


  style_slice($_limit, 10, 20);
  list_disp($query,
            array(
              array("name", "Nom", "text", ACL_ALL, 0, array("center")),
              array("score", "Score", "text", ACL_ALL, 0, array("center")),
              array("comment", "Commentaire", "text", ACL_ALL, 0, array("center")),
              array("level", "Avancement", "bool", ACL_ALL, 
                    array("Nouveau", "Compilable", "Validé", "Compétition", 
                          "Fonction interdite", "Non compilable", "Erreur" ), 
                    array("center", "center_green","center_green","center_green",
                          "center_red","center_red","center_red")),
              array("id", "Log", "link", ACL_ALL, array("_dl_type" => "log_champion")), array("center"),
              array("id", "Download", "link", ACL_ALL, array("_dl_type" => "champion")), array("center"),
              array("id", "Delete", "link", ACL_ALL, array("_action" => "delete_champion"), array("center")),
              array("id", "Switch", "link", ACL_ALL, array("_action" => "switch_mode_champion"), array("center")),
              ),
            array("gray", "bluegray"), "even");
  style_slice($_limit, 10, 20);
            
}

/*
 *  Affiche la liste de tous les champions pour le jeux en cours
 */
function disp_all_champion($id_game)
{
  global $_limit;

  style_title("Tous les champions");

  $query = db_query("SELECT champion.id, champion.name, champion.comment,".
                    " champion.level, champion.status,".
                    " SUM(competiteur.score) AS score,".
                    " user.nickname as concepteur,".
		    " user.id as id_user".
                    " FROM champion, user, competiteur".
                    " WHERE champion.id_game=$id_game".
                    " AND user.id=champion.id_candidat".
                    " AND competiteur.id_champion = champion.id".
                    " GROUP BY champion.id".
		    " ORDER BY user.nickname, champion.level, score DESC".
                    //" LIMIT ".($_limit * 20).", 20");
                    "");
		    

  style_slice($_limit, 10, 20);
  list_disp($query, 
            array(
              array("concepteur", "Concepteur", "link_generique_info", ACL_ALL, 
	      	    array("_id_page" => "200"), array("center"), array("_info_id" => "id_user")),
              array("name", "Nom", "text", ACL_ALL, 0, array("center")),
              array("score", "Score", "text", ACL_ALL, 0, array("center")),
              array("comment", "Commentaire", "text", ACL_ALL, 0, array("center")),
              array("level", "Avancement", "bool", ACL_ALL, 
                    array("Nouveau", "Compilable", "Validé", "Compétition", 
                          "Fonction interdite", "Non compilable", "Erreur" ),
                    array("center", "center_green","center_green","center_green",
                          "center_red","center_red","center_red")),
              array("id", "Log", "link", ACL_ALL, array("_dl_type" => "log_champion")), array("center"),
              array("id", "Code", "link", ACL_ALL, array("_dl_type" => "code_champion")), array("center"),
              array("id", "Download", "link", ACL_ALL, array("_dl_type" => "champion")), array("center"),
              array("id", "Delete", "link", ACL_ALL, array("_action" => "delete_champion"), array("center")),
              array("id", "Switch", "link", ACL_ALL, array("_action" => "switch_mode_champion"), array("center")),
              ),
            array("gray", "bluegray"), "even");
  style_slice($_limit, 10, 20);
 
}

if ($_info_id <= 0)
  return game_noselect();

if (access_check(ACL_CANDIDAT))
  {
    if (db_num_rows(db_query("SELECT id, status".
                             " FROM game".
                             " WHERE id=".$_info_id.
                             " AND status=1")) < 1)
      {
        style_text("Ces informations ne sont pas disponibles.");
        return;
      }
  }


//style_title("Attention");
//style_text("Bonjour, phase de test en cours. Il faut attendre que tous les matchs".
//	   " de competition soient effectuées avant que vos nouveaux champions soient utilisable");


check_action($_entry);

add_champion($_info_id, $user[id]);

if (access_check(ACL_CANDIDAT))
  disp_champion_candidat($_info_id, $user[id]);
else 
  disp_all_champion($_info_id);



?>
