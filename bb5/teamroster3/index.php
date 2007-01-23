<?php
require_once('backstage/minixml.php');
require_once('backstage/parse_xml.php');
require_once('backstage/helper.php');

$lang = checkLang();
$interface = parseInterface('data/'.$lang.'/index.xml');

header('Content-type: text/html; charset="UTF-8"');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>

	<title>SNORE - TBT Roster Editor</title>
	
	<meta name="author" content="Marc-Antonio Bisotti"/>
	<meta name="date" content="2006-11-12T16:15:15+0100"/>
	<meta name="copyright" content="Gnu Public License" />
	<meta name="keywords" content="Blood Bowl Tow Bowl Tactics Team Editor Roster Editor" />
	<meta name="description" content="Manages Tow Bowl Tactics Teamsheets according to the LRB 5.0" />
	<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
	
	<link rel="stylesheet" type="text/css" media="screen" href="index.css" title="Default Style" />
	
</head>

<body>

<div id="title">

	<div id="flags" style="float: right;">
	  <form action="set_cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="en"></input>
	  <button type="submit"><img src="data/en/flag.jpg" /></button>
	  </form>
	  <form action="set_cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="fr"></input>
	  <button type="submit"><img src="data/fr/flag.jpg" /></button>
	  </form>
	  <form action="set_cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="de"></input>
	  <button type="submit"><img src="data/de/flag.jpg" /></button>
	  </form>
	</div>
	
	<h1>TBT - SNORE</h1>
	<p class="subtitle">the &quot;Super New Online Roster Editor&quot;</p>

</div>

<div id="content">

<p><?php sprint($interface['intro']) ?></p>

<h2><?php sprint($interface['heading'][0]) ?></h2>

	<div id="list">

		<?php
			$races = parseRaces('data/'.$lang.'/races.xml');
			$races_list = listRaces($races);
			
			foreach ( $races_list as $race ) {
				echo "<a class=\"block\" href=\"roster.php?race=$race\">$race</a> \n";
			}
		?>

	</div>

<h2><?php sprint($interface['heading'][1]) ?></h2>

<form action="roster.php" method="post" enctype="multipart/form-data">
    <p><input type="hidden" name="MAX_FILE_SIZE" value="30000" />
    <input type="hidden" name="upload" value="true" /></p>
    <p><?php sprint($interface['upload']['label']) ?> <input name="userfile" type="file" />
    <input type="submit" value="<?php echo $interface['upload']['submit'] ?>" /></p>
</form>

</div>

<div id="foot">
<p>
 	<a href="http://jigsaw.w3.org/css-validator/check/referer">
  	<img style="border:0;width:88px;height:31px"
       src="http://jigsaw.w3.org/css-validator/images/vcss" 
       alt="Valid CSS!" />
	</a>
    <a href="http://validator.w3.org/check?uri=referer">
    <img style="border:0;width:88px;height:31px"
        src="http://www.w3.org/Icons/valid-xhtml10"
        alt="Valid XHTML 1.0 Strict" height="31" width="88" />
	</a>
</p>
</div>

</body>

</html>