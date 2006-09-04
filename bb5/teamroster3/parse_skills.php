<?php

/* 
** This script will parse the races.xml and return
** all variables needed for a given team
*/

require_once('minixml.inc.php'); // XML Framework

$xmlDoc = new MiniXMLDoc();
$xmlDoc->fromFile('skills.xml');
$skill_list = $xmlDoc->toArray();

$skill_list = $skill_list['parameters']['skills'];
$skill_list['General'] = $skill_list['General']['skill'];
$skill_list['Passing'] = $skill_list['Passing']['skill'];
$skill_list['Agility'] = $skill_list['Agility']['skill'];
$skill_list['Strength'] = $skill_list['Strength']['skill'];
$skill_list['Mutation'] = $skill_list['Mutation']['skill'];
$skill_list['Extraordinary'] = $skill_list['Extraordinary']['skill'];

// create javascript-output

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

// beware! the former array $skill_list now becomes a string with javascript code in it
$skill_list = $skills_general.$skills_passing.$skills_agility.$skills_strength.$skills_mutation.$skills_extraordinary;

?>
