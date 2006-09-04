<?php

/* 
** This script will parse the races.xml and return
** all variables needed for a given team
*/

require_once('minixml.inc.php'); // XML Framework

$xmlDoc = new MiniXMLDoc();
$xmlDoc->fromFile('races.xml');
$races = $xmlDoc->toArray();
 
// $teamname = "Chaos"; for testing purposes

for($i=0; $i <= $races['races']['race']['_num']; $i++) {

	if ($teamname == $races['races']['race'][$i]['_attributes']['name']) {
		$race_ID = $i; // $teamname is the $_GET variable sent from index.php
		break;
	}
	if ($i == $races['races']['race']['_num']) { // would mean all names were tried without success
		die("<p>No team selected. <a href=\"index.php\">Return</a>.</p>");
	}
} 

$race = $races['races']['race'][$race_ID];

$race['apothecary'] = $race['apothecary']['_attributes']['use'];
$race['reroll_cost'] = $race['reroll']['_attributes']['cost'];
$race['player_num'] = $race['positions']['position']['_num'];
$race['logo'] = $race['_attributes']['emblem'];

for($i=0; $i<$race['player_num']; $i++) {
	$race['player_'.$i] = $race['positions']['position'][$i]['_attributes'];
	$race['player_'.$i]['skills'] = $race['positions']['position'][$i]['skills']['skill'];
	if(is_array($race['player_'.$i]['skills'])) {
		if(array_key_exists('_num', $race['player_'.$i]['skills'])) {
			$race['player_'.$i]['skill_num'] = $race['player_'.$i]['skills']['_num'];
			for($j=0; $j<$race['player_'.$i]['skill_num']; $j++) {
				$race['player_'.$i]['skill_'.$j] = $race['player_'.$i]['skills'][$j]['_attributes']['name'];
			}
		} else {
			$race['player_'.$i]['skill_num'] = 1;
			$race['player_'.$i]['skill_0'] = $race['player_'.$i]['skills']['_attributes']['name'];
		}
	} else {
		$race['player_'.$i]['skill_num'] = 0;
	}
unset ($race['player_'.$i]['skills']);
}
unset($race['positions']);
unset($race['_attributes']);
unset($race['reroll']);

/*
// for testing purposes
echo "<html><head></head><body><pre>\n";
print_r($race);
echo "</pre></body></html>\n";
*/

?>
