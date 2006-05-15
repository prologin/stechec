<?

	$tmpfname = "xml/tmpf".time().".xml";
	$fp = fopen($tmpfname, "w+");
	fwrite($fp,'<?xml version="1.0" ?>'."\n");
	
	fwrite($fp,'<formations>'."\n");

	for($i=1;$i<=16;$i++){
		$tmp = "a".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'  <formation id="1" n="'.$i.'" col="'.$m[0].'" row="'.$m[1].'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}
	fwrite($fp,"\n");

	for($i=1;$i<=16;$i++){
		$tmp = "b".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'  <formation id="2" n="'.$i.'" col="'.$m[0].'" row="'.$m[1].'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}
	fwrite($fp,"\n");

	for($i=1;$i<=16;$i++){
		$tmp = "c".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'  <formation id="3" n="'.$i.'" col="'.$m[0].'" row="'.$m[1].'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}
	fwrite($fp,"\n");

	for($i=1;$i<=16;$i++){
		$tmp = "d".$i;
		if($$tmp) {
			$m = split(',',$$tmp);
			fwrite($fp,'  <formation id="4" n="'.$i.'" col="'.$m[0].'" row="'.$m[1].'" x="'.$m[0].'" y="'.$m[1].'" />'."\n");
		}
	}
	fwrite($fp,"\n");

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
			echo "<hr>Si le fichier formations.xml ne s'affiche pas correctement, celui-ci est disponible <a href='".$tmpfname."'>ICI</a>.<br>(Clic-droit, \"save link as\" pour le recuperer)</body></html>";
		} 
	}
?>
