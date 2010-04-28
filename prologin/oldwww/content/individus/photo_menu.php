<?
/*
 * menu_photo.php.
 * 
 * If the user has the correct acl, this page displays a menu where it
 * is possible to choose an item to display.
 *
 * July 2004, by nicolas schichan.
 *
 */

global $user_profil;

/*
 * check if we are either assistant, prof or adm staff ...
 */
if (access_check(ACL_NOTELEVE)) {
  $promo = $user_profil["promo"];


  $links = array(
		array("Candidat", 24),
		array("Organisateur", 241),
		);

  $virgin = 1;
  $out = "";
  print "<div class=text>\n";
  foreach ($links as $i) {
    print "<a class=links href="; href(array("_id_page" => $i[1])); print ">$i[0]</a> ";
  }
  print "</div>\n";
}


?>
