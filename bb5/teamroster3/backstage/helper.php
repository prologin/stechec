<?php

function sprint($string) {	// sprint like in "Special characters PRINT"
	echo htmlentities($string, ENT_QUOTES, 'UTF-8');
}

function checkLang() {
	
	$lang = &$_COOKIE['lang'];
	
	if ( isset($lang) && is_dir('data/'.$lang) ) {
		$lang = $_COOKIE['lang'];
	}
	else {
		$lang = 'en';
	}
	
	return $lang;
}

?>