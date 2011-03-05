<?

	$tmpfname = "../xml/tmpf".time().".xml";
	$fp = fopen($tmpfname, "w+");
	fwrite($fp,'<?xml version="1.0" ?>'."\n");
	
	fwrite($fp,'<formations>'."\n");

	for($i=1;$i<=16;$i++){
		$tmp = "a".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'<formation id="1" n="'.$i.'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}

	for($i=1;$i<=16;$i++){
		$tmp = "b".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'<formation id="2" n="'.$i.'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}

	for($i=1;$i<=16;$i++){
		$tmp = "c".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'<formation id="3" n="'.$i.'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}

	for($i=1;$i<=16;$i++){
		$tmp = "d".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'<formation id="4" n="'.$i.'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}

	fwrite($fp,'</formations>'."\n");
	fclose($fp);

	if($dl_xml) {
		header("Content-Disposition: attachment; filename=formations.xml");
		header("Content-type: application/x-octet-stream\n\n");
		readFile($tmpfname);
	}
	else {
		if (stristr($_SERVER['HTTP_USER_AGENT'],"MSIE") && !stristr($_SERVER['HTTP_USER_AGENT'],"Opera")){ 
			header("Content-type: text/xml\n\n");
			readFile($tmpfname);
		} 
		else { 
			echo "<html><body><pre>";
			$fp = fopen($tmpfname,"r");
			while ($fp && !feof ($fp)) {
			    $buffer = fgets($fp, 4096);
				echo htmlentities($buffer);
			}
			fclose($fp);
			echo "</pre>";
			echo "<hr>If the xml file does not download correctly, it is also available at <a href='http://toweld.free.fr/towbowltactics/teamroster/$tmpfname'>ICI</a>.<br>(Right-Click, \"save link as\" to receive it)</body></html>";
		} 
	}
?>