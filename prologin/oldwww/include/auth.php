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

    You should have received a copy of the GNUg General Public License
    along with the ACU Intranet project; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Copyright ACU EPITA Promo 2004

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>

*/

define(LOGIN_TIMEOUT, 2700);

// error :
//  0 : auth successfull
//  1 : auth failed
//  2 : connection to server failed


/*
function download_pass($user)
{ 
  $infile = tempnam("/tmp", "pass");
  $outfile = "$infile.gpg";

  $res = db_query("SELECT plainpasswd FROM user WHERE `login`='$user'");
  if (!$res)
    return 0;
  $row = db_fetch_row($res);

  if ($row[0] == "")
  {
    $random=rand();        // Seeded via initialize()
    $rand64= "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    $salt=substr($rand64,$random  %  64,1).substr($rand64,($random/64) % 64,1);

    $pass = exec("pwgen -s");
    $hash = crypt($pass, $salt);

    db_query("UPDATE user SET `plainpasswd`='$pass', `ngpasswd`='{crypt}$hash' WHERE `login`='$user'");
  }
  else
  {
    $pass = $row[0];
  }
  
  $fd = fopen($infile, "w");
  fputs($fd, "Votre mot de passe intranet est : $pass\n");
  fclose($fd);
  system("env GNUPGHOME='/home/local/intradmin/.gnupg' gpg --batch --no-tty --always-trust --encrypt -r $user@epita.fr -o $outfile $infile");
*/
  /* Send the encrypted password.
  header("Content-type: application/x-unknown");
  header("Content-Disposition: attachment; filename=intrapass.gpg");
  $filesize = filesize ($outfile);
  header("Content-length: $filesize");
  readfile($outfile);
*/
/*
  system("echo 'Voici votre mot de passe intranet' | mutt -a $outfile -s 'Acces Intranet' $user@epita.fr");
  display_login_page("Your password has been sent");
}
*/

function check_password($username, $password)
{
  // prolo
  //db_init("localhost");
  db_init();


  $res = db_query("SELECT passwd FROM user WHERE `login`='$username'");
  //$res = db_query("SELECT passwd FROM user WHERE `login`='$username' AND id_profil != 1");
  if (!$res)
    return 0;

  $row = db_fetch_row($res);
  return (strcmp($row[0], md5($password)) == 0 ? 1 : 0);
  
  /*
  $row = db_fetch_row($res);
  if ($row[0] == 1 && $row[1] == 0)
  {
    $passwd = "passwd";
  }
  else
  {
    $passwd = "ngpasswd";
  }

  $res = db_query("SELECT $passwd FROM user WHERE `login`='$username'");
  $row = db_fetch_row($res);
  $hash = substr($row[0], 7);
  $salt = substr($hash, 0, 2);
  $str = crypt($password, $salt);

  return ($str == $hash ? 1 : 0);
  */
  
}

function add_session_cookie($login)
{
  global $db_link;
  global $REMOTE_ADDR;

  if(!db_query("INSERT INTO session (uid, cookie_data, ".
	       "log_date, log_period, log_host, valid) ".
	       "SELECT user.id, MD5(CONCAT(RAND(),RAND(),RAND(),RAND())), ".
	       "NOW(), 0, '".addslashes(gethostbyaddr($REMOTE_ADDR))."', 1 ".
	       "FROM   user WHERE user.login='".addslashes($login)."'",
	       $db_link))
    die ("auth.php: add_session_cookie()");

  $data = db_fetch_row(db_query("SELECT id, cookie_data FROM session ".
				"WHERE id=LAST_INSERT_ID()", $db_link));

  /*
   * cookie expires in 30 days (time() + 60 * 60 * 24 * 30) ...
   */
  /* setcookie("intra_siddd", "$data[0]", time()+60*60*24*30, "/"); */

  
  setcookie ("intra_sid", "$data[0]", time()+60*60*24*30, "/");
  setcookie ("intra_cdata", "$data[1]", time()+60*60*24*30, "/");
  
}

function delog_session($login)
{
  global $db_link;

  $login = addslashes($login);
  $id = intval($_COOKIE["intra_sid"]);
  $cookie_data = addslashes($_COOKIE["intra_cdata"]);

  $res = db_query("UPDATE session, user ".
		  "SET session.valid=0 ".
		  "WHERE session.id=".$id." ".
		  "AND   user.login='".$login."' ".
		  "AND   session.uid=user.id ".
		  "AND   session.cookie_data='".$cookie_data."' ".
		  "AND   session.valid>0",
		  $db_link);
}

$time = 0;

function del_session_cookie()
{
  global $_login;
  global $time;

  setcookie("intra_sid", "",  time()+60*60*24*30, "/");
  setcookie("intra_cdata",  "",  time()+60*60*24*30, "/");
  /*  print "login: ". $_login . "\n"; */
  /* var_dump(debug_backtrace()); */
  /* phpinfo(); */
}

$msg = "";

function check_update_session_cookie($login)
{
  global $time;
  global $db_link;
  global $msg;
  global $_COOKIE;

  $login = addslashes($login);
  $id = intval($_COOKIE["intra_sid"]);
  $cookie_data = addslashes($_COOKIE["intra_cdata"]);

  if (!strlen($id) || !strlen($cookie_data))
    {
      del_session_cookie();
      return (1);
    }

  /* recherche de la session */

  $res = db_query("SELECT session.id ".
		  "FROM session, user ".
		  "WHERE session.id=".$id." ".
		  "AND   user.login='".$login."' ".
		  "AND   session.uid=user.id ".
		  "AND   session.cookie_data='".$cookie_data."' ".
		  "AND   session.valid>0 ".
		  "AND    UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(session.log_date) ".
		  "       < (session.log_period+".LOGIN_TIMEOUT.") ".
		  "ORDER BY session.id DESC LIMIT 1 ",
		  $db_link);

  if (db_num_rows($res) < 1)
    {
      /* surement expire */
      del_session_cookie();
      return (1);
    }

  db_query("UPDATE session SET log_period=UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(log_date) WHERE id=".$id,
	   $db_link);

  return (0);
}

function display_login_page($message)
{
  print ("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n");
  print ("<html><head><TITLE> Stechec: Gestionnaire de concours tout à tour </TITLE>");
  if ($message)
    $_login_message = $message;
  style_init(1);
  include "content/login.php";
  print("</body>");
  die;
}

function do_auth()
{
  global $_login;
  global $_pass;
  global $_action;
  global $msg;

  /*
  if (isset($_POST["sendpass"]))
  {
    $query = db_query("SELECT gpg_ok FROM user WHERE `login`='$_login'", $db_link);
    $row = db_fetch_row($query);
    if ($row[0] == 0)
    {
      display_login_page("You're not authorized to get a new password.");
    }
    else
    {
      download_pass($_login);
    }
  }
  */
  if ($_action == "register")
  {
	print "beautiful !!! ";
  }

  if ($_action == "delog")
    {
      delog_session($_login);
      del_session_cookie();
      display_login_page("");
    }

  
  if (!strlen($_login))
    {
      del_session_cookie(); 
      display_login_page("");
    }

  /* expired */
  switch (check_update_session_cookie($_login))
  #switch (0)
  {
    /*
  case (0):
    break;
    */
  case (2):
    del_session_cookie();
    display_login_page("Your session has expired");
    
  case (1):
    if (!strlen($_pass))
    {
      print $_pass."</br>";
      display_login_page("Please login. $msg");
      break ;
    }
    switch (check_password($_login, $_pass))
    {
    case 2:
      del_session_cookie();
      display_login_page("ldap server error");
      break;
    case 0:
      del_session_cookie();
      display_login_page("Authentication failed.");
      break;
    default:
      add_session_cookie($_login);
      break;
    }
  }
}

?>
