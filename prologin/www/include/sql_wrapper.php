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

include ("passwords.php");

function db_date($f1, $f2)
{
  return " DATE_FORMAT(".$f1.", '%d/%m/%Y') AS ".$f2;
}

function db_time($f1, $f2)
{
  return " DATE_FORMAT(".$f1.", '%d/%m/%Y %H:%i:%s') AS ".$f2;
}

function db_hmstime($f1, $f2)
{
  return " DATE_FORMAT(".$f1.", '%H:%i:%s') AS ".$f2;
}

function db_init()
{
  global $db_link;
  global $db_ip;
  global $db_user;
  global $db_pass;
  global $db_databases;

  if ($db_link = mysql_connect($db_ip, $db_user, $db_pass))
    {
      mysql_select_db($db_databases, $db_link);
    }
  else
    {
      die("sql_wrapper.php: unable to connect");
    }

  return ($db_link);
}

function db_query($query)
{
  global $db_link;

  $res = mysql_query($query, $db_link);
  // FIXME: Ne pas laisser
  //print "$query.</br>";
  print mysql_error();
  return $res;
}

function db_fetch_array($result)
{
  return mysql_fetch_array($result);
}

function db_fetch_row($result)
{
  return mysql_fetch_row($result);
}

function db_seek($result, $pos)
{
  mysql_num_rows($result);
  return mysql_data_seek($result, $pos);
}

function db_num_rows($result)
{
  return mysql_num_rows($result);
}

function db_insert_id()
{
  global $db_link;
  return mysql_insert_id($db_link);
}

function db_print($table, $cond, $field)
{
  global $db_link;

  $res = mysql_query("SELECT $field FROM $table WHERE $cond", $db_link);
  $row = mysql_fetch_row($res);
  print($row[0]);
}

?>
