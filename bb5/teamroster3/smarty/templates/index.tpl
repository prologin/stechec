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
	  <form action="index.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="en"></input>
	  <button type="submit"><img src="data/en/flag.jpg" /></button>
	  </form>
	  <form action="index.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="fr"></input>
	  <button type="submit"><img src="data/fr/flag.jpg" /></button>
	  </form>
	  <form action="index.php" method="POST" style="float: left">
	  <input type="hidden" name="lang" value="de"></input>
	  <button type="submit"><img src="data/de/flag.jpg" /></button>
	  </form>
	</div>
	
	<h1>TBT - SNORE</h1>
	<p class="subtitle">the &quot;Super New Online Roster Editor&quot;</p>

</div>

<div id="content">

<p>{$text->intro}</p>

<h2>{$text->new}</h2>

	<div id="list">
	{foreach from=$races key=race_en item=race_lang}
	<a class="block" href="roster.php?race={$race_en}">{$race_lang}</a>
	{/foreach}
	</div>

<h2>{$text->load}</h2>

	<form action="roster.php" method="post" enctype="multipart/form-data">
	    <p><input type="hidden" name="MAX_FILE_SIZE" value="30000" />
	    <input type="hidden" name="upload" value="true" /></p>
	    <p>{$text->upload} <input name="userfile" type="file" />
	    <input type="submit" value="{$text->submit}" /></p>
	</form>

</div>

<div id="foot">
<p>
 Rev. {$revision}
</p>
</div>

</body>

</html>
