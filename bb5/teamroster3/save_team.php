<?php

require_once 'backstage/parse_xml.php';
require_once 'backstage/helper.php';

$lang = checkLang();
$team = $_POST['RACE'];					// first we need the race of the team
$race = returnRace($team, $lang);			// then the info of that race
$translation = buildTranslation($team,$lang,'en');	// and the translation
$interface = parse('data/'.$lang.'/roster.xml');

$xml = simplexml_load_file('data/team.xml');

$xml->addAttribute('race', $team);
$xml->addAttribute('emblem', $_POST['TEAMLOGO']);

$xml->name = $_POST['TEAM'];
$xml->coach = $_POST['HEADCOACH'];
$xml->bank = $_POST['TREASURY'];
$xml->reroll = $_POST['REROLLS'];
$xml->rerollcost = $_POST['REROLLCOST'];
$xml->fanfactor = $_POST['FANFACTOR'];
$xml->assistant = $_POST['COACHES'];
$xml->cheerleader = $_POST['CHEERLEADERS'];
$xml->apothecary = $_POST['APOTHECARY'];
$xml->background = $_POST['BACKGROUND'];

/** Add players **/

for ( $i=0; $i<16; $i++ )
{
	if ( $_POST['POSITION'][$i] != 0 )
	{
		$pos = $_POST['POSITION'][$i] - 1;
		if ( $_POST['DISPLAY'][$i] == '' )
		{
			$pic = $race->positions->position[$pos]->display;
		}
		else
		{
			$pic = $_POST['DISPLAY'][$i];
		}
		
		$player = $xml->players->addChild('player','');
		
		if ( $_POST['NAME'][$i] == "" )
		{
			$name = " "; // dirty hack... simplexml wont add empty attributes :(
		}
		else
		{
			$name = $_POST['NAME'][$i];
		}
		$player->addAttribute('name', $name);
		
		$player->addAttribute('number', $i);
		$player->addAttribute('position', $race->positions->position[$pos]->title );
		$player->addAttribute('display', $pic );
		
		$player->addChild('ma', $_POST['MA'][$i]);
		$player->addChild('st',$_POST['ST'][$i]);
		$player->addChild('ag',$_POST['AG'][$i]);
		$player->addChild('av',$_POST['AV'][$i]);
		$player->addChild('cost', $_POST['VALUE'][$i]);
		
		if ( $_POST['SKILLS'][$i] != '' )
		{
			$skills = explode(",", $_POST['SKILLS'][$i]);
			
			foreach ( $skills as $skill )
			{
				if ( eregi('\+', $skill) )
				{
					$skill = eregi_replace('\+','',$skill);
					$player->addChild('skill', '+'.$translation[$skill]);
				}
				else 
				{
					$player->addChild('skill', $translation[$skill]);
				}
			}
		}
		
		$player->addChild('com', $_POST['COMP'][$i]);
		$player->addChild('td', $_POST['TD'][$i]);
		$player->addChild('int', $_POST['INT'][$i]);
		$player->addChild('cas', $_POST['CAS'][$i]);
		$player->addChild('mvp', $_POST['MVP'][$i]);
		$player->addChild('spp', $_POST['SPP'][$i]);
		
		if ( $_POST['INJURIES'][$i] != "" )
		{
			$missing = false; // reset values
			$nigglings = 0;
			$ma_reduction = 0;
			$st_reduction = 0;
			$ag_reduction = 0;
			$av_reduction = 0;
			
			$injuries = explode(",", $_POST['INJURIES'][$i]);
			
			foreach ($injuries as $field) {
				switch ($field) {
					case "M":
						$missing = "true";
						break;
					case "N":
						$nigglings++;
						break;
					case "-".$interface->MA:
						$ma_reduction++;
						break;
					case "-".$interface->ST:
						$st_reduction++;
						break;
					case "-".$interface->AG:
						$ag_reduction++;
						break;
					case "-".$interface->AV:
						$av_reduction++;
						break;
				}
			}
			
			$player->addChild('inj','');
			if ( $missing == true ) { $player->inj['missNextMatch'] = true; }
			if ( $nigglings > 0 )  { $player->inj['nigglingInjuries'] = $nigglings; }
			if ( $ma_reduction > 0 ) { $player->inj['MaReduction'] = $ma_reduction; }
			if ( $st_reduction > 0 ) { $player->inj['StReduction'] = $st_reduction; }
			if ( $ag_reduction > 0 ) { $player->inj['AgReduction'] = $ag_reduction; }
			if ( $av_reduction > 0 ) { $player->inj['AvReduction'] = $av_reduction; }
		}	
	}
}

$xmlString =  $xml->asXML();

$xmlString = preg_replace('/\t/', '', $xmlString);
$xmlString = preg_replace('/<\/(\w+)></', "</$1>\n<", $xmlString);
$xmlString = preg_replace('/<ma>/', "\n<ma>", $xmlString);
$xmlString = preg_replace('/<\/player>/', "</player>\n", $xmlString);

/*** OUTPUT ***/
header('Content-type: application/xml');
header("Content-Disposition: attachment; filename=\"{$xml->name}.xml\"");
echo $xmlString;

?>
