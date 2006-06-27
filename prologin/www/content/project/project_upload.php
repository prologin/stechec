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

	* Jacob Olivier <jacob_o@epita.fr>

*/

/*
** Ce fichier utilise les tables prj_uploads et prj_upload_conf
** de la base mysql.
** Le champ 'name_regexp' de la table prj_upload_conf permet de
** valider le nom de la tarball uploadee. Il est possible d'utiliser
** les variables suivantes dans la regexp:
** %login% : remplace par le login du chef de groupe
** %projet% : remplace par le nom du projet
*/

include "include/download.php";
include "include/project.php";

// Intervalle de temps entre deux soutenances
define (UPLOAD_INTERVAL, 5);
// Chemin de stockage des tarballs
define (UPLOAD_TARBALL_DEST_PATH, "/space/export/tarballs/");
// Mask de droits pour la creation de repertoires
define (UPLOAD_DEST_PATH_MASK, 0755);

// Define pour les erreurs de l'upload
define (UPLOAD_ERR_INI_FILE, 1);
define (UPLOAD_ERR_FORM_SIZE, 2);
define (UPLOAD_ERR_PARTIAL, 3);
define (UPLOAD_ERR_NOFILE, 4);

/*
** Retourne un tableau associtif contenant la conf de l'upload
** ou 0 si le projet ne se rend pas par upload
** (cf. prj_upload_conf pour la structure)
*/
function upload_get_conf($id_projet)
{
  global $db_link;

  $res = db_query("SELECT * FROM prj_upload_conf WHERE id_projet=".$id_projet,
		  $db_link);
  if (db_num_rows($res) != 1)
    return 0;
  return db_fetch_array($res);
}

/*
** Verifie que l'intervalle entre l'upload courant et le dernier
** upload est assez important pour eviter les uploads non desires
** quand l'ing1 fait un refresh de la page ...
** $id_eleve: id du chef de groupe (ou de l'eleve si projet sans groupe)
** Retourne 0 si OK, 1 sinon
*/
function upload_check_interval($id_projet, $id_eleve)
{
  global $db_link;

  $q = db_query("SELECT ((UNIX_TIMESTAMP(NOW()) - UNIX_TIMESTAMP(last_upload_time)) / 60) AS elapsed".
	   " FROM prj_uploads".
	   " WHERE id_projet=".$id_projet.
	   " AND id_eleve=".$id_eleve,
	   $db_link);
  if (db_num_rows($q) != 1)
    return 0;
  $q = db_fetch_array($q);

  return ($q['elapsed'] >= UPLOAD_INTERVAL ? 0 : 1);
}

/*
** Verifie que le nom du fichier donne satisfait la regexp
** donnee en parametre
** $regexp: la regexp pour valider le nom
** $id_projet: id du projet selectionne
** $chef: informations sur le chef de groupe (nom et id)
** Retourne 0 si ok 1 sinon
*/
function upload_check_name($regexp, $proj, $chef)
{
  $regexp = stripslashes($regexp);
 
  $regexp = preg_replace("/%projet%/", $proj['nom'], $regexp);
  $regexp = preg_replace("/%login%/", $chef['login'], $regexp);

  return !preg_match("/".$regexp."/", $_FILES['_form_tarball_path']['name']);
}

/*
** Renvoie le nom attendu pour la tarball en fonction de la
** regexp
*/
function upload_get_correct_name($regexp, $proj, $chef)
{
  $regexp = stripslashes(stripslashes($regexp));
 
  $regexp = preg_replace("/%projet%/", $proj['nom'], $regexp);
  $regexp = preg_replace("/%login%/", $chef['login'], $regexp);

  return $regexp;
}

/*
** Ajoute le numero du rendu a la fin du nom de la tarball
** $proj: array contenant des infos sur le projet
** $chef: array contenant l'intra_id et l'uid du chef de groupe
** Retourne le nom complet (string)
*/
function upload_make_tarballname($proj, $chef, $conf)
{
  global $db_link;

  $res = db_query("SELECT nb_uploads FROM prj_uploads ".
		  "WHERE id_projet=".$proj['id']." ".
		  "AND id_eleve=".$chef['id_chef'], $db_link);
  $res = db_fetch_array($res);

  // this is required, because if it is the first upload of the student,
  // nb_upload is not set thus giving bad tarballs name
  //
  // for instance:
  //   gerlac_p-tc-1-rendu.tar.bz2
  //
  if ($res['nb_uploads'] == NULL)
  {
    $res['nb_uploads'] = 0;
  }

  $name = upload_get_correct_name($conf['name_regexp'], $proj, $chef);
  $name = preg_replace("/(.*).(tar.bz2|tar.gz|tgz|rar|zip)/", 
		       "\$1-rendu". ($res['nb_uploads'] + 1) .".\$2", 
		       $name);

  return $name;
}

/*
** Effectue l'upload en lui meme. Bouge la tarball dans le repertoire
** de destination des tarballs (sur azkaban)
** Affiche l'erreur si erreur il y a
** $proj: array contenant des infos sur le projet
** $chef: array contenant l'intra_id et l'uid du chef de groupe
** Retourne 1 si une erreur se produit, 0 sinon
*/
function upload_process($proj, $chef, $conf)
{
  global $db_link;

  $conf = db_fetch_array(db_query("SELECT * FROM prj_upload_conf ".
				  "WHERE id_projet=".$proj['id'],
				  $db_link));

  // Cette verification semble necessaire car la limitation de la taille
  // de fichier envoye par la variable MAX_FILE_SIZE du protocole HTTP
  // ne semble pas donner des resultats vraiment concluants.
  if ($conf['max_upload_size'] < $_FILES['_form_tarball_path']['size'])
  {
    style_text("<b>La taille du fichier est trop importante</b>");
    return 1;
  }

  // Traitement des erreurs renvoyees par le serveur
  switch ($_FILES['_form_tarball_path']['error'])
  {
  case UPLOAD_ERR_INI_FILE:
    style_text("<b>La taille du fichier est trop importante</b>");
    return 1;
  case UPLOAD_ERR_FORM_SIZE:
    style_text("<b>La taille du fichier envoye est trop grande.</b>");
    return 1;
  case UPLOAD_ERR_PARTIAL:
    style_text("<b>Le fichier n'est pas complet. Reessaye</b>");
    return 1;
  case UPLOAD_ERR_NOFILE:
    style_text("<b>Le fichier n'a pas ete uploade</b>");
    return 1;
  }

  if (!is_dir(UPLOAD_TARBALL_DEST_PATH.$proj['nom']))
    if (mkdir(UPLOAD_TARBALL_DEST_PATH.$proj['nom'],
	      UPLOAD_DEST_PATH_MASK) == false)
      return 1;

  $tarballname = upload_make_tarballname($proj, $chef, $conf);

  if (move_uploaded_file($_FILES['_form_tarball_path']['tmp_name'], 
			 UPLOAD_TARBALL_DEST_PATH.$proj['nom']."/".
			 $tarballname) == false)
  {
    style_text("<b>Impossible de deplacer le fichier</b>");
    return 1;
  }

  return 0;
}

/*
** Incremente les compteurs dans la table
*/
function upload_update_table($projet, $id_eleve)
{
  global $db_link;
  
  $q = db_query("SELECT * FROM prj_uploads".
		" WHERE id_projet=".$projet['id'].
		" AND id_eleve=".$id_eleve." LIMIT 1",
		$db_link);

  if (db_num_rows($q))
  {
    // deja fait des uploads
    $q = db_fetch_array($q);
    $q['nb_uploads']++;
    if ($projet['proj_rendu'])
      $q['nb_uploads_after']++;
    else
      $q['nb_uploads_before']++;

    db_query("UPDATE prj_uploads SET ".
	     "nb_uploads=".$q['nb_uploads'].
	     ", last_upload_time=NOW()".
	     ", nb_uploads_before=".$q['nb_uploads_before'].
	     ", nb_uploads_after=".$q['nb_uploads_after'].
	     " WHERE id_eleve = ".$id_eleve. " AND id_projet=".$projet['id'],
	     $db_link);
  }
  else
  {
    $before = $after = 0;
    if ($projet['proj_rendu'])
      $after++;
    else
      $before++;

    // premier upload pour le projet
    db_query("INSERT INTO prj_uploads ".
	     "VALUES(".$projet['id'].", $id_eleve, 1, NOW(), $before, $after)",
	     $db_link);
  }
}

/*
** Affiche les uploads realises par l'eleve (ou le groupe auquel
** l'eleve appartient) pour le projet $id_projet
*/
function disp_my_uploads($projet, $chef)
{
  global $db_link;

  style_title("Mes uploads");

  /* code pour le rush T1 */

  $list = array(
		array("nb_uploads_before", "Avant la date de rendu", "text", ACL_ALL,
		      array(), array("center")),
		array("nb_uploads_after", "Apres la date de rendu", "text", ACL_ALL,
		      array(), array("center")),
		array("last_upload_time", "Date dernier upload", "text", ACL_ALL,
		      array(), array("center")),
		);

  $uploads = db_query("SELECT nb_uploads_before, nb_uploads_after, last_upload_time ".
		      "FROM prj_uploads WHERE id_projet=".$projet['id'].
		      " AND id_eleve=".$chef['id_chef'],
		      $db_link);
  
  list_disp($uploads, $list, array("gray"), "none");
  

  /* Futur code de l'upload - Affichage des anciennes traces de moulinette */

//   $list = array(
// 		array("filename", "Nom de la trace", "text", ACL_ALL),
//  		array("id",		"Visualiser",	"link", ACL_ALL,
//  		      array("_id_page" => 666, "_old_trace" => 1), array("center"), "_entry"),
// 		);

//   $dir = opendir(PRJ_DATA_PATH.$projet['id']."/traces/t2/old_traces");
//   $table = array();
//   $i = 0;
//   while (($file = readdir($dir)) != false)
//   {
//     if (!is_file(PRJ_DATA_PATH.$projet['id']."/traces/t2/old_traces/".$file))
//       continue ;

//     if (preg_match("/".$chef['login']."-([0-9]+)/", $file, $matches))
//     {
//       $item['filename'] = $file;
//       $item['id'] = $matches[1];
//       $table[] = $item;
//     }
//   }
//   closedir($dir);

//   sort($table);
//   if (count($table))
//     list_disp_data($table, $list, array("gray", "bluegray"), "even");
}

/*
** Affiche les gens ayant deja effectue un upload pour le projet
*/
function disp_assist_uploads($id_projet)
{
  global $db_link;
  global $_entry;

  $res = db_query("SELECT CONCAT(user.prenom, ' ', user.nom) AS name, prj_uploads.* ".
		   "FROM user, prj_uploads, user_prof ".
		   "WHERE user.id=user_prof.uid ".
		   "AND user_prof.id=prj_uploads.id_eleve ".
		   "AND prj_uploads.id_projet=".$id_projet." ".
		   "ORDER BY prj_uploads.last_upload_time DESC", $db_link);

  style_text("Il y a " . db_num_rows($res) . " tarball uploadee(s) au total");

  $list = array(
		array("name", "Chef de groupe", "text", ACL_ASSIST_ALL | ACL_SITEADM),
		array("nb_uploads", "Uploads", "text", ACL_ASSIST_ALL | ACL_SITEADM,
		      array(), array("center")),
		array("nb_uploads_before", "Avant rendu", "text", 
		      ACL_ASSIST_ALL | ACL_SITEADM, array(), array("center")),
		array("nb_uploads_after", "Apr&egrave;s rendu", "text", 
		      ACL_ASSIST_ALL | ACL_SITEADM, array(), array("center")),
		array("last_upload_time", "Dernier rendu", "text",
		      ACL_ASSIST_ALL | ACL_SITEADM, array(), array("center")),
		);

  list_disp($res, $list, array("gray", "bluegray"), "even");

  $download = array(
		    array("", "Telechargement", "title", ACL_ASSIST_ALL | ACL_SITEADM, 0),
		    array("Telecharger les donn&eacute;es d'upload", "Telechargement", "link",
			  ACL_NOT_ELEVE, 0, array("_dl_type" => "prj_upload_data")),
		    );
  table_disp_one(array(), $download, "", $_entry);
}

/*
** Affiche l'interface d'upload d'une tarball
*/
function disp_interface($id_projet)
{
  global $_action;
  global $user_profil;
  global $db_link;

  if ($id_projet <= 0)
    return projet_noselect();

  // Recuperation de l'intra_id et login du chef de groupe
  $chef = groupe_get_chef($id_projet, $user_profil['id']);
  // Recuperation de la configuration de l'upload pour le projet
  $conf = upload_get_conf($id_projet);

  $projet = db_fetch_array(db_query("SELECT *, (projet.date_rendu<NOW()) AS proj_rendu FROM projet ".
				    "WHERE id=".$id_projet, 
				    $db_link)
			   );

  // Verifier que le projet se rend bien par upload
  if (($projet['with_upload'] == 0 || $conf == 0) && access_check(ACL_ELEVE))
  {
    style_text("<b>Ce projet ne se rend pas par upload</b>");
    die();
  }

  // Verification que l'etudiant n'a pas upload trop de tarball
  $uploads = db_query("SELECT prj_uploads.nb_uploads, prj_upload_conf.max_upload".
		      " FROM prj_uploads, prj_upload_conf".
                      " WHERE prj_uploads.id_projet=".$id_projet.
                      " AND prj_upload_conf.id_projet=".$id_projet.
		      " AND prj_uploads.id_eleve=".$chef['id_chef']);

  // La tables d'upload n'est pas initialise a l'origine
  //  => il faut distinguer les cas.
  if (db_num_rows($uploads) < 1)
    {
      // Le gars n'a pas encore upload
      $uploads = db_query("SELECT max_upload FROM prj_upload_conf".
                         " WHERE prj_upload_conf.id_projet=".$id_projet);
      $uploads_res = db_fetch_array($uploads);
      $max_tarb = $uploads_res[0];
      $have_upload = 0;
    }
  else
    {
      // Le gars a deja upload
      $uploads_res = db_fetch_array($uploads);
      $max_tarb = $uploads_res[1];
      $have_upload = $uploads_res[0];
    }

  if (access_check(ACL_ELEVE))
  {
    switch ($_action)
    {
      case ("do_upload"):
        // On verifie que l'etudiant n'a pas depasse le max.
	// Mettre un max < 0 pour un upload illimite. 
	if ($max_tarb >= 0 && $have_upload >= $max_tarb)
        {
          style_text("<b>Upload impossible: le quota maximum est atteint.</b>");
          break;
        }
        
        style_title("Resultat de l'upload");
        // Update de la base de donnees
      
        // ne checker l'intervalle que si le groupe a deja fait un upload
        if (upload_check_interval($id_projet, $chef['id_chef']))
  	  style_text("<b>Upload impossible. Le dernier upload est trop proche.</b><br/>");
        else if (upload_check_name($conf['name_regexp'], $projet, $chef))
	  style_text("<b>Nom de tarball incorrect.<br/>Attendu: ".
	  	    upload_get_correct_name($conf['name_regexp'], $projet, $chef)."</b>");
        else
        {
	  if (upload_process($projet, $chef, $conf))
	    break;
	  else
	  {
	    upload_update_table($projet, $chef['id_chef']);
	    style_text("<b>Upload pris en compte</b>");
	  }
        }
        break;
      }
    
    style_title("Rendu par upload (max = ". $max_tarb ." )");
    
    if ($have_upload >= $max_tarb)
      style_text("Upload Impossible: Vous avez depassé le quota d'upload.");
    else
    {
      style_text("<b>Explications</b><br/>".
  	        "%login% est &agrave; remplacer par le nom du chef du groupe<br/>".
  	        "%projet% est &agrave; remplacer par le nom du projet &agrave; rendre");

      // Afficher un tableau rappelant les donnees de configuration
      // du rendu upload (taille max de la tarball et nom de la tarball)
      $conf_reminder = array (
			    array("", "<b>Rappels de configuration</b>", "title",
				  ACL_ALL, 0),
			    array("max_upload_size", "Taille max. de l'upload (Octets)", "text",
				  ACL_ALL, 0),
			    array("name_regexp", "Regexp (Perl)", "text",
				  ACL_ALL, 0),
			    );
    
      $_conf = db_query("SELECT * FROM prj_upload_conf WHERE id_projet=".$id_projet,
  		        $db_link);
      $_conf = db_fetch_array($_conf);
      $_conf['name_regexp'] = stripslashes($_conf['name_regexp']);
      table_disp_one($_conf, $conf_reminder, "conf_upload", "_entry");

      // Affichage de la zone de rendu
      $ihm = array(
		 array("", "<b>Upload</b>", "title", ACL_ALL, 0),
		 array("tarball_path", "Chemin Tarball", "upload", 
		       ACL_ELEVE, ACL_ELEVE),
		 );
      table_disp_one(array("id" => $id_projet), $ihm, "do_upload", "_entry", $conf['max_upload_size']);
    }
    // Affichage des uploads deja effectues
    disp_my_uploads($projet, $chef);
  } 
  else if (access_check(ACL_ASSIST_ALL | ACL_SITEADM))
  {
    style_title($projet['nom']." : Rendus d&eacute;j&agrave; effectu&eacute;s");
    disp_assist_uploads($id_projet);
  }
  else
    style_text("Vous n'avez pas les droits pour acc&eacute;der &agrave; cette page");
}

disp_interface($_info_id);

?>
