<?php

function sprint($string) {	// sprint like in "Special characters PRINT"
	echo htmlentities($string, ENT_QUOTES, 'UTF-8');
}

function checkLang() {
	if ( isset($_COOKIE['lang']) ) {
		$lang = $_COOKIE['lang'];
		
		if ( $lang != 'en' && $lang != 'de' && $lang != 'fr' ) {
			$lang = 'en';
		}
	}
	else {
		$lang = 'en';
	}
	
	return $lang;
}

?>