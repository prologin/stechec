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

function show_machine_summary()
{
  style_title("Machine");

  $msg = get_ctl_data("machine summary");
  $e = explode(" ", $msg);

  style_text("Machines connectés: $e[0]<br>" .
             "Slots par machine: $e[1]<br>" .
             "Slots disponibles: $e[2]");
}

function show_match_summary()
{
  style_title("Matchs");

  $msg = get_ctl_data("match summary");
  $e = explode(" ", $msg);
  
  style_text("Matchs en cours: $e[0]\n");
}

show_machine_summary();
show_match_summary();

?>
