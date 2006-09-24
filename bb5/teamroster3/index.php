<?php header('Content-type: text/html; charset="UTF-8"'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>

	<title>SNORE - TBT Roster Editor</title>
	
	<meta name="author" content="Marc-Antonio Bisotti"/>
	<meta name="date" content="07.09.2006" />
	<meta name="copyright" content="Gnu Public License" />
	<meta name="keywords" content="Blood Bowl Tow Bowl Tactics Team Editor Roster Editor" />
	<meta name="description" content="Manages Tow Bowl Tactics Teamsheets according to the LRB 5.0" />
	<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
	
	<link rel="stylesheet" type="text/css" media="screen" href="includes/index.css" title="Default Style" />
	
</head>

<body>

<?php

	require_once('includes/minixml.php');
	require_once('includes/parse_xml.php');
	
	if ( isset($_COOKIE['lang']) ) {
	
		$lang = $_COOKIE['lang'];
		
		if ( !($lang == 'en' || $lang == 'fr' || $lang == 'de') ) {
			$lang = 'en';
		}
		
	}
	else {
		$lang = 'en';
	}

	$interface = parseInterface('interface_'.$lang.'.xml');

	function sprint($string) {	// sprint like in "Special characters PRINT"
		echo htmlentities($string, ENT_QUOTES, 'UTF-8');
	}

?>

<div id="title">

	<div id="flags" style="float: right;">
	  <form action="cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="en"></input>
	  <button type="submit"><img src="english_flag.jpg" /></button>
	  </form>
	  <form action="cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="fr"></input>
	  <button type="submit"><img src="french_flag.jpg" /></button>
	  </form>
	  <form action="cookie.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="de"></input>
	  <button type="submit"><img src="german_flag.jpg" /></button>
	  </form>
	</div>
	
	<h1>TBT - SNORE</h1>
	<p class="subtitle">the &quot;Super New Online Roster Editor&quot;</p>

</div>

<div id="content">

<p><?php sprint($interface['index']['intro']) ?></p>

<h2><?php sprint($interface['index']['heading'][0]) ?></h2>

	<div id="list">

		<?php
		
			$xmlDoc = new MiniXMLDoc();
			$xmlDoc->fromFile('races.xml'); // to be changed wenn new races.xml arrive
			$races = $xmlDoc->toArray();
			
			for ( $i = 0; $i < $races['races']['race']['_num']; $i++) {
				$name =  $races['races']['race'][$i]['_attributes']['name'];
				echo "<a class=\"block\" href=\"roster.php?race=$name\">$name</a> \n";
			}
			
		 
		?>

	</div>

<h2><?php sprint($interface['index']['heading'][1]) ?></h2>

<form action="roster.php" method="post" enctype="multipart/form-data">
    <p><input type="hidden" name="MAX_FILE_SIZE" value="30000" />
    <input type="hidden" name="upload" value="true" /></p>
    <p><?php sprint($interface['index']['upload']['label']) ?> <input name="userfile" type="file" />
    <input type="submit" value="<?php echo $interface['index']['upload']['submit'] ?>" /></p>
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
