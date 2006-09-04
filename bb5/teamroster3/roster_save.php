<?php

require_once('minixml.inc.php');

header('Content-type: application/xml');
$teamname = htmlentities($_POST['TEAM']);
header("Content-Disposition: attachment; filename=\"$teamname.xml\"");

// I want to prepare a beautiful array

$posNames = explode(" ", $_POST['PosNames']);
$posPics = explode(" ", $_POST['PosPics']);

$xmlDoc = new MiniXMLDoc();

$arr = array();
$arr['team'] = array();

$arr['team']['race'] = htmlentities($_POST['RACE']);
$arr['team']['BBversion'] = 5;
$arr['team']['emblem'] = htmlentities($_POST['TEAMLOGO']);
$arr['team']['xmlns:xsi'] = "http://www.w3.org/2001/XMLSchema-instance";
$arr['team']['xsi:noNamespaceSchemaLocation'] = "team.xsd";
$arr['team']['name'] = $teamname;
$arr['team']['coach'] = htmlentities($_POST['HEADCOACH']);
$arr['team']['bank'] = htmlentities($_POST['TREASURY']);
$arr['team']['reroll'] = htmlentities($_POST['REROLLS']);
$arr['team']['rerollcost'] = htmlentities($_POST['REROLLCOST']);
$arr['team']['fanfactor'] = htmlentities($_POST['FANFACTOR']);
$arr['team']['assistant'] = htmlentities($_POST['COACHES']);
$arr['team']['cheerleader'] = htmlentities($_POST['CHEERLEADERS']);
$arr['team']['apothecary'] = htmlentities($_POST['APOTHECARY']);
$arr['team']['players'] = array();
$arr['team']['player'] = array();

for( $i = 0; $i < 16; $i++) {

	if ( $posNames[$_POST['POSITION'][$i]] != "" ) { 	

		$player = array();
		$player['name'] = $_POST['NAME'][$i];
		$player['number'] = $i + 1;
		$player['position'] = $posNames[$_POST['POSITION'][$i]];
		
		if ( $_POST['DISPLAY'][$i] == "" ) {
			$player['display'] = $posPics[$_POST['POSITION'][$i]];
		}
		else {
			$player['display'] = $_POST['DISPLAY'][$i];
		}
		
		$player['ma'] = $_POST['MA'][$i];
		$player['st'] = $_POST['ST'][$i];
		$player['ag'] = $_POST['AG'][$i];
		$player['av'] = $_POST['AV'][$i];
		$player['cost'] = $_POST['VALUE'][$i];
		
		if ( $_POST['SKILLS'][$i] == "" ) {
			$player['skills'] = $_POST['SKILLS'][$i];
		}
		else {
			$skills = explode(",", $_POST['SKILLS'][$i]);
			$player['skills']['skill'] = $skills;
		}
		
		if ( $_POST['INJ'][$i] == "" ) {
			$player['inj'] = $_POST['INJ'][$i];
		}
		else {
			
			$missing = false; // reset values
			$nigglings = 0;
			$ma_reduction = 0;
			$st_reduction = 0;
			$ag_reduction = 0;
			$av_reduction = 0;
			
			$injuries = explode(",", $_POST['INJ'][$i]);
			
			foreach ($injuries as $field) {
				switch ($field) {
					case "M":
						$missing = "true";
						break;
					case "N":
						$nigglings++;
						break;
					case "-MA":
						$ma_reduction++;
						break;
					case "-ST":
						$st_reduction++;
						break;
					case "-AG":
						$ag_reduction++;
						break;
					case "-AV":
						$av_reduction++;
						break;
				}
			}
			
			
			if ( $missing == true ) { $player['inj']['missNextMatch'] = true; }
			if ( $nigglings > 0 ) { $player['inj']['nigglingInjuries'] = $nigglings; }
			if ( $ma_reduction > 0 ) { $player['inj']['MaReduction'] = $ma_reduction; }
			if ( $st_reduction > 0 ) { $player['inj']['StReduction'] = $st_reduction; }
			if ( $ag_reduction > 0 ) { $player['inj']['AgReduction'] = $ag_reduction; }
			if ( $av_reduction > 0 ) { $player['inj']['AvReduction'] = $av_reduction; }
		
		}
			
		$player['com'] = $_POST['COMP'][$i];
		$player['td'] = $_POST['TD'][$i];
		$player['int'] = $_POST['INT'][$i];
		$player['cas'] = $_POST['CAS'][$i];
		$player['mvp'] = $_POST['MVP'][$i];
		$player['spp'] = $_POST['SPP'][$i];
		
		$arr['team']['players']['player'][] = $player;
	}
}

$arrayOptions = array(
	'attributes'	=> array(
			'player'	=> array('name','number','position','display'),
			'team'		=> array('race','BBversion','emblem','xmlns:xsi','xsi:noNamespaceSchemaLocation'),
			'inj'		=> array('missNextMatch','nigglingInjuries','MaReduction','StReduction','AgReduction','Avreduction')
			),
);

$xmlDoc->fromArray($arr, $arrayOptions);
print $xmlDoc->toString();
?>
