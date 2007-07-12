<?php

require_once 'backstage/smarty.php';
require_once('backstage/parse_xml.php');
require_once('backstage/helper.php');

$lang = checkLang();
$output = new mySmarty();

$interface = parse('data/'.$lang.'/roster.xml');
$skills = parse('data/'.$lang.'/skills.xml');

if (isset($_POST['upload']) && $_POST['upload'] == true)
{
	$team = parse($_FILES['userfile']['tmp_name']);
	$output->assign('team', $team);
	
	$chosen_race = (string) $team['race'];
	print_r($team);
	die();
}
else
{
	$chosen_race = htmlentities($_GET['race']);
}

$translation = buildTranslation($chosen_race, 'en', $lang);
$race = returnRace($translation[$chosen_race], $lang);

/**** OUTPUT OF HTML HEAD (including Javascript) ****/
$output->assign('skills', 	$skills->skills);
$output->assign('text', 	$interface);
$output->assign('race', 	$race);
$output->display('roster.tpl');

?>