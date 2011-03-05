<?

function removeaccents($str){ 
	$str = strtr($str,"ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ","SOZsozYYuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy");
	return $str; 
} 

	$tmpfname = "../xml/tmp".time().".xml";
	$fp = fopen($tmpfname, "w+");

	if(!$sorc) $sorc=0; 
	if(!$apo) $apo=0; 
	if(!$ppg) $ppg=0; 
	if(!$ass) $ass=0; 
	if(!$cla) $cla=100; 
	if(!$pop) $pop=0; 
	if(!$rel) $rel=0; 
	if(!$x_rel) $x_rel=0; 
	if(!$tre) $tre=0; 
	if(!$r) $r=0; 

//	include("teams/".$r.".inc.php");
	include("races.inc.php");

	if($race_tbt[$r]=='') 
		$filename="no_emblem.gif";
	else 
		$filename=$race_tbt[$r].".jpg";
	
	if( $emb ) $filename=$emb;

	fwrite($fp,'<?xml version="1.0" ?>'."\n");
	fwrite($fp, "<team name=\"".removeaccents($nom_eq)."\" race=\"".removeaccents($race_singulier_tbt[$r])."\" coach=\"".removeaccents($coach)."\" BBversion=\"4\" emblem=\"".removeaccents($filename)."\">\n");
	fwrite($fp, "<rating>$cla</rating>\n");
	fwrite($fp, "<treasury>$tre</treasury>\n");
	fwrite($fp, "<reroll>$rel</reroll>\n");
	fwrite($fp, "<rerollcost>$x_rel</rerollcost>\n");
	fwrite($fp, "<fanfactor>$pop</fanfactor>\n");
	fwrite($fp, "<assistant>$ass</assistant>\n");
	fwrite($fp, "<cheerleader>$ppg</cheerleader>\n");
	fwrite($fp, "<apothecary>$apo</apothecary>\n");
	fwrite($fp, "<wizard>$sorc</wizard>\n");
	fwrite($fp, "<formation>default.xml</formation>\n");

	fwrite($fp, "<players>\n");
	
	for($j=1;$j<=16 ;$j++) { 

		$tmp = "p".$j; $var_p = $$tmp;
		$tmp = "n".$j; $var_n = $$tmp;
		$tmp = "ma".$j; $var_ma = $$tmp;
		$tmp = "st".$j;	$var_st = $$tmp;
		$tmp = "ag".$j;	$var_ag = $$tmp;
		$tmp = "av".$j;	$var_av = $$tmp;
		$tmp = "c".$j;	$var_c = $$tmp;
		$tmp = "bl".$j;	$var_bl = $$tmp;
		$tmp = "reu".$j; $var_reu = $$tmp;
		$tmp = "td".$j;	$var_td = $$tmp;
		$tmp = "int".$j; $var_int = $$tmp;			
		$tmp = "sor".$j; $var_sor = $$tmp;
		$tmp = "jpv".$j; $var_jpv = $$tmp;
		$tmp = "xp".$j;	$var_xp = $$tmp;
		$tmp = "cout".$j; $var_cout = $$tmp;
		$var_n = ($var_n)?($var_n):("");
		$var_p = ($var_p)?($var_p):("---");
		$var_ma = ($var_ma)?($var_ma):(0);
		$var_st = ($var_st)?($var_st):(0);
		$var_ag = ($var_ag)?($var_ag):(0);
		$var_av = ($var_av)?($var_av):(0);
		$var_c = ($var_c)?($var_c):("");
		$var_bl = ($var_bl)?($var_bl):(0);
		$var_reu = ($var_reu)?($var_reu):(0);
		$var_td = ($var_td)?($var_td):(0);
		$var_int = ($var_int)?($var_int):(0);
		$var_sor = ($var_sor)?($var_sor):(0);
		$var_jpv = ($var_jpv)?($var_jpv):(0);
		$var_xp = ($var_xp)?($var_xp):(0);			
		$var_cout = ($var_cout)?($var_cout):(0);
		
		if($var_n && $var_p && $var_cout ){
			fwrite($fp, "<player name=\"".removeaccents($var_n)."\" number=\"$j\" position=\"".removeaccents($var_p)."\" race=\"".removeaccents($race_singulier_tbt[$r])."\" display=\"".$display[$race_singulier_tbt[$r]][$var_p]."\">\n");
			fwrite($fp, "<ma>$var_ma</ma>\n");
			fwrite($fp, "<st>$var_st</st>\n");
			fwrite($fp, "<ag>$var_ag</ag>\n");
			fwrite($fp, "<av>$var_av</av>\n");
			fwrite($fp, "<positionid>".$positionid[$race_singulier_tbt[$r]][$var_p]."</positionid>\n");
			fwrite($fp, "<cost>$var_cout</cost>\n");
	
			if($var_c) {
				$skills = split(",",$var_c);
				for($s=0;$s<count($skills);$s++){
					fwrite($fp, "<skill>".$skills[$s]."</skill>\n");
				}
			}

			fwrite($fp, "<bl>$var_bl</bl>\n");
			fwrite($fp, "<reu>$var_reu</reu>\n");
			fwrite($fp, "<td>$var_td</td>\n");
			fwrite($fp, "<int>$var_int</int>\n");
			fwrite($fp, "<sor>$var_sor</sor>\n");
			fwrite($fp, "<jpv>$var_jpv</jpv>\n");
			fwrite($fp, "<xp>$var_xp</xp>\n");

			fwrite($fp, "</player>\n");
		}
		else fwrite($fp, "<!-- Ligne $j incomplete -->\n");
	}
		
	fwrite($fp, "</players></team>\n");

	fclose($fp);

	if($dl_xml) {
		header("Content-Disposition: attachment; filename=team_".removeaccents($nom_eq).".xml");
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
			echo "<hr>If your file formations.xml does not work, it is aslo available at <a href='http://toweld.free.fr/towbowltactics/teamroster/$tmpfname'>ICI</a>.<br>(Right-click, \"save link as\" to recover it)</body></html>";
		} 
	}

?>
