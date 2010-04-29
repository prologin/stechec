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

function add_match()
{
  global $_info_id;
  global $user;

  // 
  // Check if data is ok
  //
  $one_my = false;
  $id_champion = array();
  $champions = split(":", $_POST["_form_match_champ"]);
  $nb_instance = 1;
  if (is_numeric($_POST["_form_match_instance"]))
    $nb_instance = $_POST["_form_match_instance"];
  if ($nb_instance < 1 || $nb_instance > 10)
    $nb_instance = 1;
  
  foreach ($champions as $champion)
  {
    if ($champion == "")
       continue ;
    // Check if id champion is ok. Must do before INSERT a match
    $res = db_query("SELECT id_candidat FROM champion".
                    " WHERE id=".$champion.
                    " AND (level = 3".
                    "      OR (level = 2 AND id_candidat = ".$user["id"]."))");
    if (db_num_rows($res) <= 0)
      style_alert("Can't add champion: ". $champion);
    else 
    {
      $data = db_fetch_array($res);
      if ($data["id_candidat"] == $user["id"])
        $one_my = true;
      array_push($id_champion, $champion);
    }
  }
  
  if (count($id_champion) == 0)
  {
    style_alert("Pas un seul champion valide.");
    return;
  } else if (access_check(ACL_CANDIDAT) && $one_my == false)
  {
    style_alert("Au moins un des champions doit vous appartenir.");
    return;
  }

  //
  // Insert new entry in matchs table
  //
  db_query("INSERT INTO matchs (id_game, id_createur, opt_match, type,
               is_competition, date, nb_champion_instance)".
           " VALUES ($_info_id, ".$user["id"].",\"".
           $_POST["_form_match_opt"]."\", 3, 0, NOW(), $nb_instance)"); 

  // Get id of new matchs
  $id_match = db_insert_id();

  // Insert new entry in competiteur table for each champion
  $team_id = 1;
  foreach ($id_champion as $champion)
    {
      for ($i = 0; $i < $nb_instance; $i++)
        {
          db_query("INSERT INTO competiteur (id_champion, id_match, id_team)" .
                   " VALUES ($champion, $id_match, $team_id)");
        }
      $team_id++;
    }
}

function show_match($id_match)
{
  style_title("Detail match numero: ". $id_match);

  $data = db_fetch_array(
                db_query("SELECT *, id as log_visio, id as log_server".
                         " FROM matchs".
                         " WHERE matchs.id=$id_match"));

   /* description des donnees de la table */
  $list = array(
	  array("", "Informations sur le match", "title", ACL_ALL, 0),
	  array("id", "Numero", "text", ACL_ALL, 0),
	  array("opt_match", "Option", "text", ACL_ALL, 0),
          array("log_visio", "Log visio", "link", ACL_ALL,
                array("_dl_type" => "log_match",), array("center")),
          array("log_server", "Log server", "link", ACL_ALL,
                array("_dl_type" => "log_server",), array("center")),
	  );

  table_disp_one($data, $list, "", "");

  $data = db_query("SELECT competiteur.score, user.nickname, champion.name,".
                   " competiteur.id as log_champ, competiteur.log". 
                   " FROM competiteur, champion, user".
                   " WHERE competiteur.id_match = ".$id_match.
                   " AND   champion.id = competiteur.id_champion".
                   " AND   user.id = champion.id_candidat".
                   " ORDER BY score DESC");
 
  $list = array(
              array("nickname", "Comcepteur", "text", ACL_ALL, 0, array("center")),
              array("name", "Nom", "text", ACL_ALL, 0, array("center")),
              array("score", "Points acquis", "text", ACL_ALL, 0, array("center")),
              array("log", "Msg server", "text", ACL_ALL, 0, array("center")),
              array("log_champ", "Log du champion", "link", ACL_ALL,
                    array("_dl_type" => "log_my_champion",), array("center")),
              );
 
  list_disp($data, $list, array("gray", "bluegray"), "even");
 
  
}

/*
** Applique les differentes action
*/
function check_action($entry)
{
  global $_action;

  switch ($_action)
  {
    case "add_match":
      add_match();
      break;

    case "show_match":
      show_match($entry);
      break;
  }
}

// FIXME: generer les table pour que l'on selectionne les champions via combo
function table_add_match($id_game)
{
  style_title("Ajouter un match");

  $list = array(
                array("", "<b>Ajout d'un match</b>", "title", ACL_ASSIST_ALL, 0),
                array("match_opt", "Options du match", "text", ACL_ALL, ACL_ALL),
                //array("match_instance", "Nombre d'instance par champion", "text", ACL_ALL, ACL_ALL), // FIXME deather : prologin 2008 : toujours a 1
                array("match_champ", "Champions participant (RegExp: id[:id]*)", "text", ACL_ALL, ACL_ALL),
               );


  table_disp_one(array(), $list, "add_match", "_entry");
}




/*
 *  Affiche la liste des champions pour le jeux en cours
 */
function disp_match_candidat($id_game, $id_candidat)
{
  global $_limit;

  style_title("Mes matchs");

  $query = db_query("SELECT matchs.*, user.nickname as createur".
                    " FROM matchs, competiteur, champion, user".
                    " WHERE matchs.id_game=$id_game".
                    " AND matchs.type=3".  
                    " AND matchs.id=competiteur.id_match".
                    " AND competiteur.id_champion=champion.id".
                    " AND champion.id_candidat=$id_candidat". 
                    " AND user.id = matchs.id_createur".
                    " GROUP BY id".
                    " ORDER BY date DESC, id DESC".
                    " LIMIT ".($_limit * 20).", 20");

  $list = array(
              array("date", "Date", "date", ACL_ALL, 0, array("center")),
              array("id", "Numero", "text", ACL_ALL, 0, array("center")), 
              array("createur", "Createur du match", "text", ACL_ALL, 0, array("center")), 
              array("opt_match", "Option du match", "text", ACL_ALL, 0, array("center")),
              array("id", "Details", "link", ACL_ALL, 
                    array("_action" => "show_match"), array("center")),
              array("is_competition", "Type", "bool", ACL_ALL,
                    array("Entrainement", "Competition"), array("center")),
              array("status", "Etat", "bool", ACL_ALL,
                    array("En attente", "En cours", "Erreur", "Réalisé"), 
                    array("center", "center_green", "center_red", "center_green")),
              );
 

  style_slice($_limit, 10, 20);
  list_disp($query, $list, array("gray", "bluegray"), "even");
  style_slice($_limit, 10, 20);

}

/*
 *  Affiche la liste de tous les champions en competition pour le jeux en cours
 */
function disp_all_match($id_game)
{
  global $user;
  global $_limit;
  style_title("Tous les matchs");

  $query = db_query("SELECT matchs.*, user.nickname as createur".
                    " FROM matchs, user".
                    " WHERE matchs.id_game=$id_game".
                    " AND matchs.type=3". 
                    " AND user.id = matchs.id_createur".
                    " ORDER BY date DESC, id DESC".
                    " LIMIT ".($_limit * 20).", 20");

  $list = array(
              array("date", "Date", "date", ACL_ALL, 0, array("center")),
              array("id", "Numero", "text", ACL_ALL, 0, array("center")), 
              array("createur", "Createur du match", "text", ACL_ALL, 0, array("center")), 
              array("opt_match", "Option du match", "text", ACL_ALL, 0, array("center")),
              array("id", "Details", "link", ACL_ALL, 
                    array("_action" => "show_match"), array("center")),
              array("is_competition", "Type", "bool", ACL_ALL,
                    array("Entrainement", "Competition"), array("center")),
              array("status", "Etat", "bool", ACL_ALL,
                    array("En attente", "En cours", "Erreur", "Réalisé"), 
                    array("center", "center_green", "center_red", "center_green")),
              );
 

  style_slice($_limit, 10, 100);
  list_disp($query, $list, array("gray", "bluegray"), "even");
  style_slice($_limit, 10, 100);

}


function disp_champion_competition($id_game)
{
  global $user;
//  global $_limit2;

  $list = array(
              array("nickname", "Concepteur", "text", ACL_ALL, 0, array("center")),
              array("id", "Numero", "text", ACL_ALL, 0, array("center")),
              array("name", "Nom", "text", ACL_ALL, 0, array("center")),
              array("score_training", "Score", "text", ACL_ALL, 0, array("center")),
              array("comment", "Commentaire", "text", ACL_ALL, 0, array("center")),
          );
  

  // Les miens
  style_title("Tous mes champions");

  $query = db_query("SELECT user.nickname, champion.* ".
                    " FROM champion, game, user ".
                    " WHERE game.id=$id_game".
                    " AND champion.id_game=game.id".
                    " AND user.id=champion.id_candidat".
                    " AND (champion.level=2 OR champion.level=3)".
                    " AND user.id = ".$user["id"].
                    " ORDER BY nickname, score_training DESC");
  
  
  list_disp($query, $list, array("gray", "bluegray"), "even");

  // Les autres
  style_title("Tous les champions contre moi");

  $query = db_query("SELECT user.nickname, champion.* ".
                    " FROM champion, game, user ".
                    " WHERE game.id=$id_game".
                    " AND champion.id_game=game.id".
                    " AND user.id=champion.id_candidat".
                    " AND champion.level=3".
                    " AND user.id <> ".$user["id"].
                    " ORDER BY nickname, score_training DESC");
//                    " LIMIT ".($_limit2 * 30).", 30");
  
  
  list_disp($query, $list, array("gray", "bluegray"), "even");


}


//fabecc: Print message to final instruction
//todelete
function disp_message()
{
  style_title("Instruction de rendu");
  style_text("Pour valider votre rendu et donc faire partie du classement du fabuleux concours national d'informatique, il vous faut remplir les conditions suivantes:");
  print "<ul class=text>\n".
        "  <li>avoir <b>un et un seul champion uploade</b> sur le site. Celui-ci doit deja <b>etre en mode competition</b></li>\n".
	"  <li>fournir avec votre rendu <b>un fichier README</b> qui decrit le fonctionnement de votre algorithme/code/strategie. Le fichier README n'est pas automatiquement inclus dans la tarball lors du \"make tar\", vous pouvez rajouter dans le Makefile le fichier README a la suite des sources (Exemple: \"SRC = prologin.c README\") puis faite un make tar et uploade le champion.</li>\n".
        "</ul>\n"
   ;
}
//!fabecc

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


check_action($_entry);

//if (access_check(ACL_CANDIDAT))
//  disp_message();

if (access_check(ACL_CANDIDAT))
  disp_match_candidat($_info_id, $user["id"]);
else
  disp_all_match($_info_id);

table_add_match($_info_id);

disp_champion_competition($_info_id); 



?>
