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
?>
<?php



function margin_disp()
{
  global $db_link;
  global $_login;
  global $user;

  /*
   * recuperation du libelle du profil actif de la personne.
   */
  $libelle = db_fetch_row(db_query("SELECT libelle FROM profil WHERE id = ".$user["id_profil"].";"));


  print "<table class=table_margin>";

  print ("<tr><td align=center>".
	 "Salut <b> $user[nickname] !</b> <br>".
	 "$libelle[0]<br>".
	 "</td></tr>");


  print "<tr><td align=center>";
  style_links(array(array("&lt;Logout&gt;", ACL_ALL,
                          array("_action" => "delog"))));

  print "</td></tr>";

  print "</table>";

}

margin_disp();

?>

