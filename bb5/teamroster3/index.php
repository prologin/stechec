<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
    <title>Teamroster</title>
    <link rel="stylesheet" type="text/css" media="screen" href="style.css" title="Default" />
</head>

<body>
    
<h1>Tow Bowl Tactics Teameditor</h1>

<p>Start a new team, choosing one of these races:</p>

<ol>
<li><a href="roster.php?teamname=Amazon">Amazon</a></li> 
<li><a href="roster.php?teamname=Chaos">Chaos</a></li> 
<li><a href="roster.php?teamname=Chaos Dwarf">Chaos Dwarf</a></li> 
<li><a href="roster.php?teamname=Dark Elf">Dark Elf</a></li> 
<li><a href="roster.php?teamname=Dwarf">Dwarf</a></li> 
<li><a href="roster.php?teamname=Elf">Elf</a></li> 
<li><a href="roster.php?teamname=Goblin">Goblin</a></li> 
<li><a href="roster.php?teamname=Halfling">Halfling</a></li> 
<li><a href="roster.php?teamname=High Elf">High Elf</a></li> 
<li><a href="roster.php?teamname=Human">Human</a></li> 
<li><a href="roster.php?teamname=Khemri">Khemri</a></li> 
<li><a href="roster.php?teamname=Lizardman">Lizardman</a></li> 
<li><a href="roster.php?teamname=Necromantic">Necromantic</a></li> 
<li><a href="roster.php?teamname=Norse">Norse</a></li> 
<li><a href="roster.php?teamname=Nurgle">Nurgle</a></li> 
<li><a href="roster.php?teamname=Ogre">Ogre</a></li> 
<li><a href="roster.php?teamname=Orc">Orc</a></li> 
<li><a href="roster.php?teamname=Skaven">Skaven</a></li> 
<li><a href="roster.php?teamname=Undead">Undead</a></li> 
<li><a href="roster.php?teamname=Vampire">Vampire</a></li> 
<li><a href="roster.php?teamname=Wood Elf">Wood Elf</a></li>
</ol>

<p>Or load an existing team!</p>
<!-- The data encoding type, enctype, MUST be specified as below -->
<form enctype="multipart/form-data" action="roster2.php" method="POST">
    <!-- MAX_FILE_SIZE must precede the file input field -->
    <input type="hidden" name="MAX_FILE_SIZE" value="30000" />
    <input type="hidden" name="upload" value="true" />
    <!-- Name of input element determines name in $_FILES array -->
    Send this file: <input name="userfile" type="file" />
    <input type="submit" value="Send File" />
</form>

<?php
/*  If you want to parse the races.xml again and generate the list above
 *  just let the script below run and copy$paste the source
 *
 *  require_once('minixml.inc.php'); // XML Framework
 *  $xmlDoc = new MiniXMLDoc();
 *  $xmlDoc->fromFile('races.xml');
 *  $races = $xmlDoc->toArray(); 
 *
 *  for ( $i = 0; $i < $races['races']['race']['_num']; $i++) {
 *      $name =  $races['races']['race'][$i]['_attributes']['name'];
 *      echo "<li><a href=\"roster.php?teamname=$name\">$name</a></li> \n";
 *  }
 */
?>
</body>
