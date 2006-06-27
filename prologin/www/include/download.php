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
   Changelog:
    03/11/2004:maxime biais: ajout du header "Pragma: public" pour tous les
                             telechargements. Fixe le probleme de dl avec IE.

*/


include "include/config.php";


function print_local_file($file)
{
  if (!is_file($file))
  {
    print("Echec: Can't find file</br>");
    return;
  }
  $fp = fopen ($file, "r");
  print "<font face=\"Courier\" size=\"2\">\n";
  while (!feof ($fp)) 
  {
    $content = fgets( $fp, 4096 );
    echo $content."</br>";
  }
  print "</font>";
  fclose ($fp);	
}

function file_get_contents_($filename)
{
  if(!($fd = fopen($filename, "rb")))
    die("files.php: fopen() error");
  $content = fread($fd, 256);
  fclose($fd);
  return $content;
}

function download_get_mime($fname)
{
  preg_match('/[^.]*\.(.*)/', $fname, $ext);

  switch ($ext[1])
    {
    case ("c"):
    case ("h"):
    case ("txt"):
      return ("text/plain");
    case ("pdf"):
      return ("application/pdf");
    case ("ps"):
      return ("application/postscript");

    case ("tgz"):
    case ("tar.gz"):
      return ("application/x-gzip");
    case ("tar.bz2"):
      return ("application/x-bzip2");
    case ("zip"):
      return ("application/zip");

    default:
      return ("application/x-unknown");
    }
}




function download_prj_access($id_projet)
{
  global $user_profil;
  global $db_link;

  /* cache les informations au eleve pour les projets non publiques */
  if (access_check(ACL_NOT_ELEVE))
    return (1);

  return (db_num_rows(db_query("SELECT id FROM projet WHERE id=".$id_projet.
			       " AND promo=".$user_profil["promo"].
			       " AND date_distrib<NOW() AND id_statut>1", $db_link)) > 0);
}

function download_prj_appendices($path, $_entry)
{
  global $_info_id;
  $id_projet = intval($_info_id);


  if (!is_file($path."/file_list"))
    return (1);

  if (count($file_list = file($path."/file_list")) <= 0)
    return (1);

  for ($i = count($file_list) - 1; $i >= 0; $i--)
    {
      if ($i != $_entry)
	continue;

      if (!preg_match('/^([^:]*):(.*)$/', $file_list[$i], $flist_line))
	return (1);

      if (!is_file($path."/".$flist_line[1]))
	return (1);


      header("Content-type: ".download_get_mime($flist_line[1]));
      header("Content-Disposition: attachment; filename=".$flist_line[1]);
      $filesize = filesize ($path."/".$flist_line[1]);
      header("Content-length: ".$filesize);
      readfile($path."/".$flist_line[1]);

      die;
    }

  return (1);
}

function download_prj_private($_entry)
{
  global $_info_id;
  global $user;
  global $db_link;

  if (isset($_GET["_ids_dl_login"]) && isset($_GET["_ids_dl_project"]))
    $id_projet = $_GET["_ids_dl_project"];
  else
    $id_projet = intval($_info_id);

  if (!download_prj_access($id_projet))
    die ("no access");

  if (isset($_GET["_ids_dl_login"]) && isset($_GET["_ids_dl_project"]))
    $path = PRJ_DATA_PATH.$id_projet."/private/".$_GET["_ids_dl_login"];
  else
    $path = PRJ_DATA_PATH.$id_projet."/private/".$user["login"];

  if (!is_file($path."/files_list"))
    return (1);

  if (count($file_list = file($path."/files_list")) <= 0)
    return (1);

  if (!preg_match('/^([^:]*):(.*)$/', $file_list[$_entry], $flist_line))
    return (1);

  $query = db_query("SELECT with_upload FROM projet WHERE id=".$id_projet);
  $query = db_fetch_array($query);
  if ($query["with_upload"] == 1)
  {
    preg_match('/^([^:]*)-rendu[0-9]+(.*)$/', $flist_line[1], $file_);
    $file_ = $file_[1].$file_[2];
  }
  else
    $file_ = $flist_line[1];


  if (!is_file($path."/".$file_))
    return (1);



  header("Content-type: ".download_get_mime($file_));
  header("Content-Disposition: attachment; filename=".$file_);
  $filesize = filesize ($path."/".$file_);
  header("Content-length: ".$filesize);
  readfile($path."/".$file_);

  die;
}

function download_prj_subject($file, $mime, $dl_file)
{
  global $_info_id;
  global $user;
  global $db_link;

  $id_projet = intval($_info_id);

  if (!is_file($file))
    return (1);

  header("Content-type: ".$mime);
  header("Content-Disposition: attachment; filename=".$dl_file);
  $filesize = filesize ($file);
  header("Content-length: ".$filesize);
  readfile($file);

  die;
}

function download_log_match($id_match)
{
  $data = db_fetch_array(db_query(
            "SELECT game.repertoire, matchs.id ".
            " FROM matchs, game".
            " WHERE matchs.id=$id_match".
            " AND game.id=matchs.id_game"));

  $file = GAME_PATH."/".$data["repertoire"]."/matchs/match_".
          $data["id"]."/visio_".$data["id"].".log";
  $dl_file = "visio_match_".$data["id"].".vlog";
  
  if (!is_file($file))
  {
    print("Echec: Internal Error</br>");
    return (1);
  }

  header("Content-type: text/plain");
  header("Content-Disposition: attachment; filename=".$dl_file);
  $filesize = filesize ($file);
  header("Content-length: ".$filesize);
  readfile($file);

  die;
}

function download_log_server($id_match)
{
  $data = db_fetch_array(db_query(
            "SELECT game.repertoire, matchs.id ".
            " FROM matchs, game".
            " WHERE matchs.id=$id_match".
            " AND game.id=matchs.id_game"));

  $file = GAME_PATH."/".$data["repertoire"]."/matchs/match_".
          $data["id"]."/server.out";
  
  print_local_file($file);
  die;
}


function download_log_champion($id_competiteur)
{
  $data = db_fetch_array(db_query(
            "SELECT game.repertoire, matchs.id".
            " FROM matchs, game, champion, competiteur".
            " WHERE competiteur.id=$id_competiteur".
            " AND matchs.id=competiteur.id_match".
            " AND champion.id=competiteur.id_champion".
            " AND game.id=matchs.id_game"));

  $file = GAME_PATH."/".$data["repertoire"]."/matchs/match_".$data["id"].
            "/client_".$id_competiteur.".out";

  print_local_file($file);
  die;
}


function download_log_compil_champion($id_champion)
{
  $champion_data = db_fetch_array(db_query(
               "SELECT game.repertoire, champion.id,".
	       " champion.id_candidat".
               " FROM game, champion".
               " WHERE champion.id=".$id_champion.
               " AND game.id=champion.id_game"));

  $path = GAME_PATH."/".$champion_data["repertoire"]."/private/".
    "candidat_".$champion_data["id_candidat"]."/";

  $file = "champion_".$champion_data["id"].".log";

  print_local_file($path."/".$file);
  die;
}


function download_code_champion($id_champion)
{
  $champion_data = db_fetch_array(db_query(
               "SELECT game.repertoire, champion.id,".
	       " champion.id_candidat".
               " FROM game, champion".
               " WHERE champion.id=".$id_champion.
               " AND game.id=champion.id_game"));

  if ($user["id_profil"] == '1' && $user["id"] != $champion_data["id_candidat"])
  {
    print("Echec: <b>Nous savons</b>, ne recommence pas !!!</br>");
    return (1);
  }


  $path = GAME_PATH."/".$champion_data["repertoire"]."/private/".
    "candidat_".$champion_data["id_candidat"]."/";

  $file = "champion_".$champion_data["id"].".code";

  print_local_file($path."/".$file);
  die;

}





/*
** Generates an XML trace for each student that has already
** uploaded a tarball. If the project has groups, it generates
** one tarball for each member
** Then it makes a tarball and send it to the client
*/
function download_upload_data($_entry)
{
  global $db_link;
  global $_info_id; // project id

  $uploads = db_query("SELECT * FROM prj_uploads WHERE id_projet=".$_info_id);

  mkdir("/tmp/upload-proj-".$_info_id);
  while ($up = db_fetch_array($uploads))
  {
    $id_eleve = $up['id_eleve'];
    // Get group members logins
    $members = db_query("SELECT user.id AS uid, login, user_prof.id AS intra_id, nom, prenom".
			" FROM user, user_prof, prj_jnt, prj_grp".
			" WHERE user.id = user_prof.uid AND user_prof.id = prj_jnt.id_eleve".
			" AND prj_jnt.id_groupe = prj_grp.id AND prj_jnt.id_projet=".$_info_id.
			" AND prj_grp.id_chef=".$id_eleve
			, $db_link);

    while ($stud = db_fetch_array($members))
    {
      $fd = fopen("/tmp/upload-proj-".$_info_id."/".$stud['login'].".trace", "w");
      fputs($fd, "<?xml version=\"1.0\"?>\n");
      fputs($fd, "<trace>\n<login>".$stud['login']."</login>\n");
      fputs($fd, "<trace_type>Single_v1</trace_type>\n<trace_data>\n");
      fputs($fd, "<before>".$up['nb_uploads_before']."</before>\n");
      fputs($fd, "<after>".$up['nb_uploads_after']."</after>\n");
      fputs($fd, "<total>".$up['nb_uploads']."</total>");
      fputs($fd, "<last_time>".$up['last_upload_time']."</last_time>");
      fputs($fd, "</trace_data>\n</trace>\n");
      fclose($fd);
    }
  }
  $filename = "upload-proj-".$_info_id.".tar";
  // Make a tarball. first tar, and then bzip
  system("tar -cf /tmp/".$filename.
	 " /tmp/upload-proj-".$_info_id."/");
  system("bzip2 /tmp/".$filename);

  // Send the tarball to the client

  header("Content-type: application/x-bzip2");
  header("Content-Disposition: attachment; filename=".$filename.".bz2");
  $filesize = filesize ("/tmp/".$filename.".bz2");
  header("Content-length: ".$filesize);
  readfile("/tmp".$filename.".bz2");

  // clean behind us :)
  //system("rm -rf /tmp/upload-proj-".$_info_id);
  //system("rm -f ".$filename.".bz2");

  die;
}

function download_champion($id_champion)
{
  global $user;
  
  $champion_data = db_fetch_array(db_query("SELECT game.repertoire, champion.id, champion.id_candidat".
                                          " FROM game, champion".
                                          " WHERE champion.id=".$id_champion.
                                          " AND game.id=champion.id_game"));
  
  if ($user["id_profil"] == '1' && $user["id"] != $champion_data["id_candidat"])
  {
    print("Echec: <b>Nous savons</b>, ne recommence pas !!!</br>");
    return (1);
  }


  $path = GAME_PATH."/".$champion_data["repertoire"]."/private/".
    "candidat_".$champion_data["id_candidat"]."/";

  $file = "champion_".$champion_data["id"].".tgz";

  header("Content-type: application/x-gzip");
  header("Content-Disposition: attachment; filename=".$file);
  $filesize = filesize ($path."/".$file);
  header("Content-length: ".$filesize);
  readfile($path."/".$file);

  die;

}

function download($_dl_type, $_entry)
{

  global $_info_id;


  // We get the name in order  to generate more user friendly name for
  // the requested subject.
  $namerow  = db_fetch_row(db_query("SELECT nom, repertoire FROM game WHERE ".
				    "id='$_info_id';"));
  $name = $namerow[0];
  $rep  = $namerow[1];

  switch ($_dl_type)
    {
      /* project downloads */
    case ("subject_txt"):
      return download_prj_subject(
        GAME_PATH."/".$rep."/subject/v".$_entry."/subject.txt",
        "text/plain", "subject-$name.txt");
        
    case ("subject_diff"):
      return download_prj_subject(
        GAME_PATH."/".$rep."/subject/v".$_entry."/diff.txt",
        "text/plain", "diff-$name.txt");
        
    case ("subject_pdf"):
      return download_prj_subject(
        GAME_PATH."/".$rep."/subject/v".$_entry."/subject.pdf",
        "application/pdf", "subject-$name.pdf");

    case ("appendices"):
      return download_prj_appendices(GAME_PATH."/".$rep."/annexe/", $_entry);

    case ("log_match"):
      return download_log_match($_entry);

    case ("log_server"):
      return download_log_server($_entry);

    case ("log_my_champion"):
      return download_log_champion($_entry);

    case ("champion"):
      return download_champion($_entry);

    case ("log_champion"):
      return download_log_compil_champion($_entry);

    case ("code_champion"):
      return download_code_champion($_entry);


/*
    case ("prj_private"):
      return download_prj_private($_entry);

      // note tables downloads 
    case ("list_notes"):
      return download_notes_list($_entry);
    case ("list_notes_m"):
      return download_module_notes_list($_entry);

    case ("user_list"):
      return download_user_list($_entry);

     // project groupe download 
    case ("prj_grp"):
      return download_groupe_list($_entry);
    // project groupe download
    case ("prj_inscr"):
      return download_inscr_list($_entry);
    // project upload data download 
    case ("prj_upload_data"):
      include "include/gen_upload_data.php";
      return dl_upload_data($_info_id, 7);
    //return download_upload_data($_entry);
*/
    default:
      die("Unavailable file");
    }
}


?>
