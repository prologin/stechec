<?php

function parseRaces($chosen_race, $xmlRaceFile) {

	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlRaceFile);
	$races = $xmlDoc->toArray();
 
	for( $i = 0; $i <= $races['races']['race']['_num']; $i++) {

		if ($chosen_race == $races['races']['race'][$i]['_attributes']['name']) {
			$race_ID = $i; 
			break;
		}
	
		if ($i == $races['races']['race']['_num']) {
			die("<p>No team selected. <a href=\"index.php\">Return</a>.</p>");
		}
	
	} 

	$race = $races['races']['race'][$race_ID];
	unset($races); // we don't need the other 20 races anymore...

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

function parseTeamRoster($xmlTeamRoster) {

	$xmlDoc = new MiniXMLDoc();
	$xmlDoc->fromFile($xmlTeamRoster);
	$data = $xmlDoc->toArray();
	
	// some acrobatics with array branches
	$data = $data['team'];

	$data['race'] = $data['_attributes']['race'];
	$data['emblem'] = $data['_attributes']['emblem'];
	unset($data['_attributes']);
		
	$data['player'] = $data['players']['player'];
	unset($data['players']);
	
	if (array_key_exists('_num', $data['player'])) {
		unset($data['player']['_num']);
	}
	
	for ( $i = 0; $i < count($data['player']); $i++) {
		if (array_key_exists($i, $data['player']) && is_array($data['player'][$i])) {
			
			$data['player'][$i]['name'] = $data['player'][$i]['_attributes']['name'];
			$data['player'][$i]['position'] = $data['player'][$i]['_attributes']['position'];
			$data['player'][$i]['number'] = $data['player'][$i]['_attributes']['number'];
			$data['player'][$i]['image'] = $data['player'][$i]['_attributes']['display'];
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
						$strTemp .= '-MA';
					}
				}
				if(array_key_exists('StReduction', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['StReduction']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }
						$strTemp .= '-ST';
					}
				}
				if(array_key_exists('AgReduction', $data['player'][$i]['inj'])) {
					for ($j = 0; $j < $data['player'][$i]['inj']['AgReduction']; $j++) {
						if ($strTemp != '') { $strTemp .= ','; }
						$strTemp .= '-AG';
					}
				}
				
				$data['player'][$i]['inj'] = $strTemp;
			}
			
			// manage skills - we want a single skill string of the 
			// type block,frenzy,xyz in ['skills']
			
			if (is_array($data['player'][$i]['skills'])) {
			
				if(!(is_array($data['player'][$i]['skills']['skill']))) {
					$data['player'][$i]['skills'] = $data['player'][$i]['skills']['skill'];
				}
				else { // if ['skill'] IS an array, so, if there is more than 1 skill
					unset($data['player'][$i]['skills']['skill']['_num']);
					$data['player'][$i]['skills'] = implode(",", $data['player'][$i]['skills']['skill']);
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

?>