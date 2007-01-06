<?php

function writeWarnings($interface) {

	echo "warning = new Array()\n";
	
	for ( $i=0; $i < 16; $i++ ) {
		echo "warning[$i] = \"{$interface['javascript']['warning'][$i]}\"\n";
	}

}

function writeStatNames($interface) {

	echo "var MA = \"{$interface['roster']['MA']}\"\n";
	echo "var ST = \"{$interface['roster']['ST']}\"\n";
	echo "var AG = \"{$interface['roster']['AG']}\"\n";
	echo "var AV = \"{$interface['roster']['AV']}\"\n";

}

function writePosStats($race) {

	// writes one javascript-array per position
	// containg all the information of that particular player-type
	// except skills
	
	$string = "stats = new Array\n";
	
	for ($i=0; $i < $race['player_num']; $i++) {
		$string .= "stats[$i] = new Array(";
		$string .= "\"{$race['player_'.$i]['title']}\",";
		$string .= "{$race['player_'.$i]['ma']},{$race['player_'.$i]['st']},";
		$string .= "{$race['player_'.$i]['ag']},{$race['player_'.$i]['av']},";
		$string .= "{$race['player_'.$i]['cost']},{$race['player_'.$i]['qty']},0,";
		$string .= "\"{$race['player_'.$i]['normal']}\",\"{$race['player_'.$i]['double']}\",";
		$string .= "\"{$race['player_'.$i]['display']}\")\n";
	}
	
	// the javascript array used to reset a player
	$string .= "stats[{$race['player_num']}] = new Array(\"\",\"\",\"\",\"\",\"\",\"\",99,16)\n";
	
	return $string;	
}

function writePosSkills($race) {

	// writes an array of skills belonging to the position with the same number
	// means: skills[X] are the skills of the player stats[X][0] 
	
	$string = "skills = new Array\n";

	for ($i=0; $i < $race['player_num']; $i++) {
		$string .= "skills[$i] = new Array(";
		
		for ($j=0; $j < $race['player_'.$i]['skill_num']; $j++) {
			$string .= "\"{$race['player_'.$i]['skill_'.$j]}\"";
			
			if($j < ($race['player_'.$i]['skill_num']-1)) { 
				$string .= ","; 
			}
		}
		
	$string .=")\n";
	}
	
	// the javascript array used to reset a player
	$string .= "skills[{$race['player_num']}] = new Array()\n";
	
	return $string;
		
}

function writeTeamInfo($race) {
	
	$string  = "var positions = {$race['player_num']}\n";
	$string .= "var apothecary = new Boolean({$race['apothecary']})\n";
	$string .= "var reroll_cost = {$race['reroll_cost']}\n";
	$string .= "var logo = \"{$race['logo']}\"\n";
	
	return $string;

}

function writeSkillArrays($skill_list) {

	// this will simply output all skills that exist in the game
	// split into the correct categories
	
	$skills_general = "skills_g = new Array(";
	for($i=0; $i<$skill_list['General']['_num']; $i++) {
		$skills_general .= "\"{$skill_list['General'][$i]}\"";
		if($i < ($skill_list['General']['_num']-1)) {
			$skills_general .= ",";
		}
	}
	$skills_general .= ")\n";
	
	$skills_passing = "skills_p = new Array(";
	for($i=0; $i<$skill_list['Passing']['_num']; $i++) {
		$skills_passing .= "\"{$skill_list['Passing'][$i]}\"";
		if($i < ($skill_list['Passing']['_num']-1)) {
			$skills_passing .= ",";
		}
	}
	$skills_passing .= ")\n";
	
	$skills_agility = "skills_a = new Array(";
	for($i=0; $i<$skill_list['Agility']['_num']; $i++) {
		$skills_agility .= "\"{$skill_list['Agility'][$i]}\"";
		if($i < ($skill_list['Agility']['_num']-1)) {
			$skills_agility .= ",";
		}
	}
	$skills_agility .= ")\n";
	
	$skills_strength = "skills_s = new Array(";
	for($i=0; $i<$skill_list['Strength']['_num']; $i++) {
		$skills_strength .= "\"{$skill_list['Strength'][$i]}\"";
		if($i < ($skill_list['Strength']['_num']-1)) {
			$skills_strength .= ",";
		}
	}
	$skills_strength .= ")\n";
	
	$skills_mutation = "skills_m = new Array(";
	for($i=0; $i<$skill_list['Mutation']['_num']; $i++) {
		$skills_mutation .= "\"{$skill_list['Mutation'][$i]}\"";
		if($i < ($skill_list['Mutation']['_num']-1)) {
			$skills_mutation .= ",";
		}
	}
	$skills_mutation .= ")\n";
	
	$skills_extraordinary = "skills_e = new Array(";
	for($i=0; $i<$skill_list['Extraordinary']['_num']; $i++) {
		$skills_extraordinary .= "\"{$skill_list['Extraordinary'][$i]}\"";
		if($i < ($skill_list['Extraordinary']['_num']-1)) {
			$skills_extraordinary .= ",";
		}
	}
	$skills_extraordinary .= ")\n";

	$string = $skills_general.$skills_passing.$skills_agility;
	$string .= $skills_strength.$skills_mutation.$skills_extraordinary;
	
	return $string;
}

function writeTeamRoster($loaded_team,$loaded_race) {

	if ( $loaded_team['bank'] 		== '' ) { $loaded_team['bank'] = 0; }
	if ( $loaded_team['reroll'] 	== '' ) { $loaded_team['reroll'] = 0; }
	if ( $loaded_team['fanfactor'] 	== '' ) { $loaded_team['fanfactor'] = 0; }
	if ( $loaded_team['assistant'] 	== '' ) { $loaded_team['assistant'] = 0; }
	if ( $loaded_team['cheerleader']== '' ) { $loaded_team['cheerleader'] = 0; }
	if ( $loaded_team['apothecary'] == '' ) { $loaded_team['apothecary'] = 0; }
	if ( $loaded_team['background'] == '' ) { $loaded_team['bank'] = 'no background'; }
	if ( $loaded_team['emblem'] 	== '' ) { $loaded_team['bank'] = 'no emblem'; }
	
	// transition from the position name (used in php) 
	// to the position number (used in javascript)
	
	$temp = array();
	
	for ( $i = 0; $i < 16; $i++ ) {
		
		if ( array_key_exists($i, $loaded_team['player']) ) {
	
			for ( $j = 0; $j < $loaded_race['player_num']; $j++ ) {
				if ( $loaded_race['player_'.$j]['title'] == $loaded_team['player'][$i]['position'] ) {
					$temp[$i] = $j;
				}
			}				

		}
		else {
			$temp[$i] = "{$loaded_race['player_num']}";
		}
	
	}
	
	$pos_string = "new Array(".implode(',',$temp).")\n";
	
echo <<<END
var teamname = "{$loaded_team['name']}"
var coach = "{$loaded_team['coach']}"
var treasury = {$loaded_team['bank']}
var rerolls = {$loaded_team['reroll']}
var fanfactor = {$loaded_team['fanfactor']}
var assistants = {$loaded_team['assistant']}
var cheerleader = {$loaded_team['cheerleader']}
var apothecary = {$loaded_team['apothecary']}
var background = "{$loaded_team['background']}"
var emblem = "{$loaded_team['emblem']}"
var arrpositions = $pos_string

END;
}

?>
