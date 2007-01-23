<?php

function parseRaces($xmlRaceFile) {
		
	/* simply transform the races.xml into an array */
		
	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlRaceFile);
	$races = $xmlDoc->toArray();
		
	return $races;	
}

function listRaces($races) {
		
	$races_list = Array();
	
	for( $i = 0; $i < $races['races']['race']['_num']; $i++) {
		$races_list[] = $races['races']['race'][$i]['_attributes']['name'];
	}
	
	return $races_list;
}

function extractRace($races, $chosen_race) {
	
	/*	cut out one race out of the several
		and bring into a convenient form */
		
	$races_list = listRaces($races);
	
	$i = 0;
 
	foreach ( $races_list as $string ) {
		if ( $chosen_race == $string) {
			break;
		}
		$i++;
	}
	
	if ( !array_key_exists($i,$races['races']['race']) ) {
		die('No valid team selected. <a href="roster.php">Return</a>.');
	}
	
	$race = $races['races']['race'][$i];

	$race['apothecary'] = $race['apothecary']['_attributes']['use'];
	$race['reroll_cost'] = $race['reroll']['_attributes']['cost'];
	$race['player_num'] = $race['positions']['position']['_num'];
	$race['logo'] = $race['_attributes']['emblem'];

	for($i=0; $i<$race['player_num']; $i++) {

		$race['player_'.$i] = $race['positions']['position'][$i]['_attributes'];
	
		if(array_key_exists('skills', $race['positions']['position'][$i])) {
			$race['player_'.$i]['skills'] = $race['positions']['position'][$i]['skills']['skill'];
		}
		else {
			$race['player_'.$i]['skills'] = '';
		}
	
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

	return $race;
}

function listPositions($races, $race_name) {
	$race = extractRace($races, $race_name);
	for ( $i = 0; $i < $race['player_num']; $i++ ) {
		$posNames[] = $race['player_'.$i]['title'];
	}
	$posNames[] = "";
	return $posNames;
}

function parseSkills($xmlSkillFile) {

	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlSkillFile);
	$skill_list = $xmlDoc->toArray();

	$skill_list = $skill_list['parameters']['skills'];
	$skill_list['General'] = $skill_list['General']['skill'];
	$skill_list['Passing'] = $skill_list['Passing']['skill'];
	$skill_list['Agility'] = $skill_list['Agility']['skill'];
	$skill_list['Strength'] = $skill_list['Strength']['skill'];
	$skill_list['Mutation'] = $skill_list['Mutation']['skill'];
	$skill_list['Extraordinary'] = $skill_list['Extraordinary']['skill'];
	
	return $skill_list;

}

function parseInterface($xmlSkillFile) {

	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlSkillFile);
	$xmlDocArray= $xmlDoc->toArray();

	return $xmlDocArray['interface'];
}

function parseTeamRoster($xmlTeamRoster, $lang) {

	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlTeamRoster);
	$data = $xmlDoc->toArray();
	$data = $data['team'];
	
	$translation = build_translation('en',$lang);
	
	$race_name_from = $data['_attributes']['race'];
	$race_name_to = $translation[$race_name_from];
	
	$races_from = parseRaces('data/en/races.xml');
	$races_to = parseRaces('data/'.$lang.'/races.xml');
	
	$positions_from = listPositions($races_from, $race_name_from);
	$positions_to = listPositions($races_to, $race_name_to);
	$pos = array_combine($positions_from, $positions_to);
	
	unset($races_from, $races_to);
		
	// some acrobatics with array branches
	$data['race'] = $race_name_to;
	$data['emblem'] = $data['_attributes']['emblem'];
	unset($data['_attributes']);
		
	$data['player'] = $data['players']['player'];
	unset($data['players']);
	
	if (array_key_exists('_num', $data['player'])) {
		unset($data['player']['_num']);
	}
	
	if ( !(array_key_exists(0, $data['player'])) ) {
		$temp = $data['player'];
		unset($data['player']);
		$data['player'][0] = $temp;
	}
	
	for ( $i = 0; $i < count($data['player']); $i++) {
		if (array_key_exists($i, $data['player']) && is_array($data['player'][$i])) {
			
			foreach ($data['player'][$i]['_attributes'] as $key => $value) {
				$data['player'][$i][$key] = $value;
			}
			unset($data['player'][$i]['_attributes']);
			
			// manage injuries - we want a single injury string of the type
			// M,N,-MA,-ST,-AG in ['inj']
			
			if (is_array($data['player'][$i]['inj'])) {
				$data['player'][$i]['inj'] = $data['player'][$i]['inj']['_attributes'];
				$strTemp = '';
				if(array_key_exists('missNextMatch', $data['player'][$i]['inj'])) {
					$strTemp .= 'M';
				}
				if(array_key_exists('nigglingInjuries', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['nigglingInjuries']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }					
						$strTemp .= 'N';
					}
				}
				if(array_key_exists('MaReduction', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['MaReduction']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }
						$strTemp .= '-'.$translation['MA'];
					}
				}
				if(array_key_exists('StReduction', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['StReduction']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }
						$strTemp .= '-'.$translation['ST'];
					}
				}
				if(array_key_exists('AgReduction', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['AgReduction']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }
						$strTemp .= '-'.$translation['AG'];
					}
				}
				
				$data['player'][$i]['inj'] = $strTemp;
			}
			
			// manage skills - we want a single skill string of the 
			// type block,frenzy,xyz in ['skills']
			
			if (is_array($data['player'][$i]['skill'])) {
				unset($data['player'][$i]['skill']['_num']);
				$temp = Array();
				foreach ( $data['player'][$i]['skill'] as $string ) {
					if ( strstr($string, '+') === FALSE ) {
						$temp[] = $translation[$string];
					} else {
						$string = substr($string, 1);
						$temp[] = '+'.$translation[$string];
					}
				}
				$data['player'][$i]['skills'] = implode(",",$temp);
				unset($data['player'][$i]['skill']);
			}
			else {
				if ( $data['player'][$i]['skill'] == "") {
					$data['player'][$i]['skills'] = "";
					unset($data['player'][$i]['skill']);
				}
				else { /* one single skill */
					$skill = $data['player'][$i]['skill'];
					unset($data['player'][$i]['skill']);
					if ( strstr($skill, '+') == FALSE ) {
						$data['player'][$i]['skills'] = $translation[$skill];
					}
					else {
						$skill = substr($skill, 1);
						$data['player'][$i]['skills'] = '+'.$translation[$skill];
					}
					unset($skill);
				}	
			}
		}
	}

	// sort Array by number of the player
	$temp = array();
	
	foreach ( $data['player'] as $element ) {
		$number = $element['number'] - 1;
		unset($element['number']);
		$temp[$number] = $element;
	}
	$data['player'] = $temp;

	return $data;
		
}

function build_translation($from,$to) {

	/* returns an associative array with the words from the first language as keys
		and the words from the second language as values */

	/* Skills */
	$skills = parseSkills('data/'.$from.'/skills.xml');
	$skills_from = array_merge($skills['General'], $skills['Passing'], $skills['Agility'], $skills['Mutation'], $skills['Strength'], $skills['Extraordinary']);
	unset($skills_from['_num']);

	$skills = parseSkills('data/'.$to.'/skills.xml');
	$skills_to = array_merge($skills['General'], $skills['Passing'], $skills['Agility'], $skills['Mutation'], $skills['Strength'], $skills['Extraordinary']);
	unset($skills_to['_num']);

	/* Stats */

	$interface = parseInterface('data/'.$from.'/roster.xml');
	$stats_from = array($interface['MA'], $interface['ST'], $interface['AG'], $interface['AV']);

	$interface = parseInterface('data/'.$to.'/roster.xml');
	$stats_to = array($interface['MA'], $interface['ST'], $interface['AG'], $interface['AV']);
	
	/* Races */
	
	$races_from = listRaces(parseRaces('data/'.$from.'/races.xml'));
	$races_to = listRaces(parseRaces('data/'.$to.'/races.xml'));
	
	/* combine all */
	
	$skills_from = array_merge($skills_from, $stats_from, $races_from);
	$skills_to = array_merge($skills_to, $stats_to, $races_to);
	
	$translation = array_combine($skills_from, $skills_to);
	
	return $translation;

}
?>
