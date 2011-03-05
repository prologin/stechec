<?php

if ( isset($_POST['lang']) )
{
	setcookie ('lang', $_POST['lang'], time()+60*60*24*30, '/', 'towbowltactics.com', '0');
	header("Location: {$_SERVER['PHP_SELF']}");
}

require_once 'backstage/smarty.php';
require_once 'backstage/parse_xml.php';
require_once 'backstage/helper.php';

$lang = checkLang();

	$races_en = listRaces('en');
	$races_lang = listRaces($lang);
	$races = array_combine($races_en, $races_lang);
	
	$interface = parse('data/'.$lang.'/index.xml');
	
/**** OUTPUT ****/
$smarty = new mySmarty();
$smarty->assign('revision', '1187');
$smarty->assign('races', $races);
$smarty->assign('text', $interface);
$smarty->display('index.tpl');

?>
