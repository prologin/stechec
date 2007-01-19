<?php
setcookie ('lang', $_POST['lang'], time()+60*60*24*30, '/', 'towbowltactics.com', '0');

header("Location: http://www.towbowltactics.com/teamroster3/");

?>
