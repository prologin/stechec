<?php
header("Content-type: text/html; charset=UTF-8");
?>
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

<div id="title">

	<h1>TBT - SNORE</h1>
	<p class="subtitle">the &quot;Super New Online Roster Editor&quot;</p>

</div>

<div id="content">

<p>This site will help you to manage your Tow Bowl Tactics Teamroster. 
You can either start with a new team, choosing one of the following races, 
or load a previously saved teamsheet in the xml format.</p>

<h2>Start a fresh team.</h2>

<table class="list">
<tr>
	<td><a class="block" href="roster.php?race=Amazon">Amazon</a></td>
	<td><a class="block" href="roster.php?race=Halfling">Halfling</a></td>
	<td><a class="block" href="roster.php?race=Nurgle">Nurgle</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Chaos">Chaos</a></td>
	<td><a class="block" href="roster.php?race=High Elf">High Elf</a></td>
	<td><a class="block" href="roster.php?race=Ogre">Ogre</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Chaos Dwarf">Chaos Dwarf</a></td>
	<td><a class="block" href="roster.php?race=Human">Human</a></td>
	<td><a class="block" href="roster.php?race=Orc">Orc</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Dark Elf">Dark Elf</a></td>
	<td><a class="block" href="roster.php?race=Khemri">Khemri</a></td>
	<td><a class="block" href="roster.php?race=Skaven">Skaven</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Dwarf">Dwarf</a></td>
	<td><a class="block" href="roster.php?race=Lizardman">Lizardman</a></td>
	<td><a class="block" href="roster.php?race=Undead">Undead</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Elf">Elf</a></td>
	<td><a class="block" href="roster.php?race=Necromantic">Necromantic</a></td>
	<td><a class="block" href="roster.php?race=Vampire">Vampire</a></td>
</tr>
<tr>
	<td><a class="block" href="roster.php?race=Goblin">Goblin</a></td>
	<td><a class="block" href="roster.php?race=Norse">Norse</a></td>
	<td><a class="block" href="roster.php?race=Wood Elf">Wood Elf</a></td>
</tr>
</table>

<?php

//	You can use this code if you want to recreate the list above

/*

	require_once('minixml.inc.php'); // XML Framework
	
	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile('races.xml');
	$races = $xmlDoc->toArray();
	
	for ( $i = 0; $i < $races['races']['race']['_num']; $i++) {
		$name =  $races['races']['race'][$i]['_attributes']['name'];
		echo "<a class="block"=\"roster.php?race=$name\">$name</a> \n";
	}
	
*/
 
?>

<h2>Load a saved team.</h2>

<form action="roster.php" method="post" enctype="multipart/form-data">
    <p><input type="hidden" name="MAX_FILE_SIZE" value="30000" />
    <input type="hidden" name="upload" value="true" /></p>
    <p>Send this file: <input name="userfile" type="file" />
    <input type="submit" value="Send File" /></p>
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