<?php
setcookie ('lang', $_POST['lang'], time()+31536000, '/', '.towbowltactics.com', '0');
header("Location: http://www.towbowltactics.com/snore/");
?>
