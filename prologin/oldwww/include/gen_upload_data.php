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

	* Carel Tristan <carel_t@epita.fr>

*/








/*
 * compresse le fichier / repertoire $in qui se trouve dans le dossier
 * $rep.  le fichier de sortie s'appelle $out.  Si le $out est en
 * relatif, considerez que vous etes dans le repertoire $rep.
 */
function bzip2 ($rep, $in, $out)
{
  //FIXME
//   if (!file_exists ($in) || !is_readable($rep) || !is_readable ($in))
//     return false;
//   if ((!file_exists ($out) && !is_writeable (dirname ($out))
//        || (file_exists($out) && !is_writable($out)) ))
//     return false;

  // desole mais ca me saoule d'utiliser la lib Bzip2
  $ret = 0;
  $output = array();
  exec("cd ". $rep. " && tar chof - ". $in . " | bzip2 -9 -c > ". $out);
  return 0;
}












/*------------------------------------------------------------------.
| cree une tarball dans le repertoire $rep_dest qui contient :      |
|	* un dossier $rep_trace qui contient les donnees d'upload   |
|	du projet $id_proj.					    |
|	les fichiers sont de la forme %login%.trace.                |
|                                                                   |
|       * un fichier 'upload_exports' qui contient les variables    |
|       d'exports a rajouter dans le export final.                  |
`------------------------------------------------------------------*/

/**
 ** retourne 0 si le fichier 'proj-$id_proj-upload-data.tar.bz2' a
 ** ete correctement cree dans le dossier $rep_trace.
 */
function	gen_upload_tarball($id_proj, $rep_dest, $rep_trace)
{
  // verifie le droit d'ecriture dans le dossier $rep_trace
  if (!is_writable($rep_dest))
    return 1;

  // dossier qui sera compresse
  $tarball=$rep_dest. "/proj-".$id_proj."-upload-data";

  exec("rm -rf ". $tarball);

  // creation du repertoire de decompression
  if (!mkdir($tarball, 0755))
    return 2;

  // creation du repertoire contenant les traces
  if (!mkdir($tarball."/t".$rep_trace, 0755))
    return 3;

  /*
   * on recupere une table de hachage qui contient toutes les donnees
   * d'upload du projet
   */
  global	$db_link;
//   $query = db_query(" SELECT user.login, upload.id_eleve,".
// 		    " upload.nb_uploads, upload.nb_uploads_before,".
// 		    " upload.nb_uploads_after,".
// 		    " user_prof.promo, projet.nom AS nom_projet".
// 		    " FROM user, prj_uploads AS upload, user_prof, projet".
// 		    " WHERE upload.id_projet=".$id_proj.
// 		    " AND user_prof.id=upload.id_eleve".
// 		    " AND user_prof.uid=user.id");

  $query = db_query("SELECT user.login, upload.nb_uploads,".
		    " upload.nb_uploads_before, upload.nb_uploads_after,".
		    " projet.promo, projet.nom AS nom_projet".
		    " FROM user, prj_uploads AS upload, projet, user_prof, prj_grp, prj_jnt".
		    " WHERE user.id=user_prof.uid".
		    " AND user_prof.id=prj_jnt.id_eleve".
		    " AND prj_jnt.id_projet=".$id_proj.
		    " AND prj_jnt.id_groupe=prj_grp.id".
		    " AND prj_grp.id_projet=".$id_proj.
		    " AND prj_grp.id_chef=upload.id_eleve".
		    " AND upload.id_projet=".$id_proj);

  // rajoute les membres du groupe
//   $query = db_query(" SELECT user.login, upload.id_eleve,".
// 		    " upload.nb_uploads, upload.nb_uploads_before,".
// 		    " upload.nb_uploads_after,".
// 		    " user_prof.promo, projet.nom AS nom_projet".
// 		    " FROM user, prj_uploads AS upload, user_prof, prj_jnt, prj_grp, projet".
// 		    " WHERE user.id=user_prof.uid AND user_prof.id=prj_jnt.id_eleve".
// 		    " AND prj_jnt.id_projet=".$id_proj.
// 		    " AND prj_jnt.id_groupe=prj_grp.id".
// 		    " AND prj_grp.id_chef=upload.id_eleve".
// 		    " AND projet.id=".$id_proj);

  // pas de rendu trouve
  if (db_num_rows($query) == 0)
    return 4;
  style_text(__LINE__);
  // on itere sur chaque etudiant pour creer leur trace
  while ($entry = db_fetch_array($query))
    {
      $nom_projet = $entry["nom_projet"];
      $promo = $entry["promo"];
      if (!($handle = fopen($tarball."/t".$rep_trace."/".$entry["login"].".trace", "w")))
	{
	  style_text("ne peut pas creer le fichier : ". $tarball."/t".$rep_trace."/".$entry["login"].".trace");
	  continue;
	}

      if (!(fwrite($handle, "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\"\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "  <trace>\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "    <upload_data>\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "      <nb_uploads>". $entry["nb_uploads"]  ."</nb_uploads>\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "      <nb_uploads_before>". $entry["nb_uploads_before"]  ."</nb_uploads_before>\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "      <nb_uploads_after>". $entry["nb_uploads_after"] ."</nb_uploads_after>\n"))) {
	fclose($handle);
	continue;
      }


      if (!(fwrite($handle, "    </upload_data>\n"))) {
	fclose($handle);
	continue;
      }

      if (!(fwrite($handle, "  </trace>\n"))) {
	fclose($handle);
	continue;
      }

      fclose($handle);
    }

  // ajout du fichier contenant les regles d'export
  if ($handle = fopen($tarball."/upload-exports", "w"))
    {
      fwrite($handle, "#\n##\n## Recapitulatif des updload\n".
	     "### du projet ".
	     $nom_projet."(numero ".$id_proj.") promo ". $promo. "\n##\n#\n\n");

      fwrite($handle, "# numero de trace, chemin xml, must exist, default,".
	     "perl var in formula, description, type d'export (0 = inconnu)\n");
      fwrite($handle, $rep_trace.",trace.upload_data.nb_uploads,0,0,".
	     "nb_uploads,Nombre de tarballs uploadees,0\n");
      fwrite($handle, $rep_trace.",trace.upload_data.nb_uploads_before,0,0,".
	     "nb_uploads_before,Nombre de tarballs uploadees avant la date de rendu,0\n");
      fwrite($handle, $rep_trace.",trace.upload_data.nb_uploads_after,0,0,".
	     "nb_uploads_after,Nombre de tarballs uploadees apres la date de rendu,0\n");

      fclose($handle);
    }

  // creation de la tarball
  bzip2($rep_dest, "proj-".$id_proj."-upload-data", "/tmp/proj-".$id_proj."-upload-data.tar.bz2");

  // suppression du repertoire de decompression
  exec("rm -rf ".$tarball);
  return 0;
}



/**
 **
 ** $upload_info : table de hachage qui contient les cles :
 **	- "login"	: le login de l'etudiant
 **	- "id_prof"	: id du profile 'etudiant' de l'etudiant
 **	- "nb_uploads"	: nombre d'upload effectue par l'etudiant
 **	- "nb_uploads_before"	: ...
 **	- "nb_uploads_after"	: ...
 */
function	gen_upload_file($upload_info, $output_dir)
{

}


function	dl_upload_data($id_project, $id_trace)
{
  if (gen_upload_tarball($id_project, "/tmp", $id_trace))
    {
      style_text("Could not generate upload data for project ".$id_project);
      return 1;
    }

  // Send the tarball to the client
  $filename = "proj-".$id_project."-upload-data.tar.bz2";
  header("Content-type: application/x-bzip2");
  header("Content-Disposition: attachment; filename=".$filename);
  $filesize = filesize ("/tmp/".$filename);
  header("Content-length: ".$filesize);
  readfile("/tmp/".$filename);
  return 0;
}

?>
