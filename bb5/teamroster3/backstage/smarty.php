<?php

require_once '/usr/local/lib/php/Smarty/Smarty.class.php';

define('PROJECT_DIR', '/home/smashdot/public_html/snore/');

class mySmarty extends Smarty { 
    function mySmarty() {
        $this->template_dir 	= PROJECT_DIR.'smarty/templates';
        $this->compile_dir 	= PROJECT_DIR.'smarty/templates_c';
        $this->config_dir 	= PROJECT_DIR.'smarty/configs';
        $this->cache_dir 	= PROJECT_DIR.'smarty/cache';
    }
}

?>
