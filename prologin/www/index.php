<?php

session_start();

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


//////// MODE MAINTENANCE //////
$maintenance = FALSE;
///////////////////////////////

function strip_int ()
{
  // Variable _old_trace added by Olivier Jacob for upload needs
  $vars = array("_entry", "_info_id", "_promo", "_limit", "_old_trace");

  for ($i = 0; $i < count($vars); $i++)
    $GLOBALS[$vars[$i]] = intval($GLOBALS[$vars[$i]]);
}

$post_data = array("_login" => $_login);

strip_int();

if (intval($_promo))
     $post_data["_promo"] = $_promo;

/*** DB init ***/

include "include/sql_wrapper.php";
db_init();

/*** User data init ***/

include "include/style.php";
include "include/access.php";
include "include/user.php";
include "include/table.php";
include "include/auth.php";


// BEGIN MAINTENANCE MODE
if ($maintenance && access_check(ACL_CANDIDAT)) {
   include "content/blue.php";
   die;
}
// END MAINTENANCE MODE

user_register($_login);
do_auth();


/* want delog ? */

user_init($_login);

if (strlen($_dl_type))
{
  include "include/download.php";
  download($_dl_type, intval($_entry));
  die;
}


/*** Style init ***/

print ("<!doctype html public \"-//w3c//dtd html 4.0 transitional//en\">\n");
print ("<html>");
print ("<meta http-equiv=\"Content-Type\" content=\"text/html;charset=iso-8859-1\" />");
print ("<head><TITLE> Stechec: Gestionnaire de concours tour à tour </TITLE>"); /* Document begin */


style_init($user["id_style"]);

/*** Menu init ***/

include "include/menu.php";


$page = page_get(intval($_id_page));
$post_data["_id_page"] = $page["id_page"];
?>

  <meta name='keywords' content='Stechec, concours'>
</head>

<body marginwidth=0 marginheight=0 topmargin=0 rightmargin=0 leftmargin=0 bottommargin=0>

<table cellspacing=0 cellpadding=0 width='100%' cols=2 class=title_zone>

  <tr>
    <td class=title_zone colspan=2>
      <?php include "content/head.php"; ?>
    </td>
  </tr>

</table>



<table cellspacing=0 cellpadding=0 width='100%' cols=2>
  <tr>
    <td colspan="2">
      <?php menu_bar($page); ?>
    </td>
  </tr>
</table>


<table cellspacing=0 cellpadding=0 width='100%' height="90%" cols=2>
  <tr>
    <td class=left_margin valign=top>
      <?php include "content/margin.php"; ?>
    </td>

    <td class=content align=left valign=top>

<?php

    if (isset($_use_profil) && $_use_profil != "exit")
     style_links(array(array("Retourner en mode administrateur", ACL_ALL,
			     array("_use_profil" => "exit"))));


     if ((intval($page["access_level"]) & intval($profil["access"]))
	 && file_exists("content/".$page[file])) {

       // fixme delex
       // print "$page[file] </br>";

       include "content/".$page[file];
     }



?>
    <br>
    </td>
  </tr>

</table>


</body>
</html>
