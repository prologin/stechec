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

include "include/notes.php";

function disp_my_access()
{
  global $db_link;
  global $user;

  style_title("Mes log de connections");

  $list_note = array(
		    array("log_date",		"Date",		"datetime", ACL_ALL,
			  0, array("center")),
		    array("period",		"Duree",	"text", ACL_ALL,
			  0, array("center")),
		    array("log_host",		"Host",		"text", ACL_ALL),
		    array("valid",		"Deconnection",	"bool", ACL_SITEADM,
			  array("Logout", ""), array("center")),
		    );

  $q1 = db_query("SELECT ".db_time("log_date", "log_date").", valid, ".
		 "CONCAT(FLOOR(log_period/60), ':', log_period%60) AS period, log_host ".
		 "FROM   session ".
		 "WHERE  session.uid=".$user["id"]." ".
		 "ORDER BY session.log_date DESC LIMIT 30",
		 $db_link);

  list_disp($q1, $list_note, array("gray", "bluegray"), "even");
}

disp_my_access();

?>

