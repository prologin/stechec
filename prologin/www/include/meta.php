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

function get_ctl_data($command) {
  $sock = socket_create(AF_UNIX, SOCK_STREAM, 0);
  if (socket_connect($sock, "/tmp/.sun_meta") == false) {
    print "error opening socket: " . socket_strerror(socket_last_error()) . "\n";
    return;
  }

  socket_write($sock, $command);
  while (($l = socket_read($sock, 1024, PHP_BINARY_READ)) && $l != "") {
    $msg .= $l;
  }
  return $msg;
}

?>
