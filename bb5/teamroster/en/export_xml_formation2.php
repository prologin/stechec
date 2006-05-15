<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<?

	$tmpfname = "../xml/tmpf".time().".xml";
	$fp = fopen($tmpfname, "w+");
	fwrite($fp,'<?xml version="1.0" ?>'."\n");
	
	fwrite($fp,'<formations>'."\n");

	for($y=0;$y<13;$y++) {
		for($x=0;$x<15;$x++) { 
			$tmp = "a".$x."_".$y;
			if($$tmp) {
				fwrite($fp,'<formation id="1" n="'.$$tmp.'" x="'.$x.'" y="'.$y.'" />'."\n");	
			}
			$tmp = "b".$x."_".$y;
			if($$tmp) {
				fwrite($fp,'<formation id="2" n="'.$$tmp.'" x="'.$x.'" y="'.$y.'" />'."\n");	
			}
			$tmp = "c".$x."_".$y;
			if($$tmp) {
				fwrite($fp,'<formation id="3" n="'.$$tmp.'" x="'.$x.'" y="'.$y.'" />'."\n");	
			}
			$tmp = "d".$x."_".$y;
			if($$tmp) {
				fwrite($fp,'<formation id="4" n="'.$$tmp.'" x="'.$x.'" y="'.$y.'" />'."\n");	
			}
		}
	}


	fwrite($fp,'</formations>'."\n");
	fclose($fp);

	echo "<html>";
	?>
<HEAD>
<TITLE>Editeur de formation</TITLE>
<META NAME="Generator" CONTENT="EditPlus">
<META NAME="Author" CONTENT="toweld@rocketmail.com">
<META NAME="Keywords" CONTENT="tbt,tow,bowl,tactics,editeur,formation,xml">
<META NAME="Description" CONTENT="Formation Editor for Tow Bowl Tactics">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
</HEAD>

	<?
	echo "<body><pre>";
	$fp = fopen($tmpfname,"r");
	while ($fp && !feof ($fp)) {
	    $buffer = fgets($fp, 4096);
		echo htmlentities($buffer);
	}
	fclose($fp);
	echo "</pre>";
	echo "<hr>If your file formations.xml does work, it is aslo available at <a href='http://toweld.free.fr/towbowltactics/teamroster/$tmpfname'>ICI</a>.<br>(Right-click, \"save link as\" to recover it)";
?>
	<p>
    <a href="http://validator.w3.org/check/referer"><img border="0"
        src="http://www.w3.org/Icons/valid-html40"
        alt="Valid HTML 4.0!" height="31" width="88"></a></p>
<?
	echo "</body></html>";
?>