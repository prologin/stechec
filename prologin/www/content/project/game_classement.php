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

function add_match($id_game, $id_champion, $opt)
{

  db_query("INSERT INTO matchs (id_game, opt_match, type, date)".
           " VALUES ($id_game, \"$opt\", 3, NOW())");

  $id_match = db_insert_id();

  // Insert new entry in competiteur table for each champion
  foreach ($id_champion as $champion)
    db_query("INSERT INTO competiteur (id_champion, id_match)".
             " VALUES ($champion, $id_match)");
}


function add_group_match($id_game)
{
  global $user;

  $nb_matchs = $_POST['_form_sel_nb_matchs'];
  $nb_best = $_POST['_form_sel_nb_champ'];

  //
  // Init tab of champion
  //
  // Select in first the best champion in competition
  $query = db_query("SELECT champion.id, champion.score_tournament AS tscore, ".
		    " CONCAT(user.nom, ' ', user.prenom, ' (',user.nickname, ')') AS candidat,".
		    " champion.name".
           	    " FROM champion, user".
	            " WHERE champion.level = 3".
	            " AND champion.id_game = $id_game".
		    " AND user.id = champion.id_candidat".
		    " AND user.id_profil = 1".
	            " ORDER BY tscore DESC");
  $champ_idx = array();	
  $champ_data = array();

  while (count($champ_data) < $nb_best && $data = db_fetch_array($query)) {
    $champ_idx[] = $data["id"];
    array_push($champ_data,
    	       array("id" => $data["id"], 
    	             "use" => "0",
	             "candidat" => $data["candidat"], 
	             "name" => $data["name"]));
  }

  // If don't have enought champion whe pick on entrenment match and no match
  if (count($champ_data) < $nb_best) {
    echo "Not enough competitors (" . count($champ_data) . "). Taking some from where we can.<br />";
    $query = db_query("SELECT champion.id,".
    		      " champion.score_training AS tscore,".
		      " CONCAT(user.nom, ' ', user.prenom, ' (',user.nickname, ')') as candidat,".
		      " champion.name".
             	      " FROM champion, user".
	              " WHERE champion.level = 3".
	              " AND champion.id_game = $id_game".
		      " AND user.id = champion.id_candidat".
		      " AND user.id_profil = 1".
	              " ORDER BY tscore DESC");

     while (count($champ_data) < $nb_best && $data = db_fetch_array($query)) 
     {
     	if (!in_array($data["id"], $champ_idx)) {
    	  $champ_idx[] = $data["id"];
	  array_push($champ_data, 
    		     array("id" => $data["id"], 
    		           "use" => "0",
		           "candidat" => $data["candidat"], 
		           "name" => $data["name"]));
     	}
     }


  }
    

  //
  // Init table argument
  //
  $args = array();
 
  $query = db_query("SELECT *".
  		   " FROM matchs_opt".
  		   " WHERE id_game = $id_game".
		   " AND use_competition = 1".
		   " AND min <= $nb_best".
		   " AND matchs_opt.id_game = $id_game");
  while ($tmp = db_fetch_array($query))
    array_push($args, $tmp);

  
  //
  // Loop
  //
  $total_match = 0;
  while (count($champ_data) >= 1)
  {
    // Trouver des argument correct
    shuffle($args);
    while (count($args) > 0 && $args[0]["min"] > count($champ_data))
      array_shift ($args);
    if (count($args) <= 0)
      return ;

    // Creer un match
    print "Create a new match (opt = ".$args[0]["opt"].").</br>";
    db_query ("INSERT INTO matchs (id_game, id_createur, opt_match, type, is_competition, nb_champion_instance, date)".
     " VALUES ($id_game, ".$user["id"].",\"".$args[0]["opt"]."\", 3, 1, 1, NOW())");
     echo "Ajout d'un match";
    $total_match++;

    // Get id of new match
    $id_match = db_insert_id();

    // Choose number of champion in match
    $nb_champ = rand($args[0]["min"], min($args[0]["max"], count($champ_data)));

    // Trouver les champions
    $nb_instance = 1;
    $id_team = 10;
    shuffle($champ_data);
    for ($id_champ = $find_champ = 0; $find_champ < $nb_champ; ++$find_champ)
    {
      // Create a competiteur
      //print "+- Champion num ".$champ_data[$id_champ]["id"]."</br>";
      for ($i = 0; $i < $nb_instance; $i++) {
     	db_query ("INSERT INTO competiteur (id_champion, id_match, id_team)".
     		  " VALUES (".$champ_data[$id_champ]["id"].", $id_match, $id_team)");
      }
      $id_team++;

      $champ_data[$id_champ]["use"]++;
      if ($champ_data[$id_champ]["use"] >= $nb_matchs)
	{
	  style_text("Generation de $nb_matchs match(s) pour ".
		     $champ_data[$id_champ]["candidat"]. " avec ".
		     $champ_data[$id_champ]["name"].".");
	  array_splice($champ_data, $id_champ, 1);
	}
      else
	++$id_champ;
    }
  }

  if (count($champ_data) == 1)
    style_text("Generation de ".$champ_data[0]["use"]." match pour ".
	       $champ_data[0]["candidat"]. " avec ".
	       $champ_data[0]["name"].".");

  style_text("Generation de $total_match.");

}

function disp_classement($id_game)
{
  global $_limit;
  style_title("Classement des champions");

  $query = db_query("SELECT champion.id, champion.name, champion.comment,".
                    " champion.level, champion.status, champion.score_tournament AS score,".
                    " user.nickname as concepteur,".
		    " user.id as id_user".
                    " FROM champion, user".
                    " WHERE champion.id_game = $id_game".
                    " AND user.id = champion.id_candidat".
                    " AND champion.level = 3".
		    " ORDER BY score DESC".
                   // " LIMIT ".($_limit * 50).", 50");
                    "");

  $list = array(
              array("concepteur", "Concepteur", "link_generique_info", ACL_ORGA, 
	      	    array("_id_page" => "200"), array("center"), array("_info_id" => "id_user")),
              array("concepteur", "Concepteur", "text", ACL_CANDIDAT, 0,  array("center")),
              array("name", "Nom du champion", "text", ACL_ALL, 0, array("center")),
              array("score", "Score", "text", ACL_ALL, 0, array("center")),
              array("comment", "Commentaire", "text", ACL_ALL, 0, array("center")),
          );

  style_slice($_limit, 10, 50);
  list_disp($query, $list, array("gray", "bluegray"), "even");
  style_slice($_limit, 10, 50);

  
}

/*
** Applique les differentes action
*/
function check_action($id_game)
{
  global $_action;

  switch ($_action)
  {
    case "add_group_match":
      add_group_match($id_game);
      break;
  }
}

function disp_do_some_match($id_game)
{
  style_title("Lancer une phase de selection");

  $list = array(
                array("", "<b>Lancer une phase de selection</b>", "title", ACL_ORGA, 0),
                array("sel_nb_champ", "Selection des X premiers champions", "text", ACL_ORGA, ACL_ORGA),
                array("sel_nb_matchs", "Nombre de match par champion", "text", ACL_ORGA, ACL_ORGA),
               );

  table_disp_one(array(), $list, "add_group_match", "_entry");
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


check_action($_info_id);

if (!access_check(ACL_CANDIDAT))
  disp_do_some_match($_info_id);

disp_classement($_info_id);


?>
