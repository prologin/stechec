<?php

function ifLoadedEcho($var,$team) {
	
	// used for conditional writing of values if a teamroster was loaded and the value is set
	
	if ( isset($_POST['upload']) && $_POST['upload'] == true ) {
		if ( is_array($team) ) {
			if ( isset($team[$var]) ) {
				echo $team[$var];
			}
		}
	}

}

function writeDropDown($race) {

	$string = "";

	for ($i=0; $i < $race['player_num']; $i++) {

		$string .= "<option value=\"$i\">";
		$string .= "{$race['player_'.$i]['title']} (max. {$race['player_'.$i]['qty']})";
		$string .= "</option>";
		
		if( $i < ($race['player_num']-1) ) { 
			$string .= "\n"; 
		}	
	
	}
	
	// the empty player per default
	$string .= "<option value=\"{$race['player_num']}\" selected=\"selected\"></option> \n";
	
	return $string;
	
}

function writeTitleList($race) {

	$string = "<input type=\"hidden\" name=\"PosNames\" value=\"";
	
	for ($i=0; $i < $race['player_num']; $i++) { 
		$string .= $race['player_'.$i]['title']." ";
	}
	
	$string .= "\" />";
	
	return $string;
}

function writeDisplayList($race) {

	$string = "<input type=\"hidden\" name=\"PosPics\" value=\"";
	
	for ($i=0; $i < $race['player_num']; $i++) {
		$string .= $race['player_'.$i]['display']." ";
	}

	$string .= "\" />";
	
	return $string;
	
}
function echoLoadedTableRow($dropdown, $i, $loaded_team) {

		$num = $i + 1;
		
		if ( array_key_exists($i, $loaded_team['player']) ) {			
		
echo <<<END

<!-- ++++++ The row for player number $num. +++++++++++++ -->
<tr>
<td class="bg1 center">$num</td>
<td><input name="NAME[]" onchange="checkName($i)" type="text" maxlength="25" value="{$loaded_team['player'][$i]['name']}" /></td>
<td><select name="POSITION[]" onchange="fillPlayerStats($i)">
$dropdown
</select></td>
<td>
	<a class="small red" href="javascript:changeStat('MA','m', $i)">&lt;</a>
	<input class="center" name="MA[]" type="text" size="1" readonly="readonly" value="{$loaded_team['player'][$i]['ma']}" />
	<a class="small green" href="javascript:changeStat('MA','p', $i)">&gt;</a>
</td>
<td>
	<a class="small red" href="javascript:changeStat('ST','m', $i)">&lt;</a>
	<input class="center" name="ST[]" type="text" size="1" readonly="readonly" value="{$loaded_team['player'][$i]['st']}" />
	<a class="small green" href="javascript:changeStat('ST','p', $i)">&gt;</a>
</td> 
<td>
	<a class="small red" href="javascript:changeStat('AG','m', $i)">&lt;</a>
	<input class="center" name="AG[]" type="text" size="1" readonly="readonly" value="{$loaded_team['player'][$i]['ag']}" />
	<a class="small green" href="javascript:changeStat('AG','p', $i)">&gt;</a>
</td> 
<td>
	<a class="small red" href="javascript:changeStat('AV','m', $i)">&lt;</a>
	<input class="center" name="AV[]" type="text" size="2" readonly="readonly" value="{$loaded_team['player'][$i]['av']}" />
	<a class="small green" href="javascript:changeStat('AV','p', $i)">&gt;</a>
</td> 
<td>
	<input name="SKILLS[]" type="text" readonly="readonly" value="{$loaded_team['player'][$i]['skills']}" />
	<a class="small blue" href="javascript:showSkillBox($i)">+</a>
</td> 
<td>
	<input name="INJ[]" type="text" size="4" readonly="readonly" value="{$loaded_team['player'][$i]['inj']}" />
	<a class="small blue" href="javascript:showInjBox($i)">+</a>
</td> 
<td><input name="COMP[]"    onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="{$loaded_team['player'][$i]['com']}" /></td> 
<td><input name="TD[]" 		onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="{$loaded_team['player'][$i]['td']}" /></td> 
<td><input name="INT[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="{$loaded_team['player'][$i]['int']}" /></td> 
<td><input name="CAS[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="{$loaded_team['player'][$i]['cas']}" /></td> 
<td><input name="MVP[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="{$loaded_team['player'][$i]['mvp']}" /></td> 
<td><input name="SPP[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" readonly="readonly" value="{$loaded_team['player'][$i]['spp']}" /></td>
<td><input name="VALUE[]"   class="healthy" onchange="calcPlayerSPP($i)" type="text" size="6" readonly="readonly" value="{$loaded_team['player'][$i]['cost']}" /></td>
</tr>

END;
}
else {
	echoTableRow($dropdown, $i);
}
}		

function echoTableRow($dropdown,$i) {
	
		$num = $i + 1; // count from one
		
echo <<<END

<!-- ++++++ The row for player number $num. +++++++++++++ -->
<tr>
<td class="bg1 center">$num</td>
<td><input name="NAME[]" onchange="checkName($i)" type="text" maxlength="25" value="" /></td>
<td><select name="POSITION[]" onchange="fillPlayerStats($i)">
$dropdown
</select></td>
<td>
	<a class="small red" href="javascript:changeStat('MA','m', $i)">&lt;</a>
	<input class="center" name="MA[]" type="text" size="1" readonly="readonly" />
	<a class="small green" href="javascript:changeStat('MA','p', $i)">&gt;</a>
</td>
<td>
	<a class="small red" href="javascript:changeStat('ST','m', $i)">&lt;</a>
	<input class="center" name="ST[]" type="text" size="1" readonly="readonly" />
	<a class="small green" href="javascript:changeStat('ST','p', $i)">&gt;</a>
</td> 
<td>
	<a class="small red" href="javascript:changeStat('AG','m', $i)">&lt;</a>
	<input class="center" name="AG[]" type="text" size="1" readonly="readonly" />
	<a class="small green" href="javascript:changeStat('AG','p', $i)">&gt;</a>
</td> 
<td>
	<a class="small red" href="javascript:changeStat('AV','m', $i)">&lt;</a>
	<input class="center" name="AV[]" type="text" size="2" readonly="readonly" />
	<a class="small green" href="javascript:changeStat('AV','p', $i)">&gt;</a>
</td> 
<td>
	<input name="SKILLS[]" type="text" readonly="readonly" />
	<a class="small blue" href="javascript:showSkillBox($i)">+</a>
</td> 
<td>
	<input name="INJ[]" type="text" size="4" readonly="readonly" />
	<a class="small blue" href="javascript:showInjBox($i)">+</a>
</td> 
<td><input name="COMP[]"    onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="TD[]" 		onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="INT[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="CAS[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="MVP[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="SPP[]" 	onchange="calcPlayerSPP($i)" type="text" size="2" readonly="readonly" value="" /></td>
<td><input name="VALUE[]"   class="healthy" onchange="calcPlayerSPP($i)" type="text" size="6" readonly="readonly" value="" /></td>
</tr>

END;
}

?>