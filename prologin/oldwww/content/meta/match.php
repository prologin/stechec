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

function show_match()
{
  style_title("Liste des matchs en cours");

  $res = array();
  $msg = get_ctl_data("match list");

  $a = explode("\n", $msg); 
  foreach ($a as $l) {
    $e = explode(" ", $l);
    if (count($e) == 4) {
      array_push($res,
                 array('i' => $e[0],
                       't' => $e[1],
                       's' => date("d/m/y H:i:s", $e[2]),
                       'sl' => $e[3]));
    }
  }

  list_disp_data($res,
            array(
                  array("i", "Id", "text", ACL_ALL, 0, array("center")),
                  array("t", "Type", "text", ACL_ALL, 0, array("center")),
                  array("s", "Démarré le", "text", ACL_ALL, 0, array("center")),
                  array("sl", "Slots", "text", ACL_ALL, 0, array("center")),
                  ),
            array("gray", "red"),
            "aborted");
}

show_match();

?>
