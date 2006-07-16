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

define (PROF_CANDIDAT,	1);
define (PROF_ORGA,	2);


function sendMail($name,$mail, $pass, $login)
{
   // l'	metteur
   $tete = "From: Defi Prologin <info@prologin.org>\n";
   $tete .= "Reply-To: info@prologin.org\n";
   // et zou... false si erreur d'	mission
   mail($name." <".$mail.">","Prologin defi de l'été",
"Salut à toi ".$name." et bienvenue dans la grande aventure du défi de l'été.\n\n Sur ce site tu pourras soumettre des intelligences artificielles pour gagner à l'édition 2006 du défi de l'été.\n\nJe te rappelle juste que ton pass est : ".$pass." et ton login est :".$login." ne les perd pas ! :)\n\nBonne journée, bonne chance et bon code.\n\nCordialement, \n-- \nL'équipe Prologin",$tete);
   }

/*
** Retourne la ligne de la base de donnees correspondant au profil
** $profil_type: id du profil a recuperer
*/
function get_profil_type($profil_type)
{
  global $db_link;

  $res = db_query("SELECT * FROM profil ".
		  "WHERE id='$profil_type'",
		  $db_link);

  if (db_num_rows($res) != 1)
    die("Error: get_type_profil()");

  return (db_fetch_array($res));
}

/*
** Recupere le profil de l'utilisateur identifie par $id_profil
*/
function get_profil($id_profil)
{
  global $db_link;

  $res = db_query("SELECT * FROM user_prof ".
		  "WHERE id=".$id_profil,
		  $db_link);

  if (db_num_rows($res) > 0)
    return (db_fetch_array($res));

  die("Error: get_profil()");
}

/*
** Renvoie le profil par defaut de l'utilisateur $user
*/
function user_get_def_profil($user)
{
  print "Nonnnnnnnnnnnnnnnn: user.php: user_get_def_profil</br>";

  global $db_link;

  $res = db_query("SELECT * FROM user_prof ".
		  "WHERE uid=".$user["id"]." and ".
		  "statut=1 and ".
		  "id=".$user["activ_profil"],
		  $db_link);

  if (db_num_rows($res) > 0)
    return (db_fetch_array($res));

  // fall back to default profil 
  $res = db_query("SELECT * FROM user_prof ".
		  "WHERE uid=".$user["id"]." and ".
		  "(statut=1 or id_profil>7)",
		  $db_link);

  if (db_num_rows($res) < 1)
    die("Error: user_get_def_profil()");

  $arr = db_fetch_array($res);

  // activate default profil 
  db_query("UPDATE user ".
	   "SET activ_profil=".$arr["id"].
	   " WHERE id=".$user["id"],
	   $db_link);

  return ($arr);
}

/*
** Retourne les informations generales sur l'utilisateur a partir de son login
** $login: login de l'utilisateur dont on veut les informations
*/
function user_get_from_login($login)
{
  global $db_link;

  $res = db_query("SELECT * FROM user ".
		  "WHERE login='".addslashes($login)."'", $db_link);

  if (db_num_rows($res) != 1)
    display_login_page("unknown user");

  return (db_fetch_array($res));
}

/*
** Retourne les informations generales sur l'utilisateur a partir de son profil
** $id_profil: id du profil de l'utilisateur
*/
function user_get_from_profil($id_profil)
{
  global $db_link;

  $res = db_query("SELECT user.* FROM user, user_prof ".
		  "WHERE user.id=user_prof.uid ".
		  "AND   user_prof.id=".$id_profil,
		  $db_link);

  if (db_num_rows($res) != 1)
    display_login_page("profil error");

  return (db_fetch_array($res));
}

/*
** Retrouve dans la base de donnees l'intra id d'un user
** a partir de son login
** Retourne l'intra_id uniquement
*/
function user_get_intra_id_from_login($login, $promo=null)
{
  global $db_link;

  $login = trim($login);
  $res = db_query("SELECT user_prof.id FROM user_prof, user".
		  " WHERE user_prof.uid = user.id".
		  " AND user.login='".$login."'" .
		  " AND user_prof.id_profil = 1" .
		  ($promo == null ? "" : " AND user_prof.promo = " . $promo),
		  $db_link);

  if (db_num_rows($res) == 0)
    die("Error: user_get_intra_id_from_login. login=&lt;".$login."&gt;");
  $res = db_fetch_row($res);
  return $res[0];
}

/*
** Retourne un tableau template pour l'edition/affichage de profil
** $uid: uid epita de l'utilisateur
*/
function profil_template($uid)
{
  global $user;

  $acc_owner = $user["id"] == $uid ? ACL_ALL : 0;

  return (array(
		array("id_profil",	"Type de profil","profil",
		      ACL_ALL, ACL_SITEADM),
		array("visibilite",	"Visibilite profil","profil",
		      ACL_ALL, ACL_ADM | ACL_ASSISTCHEF | ACL_SITEADM),
		array("promo",		"Promotion",	"text",
		      ACL_ALL, ACL_SITEADM),
		array("statut",		"Presence",	"bool",
		      ACL_ALL, ACL_ADM | ACL_ASSISTCHEF | ACL_SITEADM,
		      array("---", "Actif", "Inactif")),
		array("id_style",	"Style",	"style",
		      $acc_owner | ACL_SITEADM, $acc_owner | ACL_SITEADM),
		));
}

/*
** Retourne un tableau template pour l'edition/affichage des informations
** de l'utilisateur
** $uid: uid epita de l'utilisateur
*/
function user_template($uid)
{
  return (array(
		array("photo",		"Photo",		"photo", ACL_ALL, ACL_ORGA),
		array("prenom",		"Prenom",		"text", ACL_ALL, ACL_ORGA),
		array("nom",		"Nom",			"text", ACL_ALL, ACL_ORGA),
		array("nickname",	"Nick Name",		"text", ACL_ALL, ACL_ORGA),
		array("date_naiss",	"Date de naissance",	"date", ACL_ORGA, ACL_ORGA),
		array("sexe",		"Sexe",			"text", ACL_ALL, ACL_ORGA),
		array("email",		"E-mail",		"text", ACL_ORGA, ACL_ORGA),
		array("adresse",	"Adresse",		"text", ACL_ORGA, ACL_ORGA),
		array("codep",		"Code postal",		"text", ACL_ORGA, ACL_ORGA),
		array("ville",		"Ville",		"text", ACL_ORGA, ACL_ORGA),
		array("phone",		"Telephone",		"text", ACL_ORGA, ACL_ORGA),
		array("gsm",		"GSM",			"text", ACL_ORGA, ACL_ORGA),
		array("login",		"Login",		"login", ACL_ORGA, ACL_ORGA)));
}

function user_check_edit()
{
  global $_form_ssh_key;

  if (preg_match('/ssh-(dss|rsa) \S+ \S+@\S+/', $_form_ssh_key, $array))
    $_POST['_form_ssh_key'] = $array[0];
  else
    $_POST['_form_ssh_key'] = "";
}

/*
** Met a jour les informations sur les profils de l'utilisateur
*/
function update_profil()
{
  global $_action;
  global $_entry;
  global $db_link;
  global $user;
  global $user_profil;
  global $_action;
  global $_info_id;


  switch ($_action)
    {
      // Creation de profil
    case ("new_profil"):
      if (!access_check(ACL_SITEADM))
	break;

      db_query("INSERT INTO user_prof (uid) ".
	       "VALUES (".$_info_id.")", $db_link);

      $_action = "";
      return (1);

    case ("select_profil"):
      $res = db_query("SELECT id FROM user_prof ".
		      "WHERE uid=".$user["id"]." and ".
		      "statut=1 and ".
		      "id=".$_entry,
		      $db_link);

      if (db_num_rows($res) != 1)
	print("Ce profil n'est pas activable");
      else
	db_query("UPDATE user ".
		 "SET activ_profil=".$_entry.
		 " WHERE id=".$user["id"],
		 $db_link);

      $_action = "";
      return (1);

      // Effacer un profil
    case ("remove_profil"):

      if (!access_check(ACL_SITEADM))
	break;

      db_query("DELETE FROM user_prof ".
	       "WHERE id=".$_entry." and id_profil<8", $db_link);

      $_action = "";
      return (1);

    case ("edit_user"):
      $res = db_query("SELECT id FROM user ".
		      "WHERE id=".$_entry,
		      $db_link);

      if (db_num_rows($res) != 1)
	die("Error: update_profil()");

      user_check_edit();

      if (!table_update("user", user_template($_entry), "id=$_entry"))
	print ("Erreur de mise a jour des donnees");

      $_action = "";
      return (1);

      // Edition de profil
    case ("edit_profil"):
      $res = db_query("SELECT uid, id_profil ".
		      "FROM user_prof ".
		      "WHERE id=".$_entry,
		      $db_link);

      if (db_num_rows($res) != 1)
	die("Error: update_profil()");

      $row = db_fetch_row($res);

      /* only site admin can edit site admin profil */
      if (($row[1] == 8) && ($user_profil["id_profil"] != 8))
	break;

      if (!table_update("user_prof", profil_template($row[0]), "id=$_entry"))
	print ("Erreur de mise a jour des donnees");

      $_action = "";
      return (1);
    }

  return (0);
}

/*
** Initialise toutes les donnees globales de l'utilisateur depuis son login
*/
function user_init($login)
{
  global $user;
//  global $user_profil;
  global $profil;
  global $post_data;
  global $_use_profil;
  global $_entry;

  do
    {
      $user = user_get_from_login($login);
      //$user_profil = user_get_def_profil($user);
      $profil = get_profil_type($user["id_profil"]);

      /*
      if (access_check(ACL_SITEADM) && isset($_use_profil))
	{
	  switch ($_use_profil)
	    {
	    case ("init"):
	      $_use_profil = $_entry;
	    default:
	      $post_data["_use_profil"] = $_use_profil = intval($_use_profil);
	      $user = user_get_from_profil($_use_profil);
	      $user_profil = get_profil($_use_profil);
	      $profil = get_profil_type($user_profil["id_profil"]);
	    case ("exit"):
	    }
	}
        */
    } while (update_profil());
}

function is_mail_valid($mail)
{
  return ereg("^[a-z0-9._-]+@[a-z0-9.-]{2,}[.][a-z]{2,3}$", $mail);
}

function user_register()
{
	global $_login;
	global $_pass;
	global $_mail;
	global $_last;
	global $_first;

	$query = db_query("SELECT nom FROM user WHERE `login`='$_login'");
	$row = db_fetch_row($query);
	if (strlen($row[0]) && isset($_POST['register']))
	{
	  return;
	}
	if (isset($_POST['register']) && isset($_login) && isset($_pass) && strcmp($_login, "") && strcmp($_pass, "") && strcmp($_last, "") && strcmp($_first, "") && is_mail_valid($_mail))
	{
	db_query("INSERT INTO user (id_profil, id_style, nom, prenom, nickname, login, passwd, email) VALUES (1, 1, '$_last', '$_first', '$_login', '$_login', MD5('$_pass'), '$_mail')");
	sendMail($_last." ".$_first, $_mail, $_pass, $_login);
	}
}

/*
** Retourne la promo dont l'annee correspond a la globale $_promo
** Affiche la liste des promotions disponibles sur l'intra
*/
function promo_list_disp()
{
  global $db_link;
  global $post_data;
  global $user_profil;
  global $_promo;
  global $page;


  print "Nonnnnnnnnnnnnnnnn: user.php: promo_list_disp</br>";

  if (intval($user_profil["promo"]))
    return ($user_profil["promo"]);

  if (!access_check(ACL_SITEADM | ACL_PROF | ACL_ADM))
    return ($user_profil["promo"]);

  if (($_promo = intval($_promo)) > 0)
    $post_data["_promo"] = $_promo;

  /* exec db query */
  $res = db_query("SELECT promo AS id, COUNT(*) AS count ".
		  "FROM user_prof ".
		  "WHERE promo>0 AND id_profil=1 ".
		  "GROUP BY promo ORDER BY promo DESC",
		  $db_link);

  style_title("Liste des promotions");

  list_disp($res, array(
			array("id",		"Promotion",	"link_info",
			      ACL_ALL, $page["id_page"], 0, "_promo"),
			array("count",		"Nombre D'etudiants",	"text",
			      ACL_ALL),
			), array("gray", "bluegray"), "color");

  /* retourne la promotion slectionne ou 0 */
  return ($_promo);
}


?>
