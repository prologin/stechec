<?php

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

class benchmark
{
	private $start;

	function start()
	{
		$time = explode(' ', microtime());
		$this->start = $time[0] + $time[1];
	}

	function time()
	{
		$time = explode(' ', microtime());
		$stop = $time[0] + $time[1];	
		return number_format(($stop - $this->start), '4');
	}
}

if (! function_exists('array_combine')) {
  function array_combine($keys, $values) {
   foreach($keys as $key) $out[$key] = array_shift($values);
   return $out;
  }
}

?>
