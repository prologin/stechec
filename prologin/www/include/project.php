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

if (!defined("__INCLUDE__PROJECT__"))
{
define (__INCLUDE__PROJECT__, 1);



define(PRJ_STG_NONINSCRIT,	1);
define(PRJ_STG_NONVALIDE,	2);
define(PRJ_STG_INSCRIT,		3);

define(TASK_ST_WAIT, 1);
define(TASK_ST_RUNNING, 2);
define(TASK_ST_SUCCESS, 3);
define(TASK_ST_FAILED, 4);
define(TASK_ST_INT, 5);
define(TASK_ST_UNDEF, 6);

define(TASK_TYP_UPDATEMLST, 1);
define(TASK_TYP_UPDATECVS, 2);
define(TASK_TYP_UPDATECVSKEY, 3);
define(TASK_TYP_UPDATECVSMGR, 4);
define(TASK_TYP_FETCHCVSMGR, 5);
define(TASK_TYP_COMMIT_FILE, 6);
define(TASK_TYP_UPDATE_SOUT_LIST, 7);

function project_del_groups($id_projet)
{
  global $db_link;

  /* supprime les groupes de projets */
  db_query("DELETE prj_grp FROM prj_jnt, prj_grp ".
	   "WHERE prj_jnt.id_projet=".$id_projet." ".
	   "AND   prj_grp.id=prj_jnt.id_groupe",
	   $db_link);

  /* annul le lien vers les groupes dans la jointure projet/eleve */
  db_query("UPDATE prj_jnt ".
	   "SET id_groupe=0, etape=".PRJ_STG_NONINSCRIT." ".
	   "WHERE id_projet=".$id_projet,
	   $db_link);
}

function project_del_group($id_projet, $id_group)
{
  global $db_link;

  /* supprime le groupe de projets */
  db_query("DELETE prj_grp FROM prj_jnt, prj_grp ".
	   "WHERE prj_jnt.id_projet=".$id_projet." ".
	   "AND   prj_jnt.id_groupe=".$id_groupe." ".
	   "AND   prj_grp.id=prj_jnt.id_groupe",
	   $db_link);

  /* annul le lien vers les groupes dans la jointure projet/eleve */
  db_query("UPDATE prj_jnt ".
	   "SET id_groupe=0, etape=".PRJ_STG_NONINSCRIT." ".
	   "WHERE id_projet=".$id_projet." ".
	   "AND   id_groupe=".$id_groupe,
	   $db_link);
}

function project_del_jointure($id_projet)
{
  global $db_link;

  /* supprime la jointure projet/eleve */
  db_query("DELETE FROM prj_jnt ".
	   "WHERE prj_jnt.id_projet=".$id_projet,
	   $db_link);
}

/*
** Indique si le projet est a rendre par upload
** $id_projet: id fu projet
** Retourne 1 si vrai, 0 si faux
*/
function project_with_upload($id_projet)
{
  global $db_link;

  if ($id_projet <= 0)
    return 0;
  $res = db_query("SELECT with_upload FROM projet WHERE id=".$id_projet);
  $res = db_fetch_row($res);
  return $res[0];
}

/*
** Recupere les informations du groupe de projet
** dont l'id du chef est $id_chef, pour le projet $id_projet
** Renvoie un result set mysql, les infos sont de la forme
** (uid, login, intra_id, nom, prenom)
*/
function project_get_members($id_projet, $id_chef)
{
  global $db_link;

  $members = db_query("SELECT user.id AS uid, login, user_prof.id AS intra_id, nom, prenom".
		      " FROM user, user_prof, prj_jnt, prj_grp".
		      " WHERE user.id = user_prof.uid AND user_prof.id = prj_jnt.id_eleve".
		      " AND prj_jnt.id_groupe = prj_grp.id AND prj_jnt.id_projet=".$id_projet.
		      " AND prj_grp.id_chef=".$id_chef
		      , $db_link);
  return $members;
}

/*
**
*/
function project_upload_conf_set($id_projet, $max_size, $regexp, $max_upload)
{
  global $db_link;

  $regexp = addslashes($regexp);

  $res = db_query("SELECT * FROM prj_upload_conf WHERE id_projet=$id_projet",
		  $db_link);
  if (db_num_rows($res))
    db_query("UPDATE prj_upload_conf SET max_upload_size=$max_size,".
	     " name_regexp='$regexp', max_upload=$max_upload".
             " WHERE id_projet=$id_projet");
  else
    db_query("INSERT INTO prj_upload_conf".
             " (id_projet, max_upload_size, name_regexp, max_upload)".
             " VALUES($id_projet, $max_size, '$regexp', $max_upload)");
}

/*
** verifie les condition d'inscription dans un groupe
*/
function groupe_check_access($id_projet, $id_eleve)
{
  /* selectionne l'entree */
  $res = db_query("SELECT prj_jnt.id_groupe ".
		  "FROM prj_jnt, projet ".
		  "WHERE projet.id=".$id_projet." ".
		  // la date limite des inscription n'est pas passee
		  "AND   projet.date_cloture>NOW() ".
		  // la date de publication est passee
		  "AND   projet.date_distrib<NOW() ".
		  // le projet peut s'effectuer en groupe
		  "AND   projet.max_user>1 ".
		  // le projet a un statut valide
		  "AND   projet.id_statut=2 ".
		  "AND   prj_jnt.id_projet=projet.id ".
		  // l'eleve est concerne par le projet
		  "AND   prj_jnt.id_eleve=".$id_eleve,
		  $db_link);

  /* aucune entree: profil non concerne par le projet */
  return (db_fetch_row($res));
}

/*
** Indique si un groupe est joignable pour un eleve
** $id_projet: id du projet
** $id_groupe: id du groupe a verifier
** $id_eleve: intra_id de l'eleve voulant rejoindre le groupe
** Retourne 1 si joignable, 0 si pas joignable
*/
function groupe_check_joinable($id_projet, $id_groupe, $id_eleve)
{
  /* projet joignable par l'eleve */
  if (db_num_rows(db_query("SELECT prj_jnt.id_groupe ".
			   "FROM prj_jnt, projet ".
			   "WHERE projet.id=".$id_projet." ".
			   "AND   prj_jnt.id_projet=projet.id ".
			   "AND   prj_jnt.id_groupe=".$id_groupe,
			   $db_link)) <= 0)
    return (0);

  /* eleve deja dans le groupe ? */
  if (db_num_rows(db_query("SELECT id_groupe ".
			   "FROM prj_jnt ".
			   "WHERE id_eleve=".$id_eleve." ".
			   "AND   id_groupe=".$id_groupe,
			   $db_link)))
    return (0);

  /* nombre d'inscrit dans le groupe */
  $row = db_fetch_row(db_query("SELECT COUNT(*)>=projet.max_user ".
			       "FROM prj_jnt, projet ".
			       "WHERE prj_jnt.id_groupe=".$id_groupe." ".
			       "AND   prj_jnt.id_projet=projet.id ".
			       "AND   prj_jnt.etape>".PRJ_STG_NONVALIDE,
			       $db_link));

  if ($row[0])
    {
      style_alert("Ce groupe de projet est deja complet");
      return (0);
    }

  return (1);
}

/*
** Indique si l'eleve est le chef du groupe passe en parametre
** $id_projet: id du projet
** $id_groupe: id du groupe que l'on veut verifier
** $id_eleve: intra_id de l'eleve pour qui on veut verifier si il est chef
*/
function groupe_check_chef($id_projet, $id_groupe, $id_eleve)
{
  /* selectionne l'entree */
  $res_a = db_query("SELECT prj_jnt.id_groupe ".
		    "FROM prj_jnt, prj_grp ".
		    "WHERE prj_jnt.id_projet=".$id_projet." ".
		    "AND   prj_jnt.id_groupe=".$id_groupe." ".
		    "AND   prj_jnt.id_groupe=prj_grp.id ".
		    "AND   prj_grp.id_chef=".$id_eleve,
		    $db_link);

  return (db_num_rows($res_a) > 0);
}

/*
** Recupere l'intra_id et le login du chef de groupe a partir d'un login
** d'un membre du groupe
*/
function groupe_get_chef($id_projet, $id_eleve)
{
  global $db_link;
  global $user;

  $res = db_query("SELECT prj_grp.id_chef, user.login ".
		  "FROM prj_grp, prj_jnt, user, user_prof".
		  " WHERE prj_jnt.id_eleve = ".$id_eleve.
		  " AND user.id = user_prof.uid".
		  " AND user_prof.id = prj_grp.id_chef".
		  " AND prj_jnt.id_groupe = prj_grp.id ".
		  " AND prj_jnt.id_projet=".$id_projet,
		  $db_link);
  // si aucun record n'est trouve, projet individuel, l'eleve
  // est donc son propre chef
  if (db_num_rows($res) < 1)
    return array("login" => $user['login'], "id_chef" => $id_eleve);

  return db_fetch_array($res);
}

/*
** supprime le groupe ou l'eleve est chef
** ne fait rien s'il n'est pas chef
*/
function groupe_drop_chef($id_projet, $id_chef)
{
  global $db_link;

  /* marque tout les membres du groupe ou l'eleve est chef
   * comme non inscrit et sans groupe */
  db_query("UPDATE prj_jnt, prj_grp ".
	   "SET prj_jnt.id_groupe=0, ".
	   "    prj_jnt.etape=".PRJ_STG_NONINSCRIT." ".
	   "WHERE prj_jnt.id_projet=".$id_projet." ".
	   "AND   prj_grp.id_chef=".$id_chef." ".
	   "AND   prj_jnt.id_groupe=prj_grp.id",
	   $db_link);

  /* supprime le groupe ou l'eleve est chef de groupe */
  db_query("DELETE FROM prj_grp ".
	   "WHERE id_projet=".$id_projet." ".
	   "AND   id_chef=".$id_chef,
	   $db_link);
}

/*
** Creer un groupe de projet (utilise par l'ajout de groupe par fichier)
** $members est un tableau contenant les logins des membres
** $id_projet est l'id du projet concerne
*/
function project_create_group($members, $id_projet)
{
  global $db_link;
  global $user_profil;
  $promo = $user_profil["promo"];


  // Recuperer des informations sur le projet selectionne
  $projet = db_fetch_array(
			   db_query("SELECT * FROM projet WHERE id=$id_projet"));
  if (count($members) > $projet['max_user'])
  {
    $string = "<b>Groupe rejet&eacute;</b> : ";
    for ($i = 0; $i < count($members); $i++)
      $string .= $members[$i]." ";
    style_text($string."<br/>");
    return ;
  }

  // Cas du chef de groupe
  $intra_id = user_get_intra_id_from_login($members[0], $promo);

  db_query("INSERT INTO prj_grp (statut, id_projet, id_chef) ".
	   "VALUES (".PRJ_STG_NONINSCRIT.", ".$id_projet.", ".
	   $intra_id.")", $db_link);

  // recuperer l'id du groupe que l'on vient de creer
  $grp = db_query("SELECT * FROM prj_grp WHERE id_projet=".$id_projet.
		  " AND id_chef=".$intra_id.
		  " AND statut=".PRJ_STG_NONINSCRIT.
		  " LIMIT 1",
		  $db_link);
  $id_group = db_fetch_array($grp);

  // Inserer tous les membres dans la table d'inscription
  for ($i = 0; $i < count($members); $i++)
    {
      $intra_id = user_get_intra_id_from_login($members[$i], $promo);
      db_query("UPDATE prj_jnt SET id_groupe=".$id_group['id'].
	       ", etape=".PRJ_STG_INSCRIT.
	       " WHERE id_projet=".$id_projet.
	       " AND id_eleve=".$intra_id,
	       $db_link);
    }
  return 0;
}

/*
** supprime toute les donnees relatives au projet dans
** la base de donnees (groupes, chefs de groupe, responsables
** du projet, projet)
*/
function project_del($id_projet)
{
  global $db_link;

  /* supprime les information
   * relatives aux inscriptions */
  project_del_groups($id_projet);
  project_del_jointure($id_projet);

  /* supprime la liste des responsables du projet */
  db_query("DELETE FROM prj_resp WHERE id_projet=".$id_projet,
	   $db_link);

  /* le projet */
  db_query("DELETE FROM projet WHERE id=".$id_projet,
	   $db_link);
}

/*
** Fonction appelee quand on veut afficher des donnees relatives
** a un projet mais qu'aucun projet n'a ete selectionne.
** Affiche un lien vers la liste des projets
*/
function projet_noselect()
{
  style_text("Veuillez selectionner un projet dans la liste...");
  style_links(array(
		    array("Acceder a la liste des projets", ACL_ELEVE,
			  array("_id_page" => 31))
		    ));
  return (1);
}

/*
** Retourne un tableau template pour la creation de projet
** (definition de l'interface de creation de projet)
*/
function project_new_template()
{
  /* donnees du formulaire de creation de projet */
  return (array(
		array("",	"Informations projet",	"title",
		      ACL_ALL, 0),
		array("nom",		"Nom",			"text",
		      ACL_ALL, ACL_ASSIST | ACL_SITEADM),
		array("id_type",	"Type",			"prj_typ_enum",
		      ACL_ALL, ACL_ASSIST | ACL_SITEADM),
		array("max_groupe",	"Nombre de groupes max","text",
		      ACL_ALL, ACL_ASSIST | ACL_SITEADM),
		array("max_user",	"Nombre d'eleves max",	"text",
		      ACL_ALL, ACL_ASSIST | ACL_SITEADM),
		));
}


/*
**  Retourne l'acces de responsable pour un projet
*/
function get_acces_project_resp($id_projet)
{
  global $db_link;
  global $user_profil;

  return db_num_rows(db_query("SELECT * FROM prj_resp ".
				 "WHERE id_projet=".$id_projet." and ".
				 "id_profil=".$user_profil["id"], $db_link)) > 0
    ? ACL_ALL				/* acces reponsable de projet */
    : ACL_SITEADM | ACL_ASSISTCHEF;	/* acces non responsable */
}


}

/*
**
*/
function update_ssh_keys($id_projet, $project_grp)
{
  global $db_link;
  global $user_profil;

  db_query("INSERT INTO task (requestor_id, task_type_id, task_parameter) ".
	   "VALUES (".$user_profil['id'].", ".TASK_TYP_UPDATECVSKEY.", ".
	   "'$id_projet $project_grp')", $db_link);

  db_query("UPDATE prj_grp SET cvs_keys_id_task=LAST_INSERT_ID() WHERE ".
	   "prj_grp.id=$project_grp", $db_link);
}
?>


