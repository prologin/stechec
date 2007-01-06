<?php
setcookie ('lang', $_POST['lang'], time()+60*60*24*30, '/', 'localhost', '0');

header("Location: http://localhost/~user/teamroster/");
?>
