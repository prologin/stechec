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

include "include/download.php";
include "include/game.php";

function disp_subjects($id_projet, $path)
{
  $sub_ver = intval(file_get_contents_($path."/current_version"));
  
  if ($sub_ver <= 0)
  {
    style_text ("Pas encore de sujet pour le jeu.");
    return;
  }


  $list = array(
		array("ver",		"Version",	"text", ACL_ALL,
		      0, array("center")),
		array("txt_id",		"Format texte",	"link", ACL_ALL,
		      array("_dl_type" => "subject_txt",),  array("center")),
		array("diff_id",	"Diff texte",	"link", ACL_ALL,
		      array("_dl_type" => "subject_diff",),  array("center")),
		array("pdf_id",		"Format PDF",	"link", ACL_ALL,
		      array("_dl_type" => "subject_pdf",),  array("center")),
		);

  for ($i = 1; $i <= $sub_ver; $i++)
    {
      $line["ver"] = $i;

      if (is_file($path."/v".$i."/diff.txt"))
        $line["diff_id"] = $i;

      if (is_file($path."/v".$i."/subject.txt"))
        $line["txt_id"] = $i;
      
      if (is_file($path."/v".$i."/subject.pdf"))
        $line["pdf_id"] = $i;
      
      $table[] = $line;
    }
  list_disp_data($table, $list, array("gray", "bluegray"), "even");
}



function disp_appendices($id_projet, $path)
{
  if (count($file_list = file($path."/file_list")) <= 0)
  {
    style_text("Pas d'annexe pour le jeu.");
    return;
  }

  $list = array(
		array("name",		"Fichier",	"link_info", ACL_ALL,
		      array("_dl_type" => "appendices",), 0, "_entry"),
		array("desc",		"Description",	"text", ACL_ALL),
		);

  for ($i = count($file_list) - 1; $i >= 0; $i--)
    {
      if (!preg_match('/^([^:]*):(.*)$/', $file_list[$i], $flist_line))
	continue;

      if (!is_file($path."/".$flist_line[1]))
	continue;

      $line["id"] = $i;
      $line["name"] = $flist_line[1];
      $line["desc"] = $flist_line[2];

      $table[] = $line;
    }

  if (count($table))
    list_disp_data($table, $list, array("gray", "bluegray"), "even");
  else
    style_text("Pas d'annexe pour le jeu.");
}



/*
function disp_my_privates($path)
{
  global $user;

  $list = array(
		array("name",		"Fichier",	"link_info", ACL_ALL,
		      array("_dl_type" => "prj_private",), 0, "_entry"),
		array("desc",		"Description",	"text", ACL_ALL),
		);

  $table = array();

  $flist_name = $path."/".$user["login"]."/files_list";

  if (!is_file($flist_name))
    return;

  if (count($file_list = file($flist_name)) <= 0)
    return;

  for ($i = count($file_list) - 1; $i >= 0; $i--)
    {
      if (!preg_match('/^([^:]*):(.*)$/', $file_list[$i], $flist_line))
	continue;

      if (!is_file($path."/".$user["login"]."/".$flist_line[1]))
	continue;

      $line["name"] = $flist_line[1];
      $line["desc"] = $flist_line[2];
      $line["id"] = $i;

      array_push($table, $line);
    }

  if (count($table))
    {
      style_title("Mes fichiers");
      list_disp_data($table, $list, array("gray", "bluegray"), "even");
    }
}
*/


function disp_files($id_projet)
{
  global $user_profil;

  if ($id_projet <= 0)
    return game_noselect();

  /* cache les informations au eleve pour les projets non publiques */

  $res = db_fetch_array(db_query("SELECT id, repertoire, status FROM game WHERE id=".$id_projet));

  if (access_check(ACL_CANDIDAT) && $res["status"] == 0)
  {
    style_text("Ces informations ne sont pas disponibles.");
    return;
  }

  $game_rep = GAME_PATH."/".$res["repertoire"];

  if (!is_dir($game_rep))
    {
      style_text("Aucun fichier disponible");
      return;
    }

  /* les sujets */
  style_title("Le sujet");

  $sub_path = $game_rep."/subject";
  if (is_file($sub_path."/current_version"))
    disp_subjects($id_projet, $sub_path);
  else
    style_text ("Pas encore de sujet pour le jeu.");

  /* les annexes */

  style_title("Les fichiers annexes");
  
  $app_path = $game_rep."/annexe";
  if (is_file($app_path."/file_list"))
    disp_appendices($id_projet, $app_path);
  else
    style_text("Pas d'annexe pour le jeu.");
}

disp_files($_info_id);

?>
