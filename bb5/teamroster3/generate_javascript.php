<?php

$dropdown = "";
$pos_stats = "stats = new Array\n";

// the javascript array used to reset a player
$pos_stats .= "stats[{$race['player_num']}] = new Array(\"\",\"\",\"\",\"\",\"\",\"\",99,16)\n";

// the javascript array containing the skills of the players
$pos_skills .= "skills = new Array\n";
$pos_skills .= "skills[{$race['player_num']}] = new Array()\n";

for ($i=0;$i<$race['player_num'];$i++) {

	// prepare the dropdown-list of the positions
	$dropdown = $dropdown."<option value=\"$i\">{$race['player_'.$i]['title']} (max. {$race['player_'.$i]['qty']})</option>";
	if($i<($race['player_num']-1)) { $dropdown .= "\n"; }

	// and the Javascript-Arrays containing the suiting data
	$pos_stats .= "stats[$i] = new Array(";
	$pos_stats .= "\"{$race['player_'.$i]['title']}\",{$race['player_'.$i]['ma']},{$race['player_'.$i]['st']},";
	$pos_stats .= "{$race['player_'.$i]['ag']},{$race['player_'.$i]['av']},{$race['player_'.$i]['cost']},";
	$pos_stats .= "{$race['player_'.$i]['qty']},0,\"{$race['player_'.$i]['normal']}\",\"{$race['player_'.$i]['double']}\",";
	$pos_stats .= "\"{$race['player_'.$i]['display']}\")\n";
	$pos_skills .= "skills[$i] = new Array(";
	for ($j=0;$j<$race['player_'.$i]['skill_num'];$j++) {
		$pos_skills .= "\"{$race['player_'.$i]['skill_'.$j]}\"";
		if($j<($race['player_'.$i]['skill_num']-1)) { $pos_skills .= ","; }
	}
	$pos_skills .=")\n";
}

$dropdown .= "<option value=\"{$race['player_num']}\" selected=\"selected\"></option> \n";
 
echo <<<END
var positions = {$race['player_num']}
var apothecary = new Boolean({$race['apothecary']})
var reroll_cost = {$race['reroll_cost']}
var logo = "{$race['logo']}"

function setlogo() {
	document.getElementById("BADGE").src = "logos/"+logo
}

$pos_stats
$pos_skills
$skill_list

END;
?>
