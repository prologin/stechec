<?php

/*

 Stechec project is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 The complete GNU General Public Licence Notice can be found as the
 `NOTICE' file in the root directory.

 Copyright (C) 2007 Prologin

*/


include "include/meta.php";

function show_machine()
{
  style_title("Liste des machines");

  $res = array();
  $msg = get_ctl_data("machine list");

  $a = explode("\n", $msg); 
  foreach ($a as $l) {
    $e = explode(" ", $l);
    if (count($e) == 6) {
      if ($e[5] < 1) {
        $e[5] = 1;
      }
      array_push($res,
                 array('h' => $e[0],
                       'p' => $e[1],
                       'st' => $e[2],
                       'he' => date("H:i:s", $e[3]),
                       'sl' => $e[4],
                       'r' => $e[5]));
    }
  }

  list_disp_data($res,
            array(
                  array("h", "Host", "text", ACL_ALL, 0, array("center")),
                  array("p", "Port", "text", ACL_ALL, 0, array("center")),
                  array("st", "Status", "bool", ACL_ALL, array("Down", "Up"), array("center_red", "center_green")),
                  array("he", "Dernier ping", "text", ACL_ALL, 0, array("center")),
                  array("sl", "Slots", "text", ACL_ALL, 0, array("center")),
                  array("r", "Remplissage", "percent", ACL_ALL),
                  ),
            array("gray", "red"),
            "aborted");
}

show_machine();

?>
