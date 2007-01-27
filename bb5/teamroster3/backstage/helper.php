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

class benchmark{
    function start(){
        $time = explode(' ', microtime());
        $this->start = $time[0] + $time[1];
    }

    function stop(){
        $time = explode(' ', microtime());
        $this->stop = $time[0] + $time[1];
    }
    
    function time(){
        return number_format(($this->stop - $this->start), '4');
    }
}
?>