<?php header("Content-type: text/html; charset=utf-8"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<title>TBT Snore</title>
<link rel="stylesheet" type="text/css" media="screen" href="includes/roster.css" title="Default" />
<script type="text/javascript" src="includes/functions.js"></script>
<script type="text/javascript" src="includes/subroutines.js"></script>
<script type="text/javascript" src="includes/roster.js"></script>
<script type="text/javascript">

<?php
error_reporting(E_ALL);

require_once('includes/minixml.php');
require_once('includes/parse_xml.php');
require_once('includes/echo_html.php');
require_once('includes/echo_js.php');

if (isset($_POST['upload']) && $_POST['upload'] == true) {
	$team = parseTeamRoster($_FILES['userfile']['tmp_name']);
	$chosen_race = $team['race'];
}
else {
	$team = 0;
	$chosen_race = htmlentities($_GET['race']);
}

$race = parseRaces($chosen_race, 'races.xml');
if (isset($_POST['upload']) && $_POST['upload'] == true) {
		echo writeTeamRoster($team,$race);
}
$skill_list = parseSkills('skills.xml');

echo writePosStats($race)."\n";
echo writePosSkills($race)."\n";
echo writeTeamInfo($race)."\n";
echo writeSkillArrays($skill_list);

?>

</script>

</head>

<?php

if (isset($_POST['upload']) && $_POST['upload'] == true) {
	$startup_functions = "setlogo();setSelects();findInjuredPlayers();calcExtraValue(16);calcExtraValue(17);";
	$startup_functions .= "calcExtraValue(18);calcExtraValue(19);calcExtraValue(20);calcTeamValue()";
}
else {
	$startup_functions = "setlogo()";
}
?>

<body onload="<?php echo $startup_functions; ?>">
<h1>Tow Bowl Tactics Teamroster</h1>

<form id="ROSTER" action="save.php" method="post" enctype="multipart/form-data">
<table>

	<tr class="bg1 center thicker-bottom">
		<td>#</td><td>PLAYER'S NAME</td><td>POSITION</td>
		<td>MA</td><td>ST</td><td>AG</td><td>AV</td>
		<td>SKILLS</td><td>INJ</td>
		<td class="bg3">COM</td><td class="bg3">TD</td><td class="bg3">INT</td>
		<td class="bg3">CAS</td><td class="bg3">MVP</td><td class="bg3">SPP</td><td>VALUE</td>
	</tr>
		
<?php

$dropdown = writeDropDown($race);

if (isset($_POST['upload']) && $_POST['upload'] == true) {

	for ( $i = 0; $i < 16; $i++ ) {
		echoLoadedTableRow($dropdown, $i, $team);
	}
}
else {
	for ( $i = 0; $i < 16; $i++ ) {
		echoTableRow($dropdown, $i);
	}
}

?>

<tr class="thicker-top">
      <td rowspan="6" colspan="2">
      	<p><a href="javascript:show('pic_box')">Customize Teampics</a><br />
      	<a href="javascript:show('background_box')">Write Background</a><br /><br />
      	<input name="VERBOSE" type="checkbox" checked="checked" />Write statchanges to roster<br />
      	<a href="javascript:save()">Save Roster</a><br />
      	<a href="index.php">Return to Start.</a>
      </td>
      
      <td rowspan="6" colspan="2" style="text-align: center">
      	<p><img alt="a colorful picture" id="BADGE" style="width:104px;height:140px;" src="" /></p>
      </td>
      
      <td colspan="3" rowspan="2">TEAM</td>
      
      <td rowspan="2">
      	<input name="TEAM" type="text" size="20" value="<?php ifLoadedEcho('name',$team); ?>" />
      </td>
      
      <td colspan="3" class="bg1">RE-ROLLS</td>
      
      <td colspan="1">
      	<input class="center" name="REROLLS" onchange="calcExtraValue(16)" type="text" size="2" maxlength="1" value="<?php ifLoadedEcho('reroll',$team); ?>" />
      </td>

      <td colspan="3" class="bg1">x 
      	<input class="bg1" name="REROLLCOST" type="text" size="5" value="<?php echo number_format($race['reroll_cost'], 0, ",", "."); ?>" readonly="readonly" /> gp
      </td>

      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>
      <td colspan="3" class="bg1">FAN FACTOR</td>
      <td colspan="1">
      	<input class="center" name="FANFACTOR" onchange="calcExtraValue(17)" type="text" size="2" maxlength="1" value="<?php ifLoadedEcho('fanfactor',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>
	  <td colspan="3">players (<a class="small blue" href="javascript:showJmBox()">manage</a>)</td>
	  <td colspan="1"><input name="HEALTHY" type="text" size="2" readonly="readonly" /></td>
      <td colspan="3" class="bg1">ASSISTANTS</td>
      <td colspan="1">
      	<input class="center" name="COACHES" onchange="calcExtraValue(18)" type="text" size="2" maxlength="2" value="<?php ifLoadedEcho('assistant',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


      <td colspan="3">RACE</td>
      <td>
      	<input name="RACE" type="text" size="8" value="<?php echo $chosen_race ?>" readonly="readonly" />
      </td>
      <td colspan="3" class="bg1">CHEERLEADERS</td>
      <td colspan="1">
      	<input class="center" name="CHEERLEADERS" onchange="calcExtraValue(19)" type="text" size="2" maxlength="2" value="<?php ifLoadedEcho('cheerleader',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


 <td colspan="3">TREASURY</td>
      <td><input name="TREASURY" type="text" size="6" value="" /></td>
      <td colspan="3" class="bg1">APOTHECARY</td>
      <td colspan="1">
      	<input class="center" name="APOTHECARY" onchange="calcExtraValue(20)" type="text" size="2" maxlength="1" value="<?php ifLoadedEcho('apothecary',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 50.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


      <td colspan="3">HEADCOACH</td>
      <td>
      	<input name="HEADCOACH" type="text" size="8" value="" />
      </td>
      <td colspan="7" class="bg1">TOTAL VALUE OF TEAM</td>
      <td>
      	<input name="TEAMVALUE" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
</table>


<div id="pic_box" class="element_hidden">
	<p>Why don't you just enter the filenames of the pictures you want to use.<br />
	Player without custom pics will use the default pictures.</p>
	<table>
	<?php
		echo "<tr><td>Team</td>";
		
		if (isset($_POST['upload']) && $_POST['upload'] == true) {
			echo "<td><input name=\"TEAMLOGO\" type=\"text\" value=\"{$team['emblem']}\" /></td></tr>\n";
		}
		else {
			echo "<td><input name=\"TEAMLOGO\" type=\"text\" value=\"{$race['logo']}\" /></td></tr>\n";
		}

		for($i=0; $i < 16; $i++) {
			$num = $i + 1;
			echo "<tr><td>$num</td><td><input name=\"DISPLAY[]\" type=\"text\" value=\"";
			if ( is_array($team) && array_key_exists($i, $team['player']) ) {
				echo $team['player'][$i]['image'];
			}
			echo "\" /></td></tr>\n";
		}
	?>
	</table>
	<p><a href="javascript:hideLayer('pic_box')">Close</a></p>
</div>

<div id="jm_box" class="element_hidden">
<p>You may hire one or more of the journeymen you used after a match. <br />
Simply click on the corresponding number (non-journeymen are not shown).</p>
<table>
<tr>
<td class="jm"><a id="jm0" class="green" href="javascript:legalize(0)">1</a></td>
<td class="jm"><a id="jm1" class="green" href="javascript:legalize(1)">2</a></td>
<td class="jm"><a id="jm2" class="green" href="javascript:legalize(2)">3</a></td>
<td class="jm"><a id="jm3" class="green" href="javascript:legalize(3)">4</a></td>
<td class="jm"><a id="jm4" class="green" href="javascript:legalize(4)">5</a></td>
<td class="jm"><a id="jm5" class="green" href="javascript:legalize(5)">6</a></td>
<td class="jm"><a id="jm6" class="green" href="javascript:legalize(6)">7</a></td>
<td class="jm"><a id="jm7" class="green" href="javascript:legalize(7)">8</a></td>
</tr>
<tr>
<td class="jm"><a id="jm8" class="green" href="javascript:legalize(8)">9</a></td>
<td class="jm"><a id="jm9" class="green" href="javascript:legalize(9)">10</a></td>
<td class="jm"><a id="jm10" class="green" href="javascript:legalize(10)">11</a></td>
<td class="jm"><a id="jm11" class="green" href="javascript:legalize(11)">12</a></td>
<td class="jm"><a id="jm12" class="green" href="javascript:legalize(12)">13</a></td>
<td class="jm"><a id="jm13" class="green"  href="javascript:legalize(13)">14</a></td>
<td class="jm"><a id="jm14" class="green" href="javascript:legalize(14)">15</a></td>
<td class="jm"><a id="jm15" class="green" href="javascript:legalize(15)">16</a></td>
</tr>
</table>
<p><a href="javascript:hideLayer('jm_box')">Close</a></p>
</div>

<div id="skill_box" class="element_hidden">
	<p><a href="javascript:hideLayer('skill_box')">Close</a></p>
	<table>
	<tr>
		<td style="width: 150px;">normal repertory</td>
		<td style="width: 150px;">Player: #<input class="bg1" name="TEMP1" type="text" readonly="readonly" size="2" /></td>
		<td style="width: 150px;">double repertory</td>
		<td style="width: 150px;">forbidden skills</td>
	</tr>
	<tr class="thicker-bottom">
		<td style="width: 150px;"><select size="10" class="repertory bg2" name="SKILLSRCN"><option></option></select></td>
		<td style="width: 150px;"><select size="10" class="repertory" name="SKILLDEST"><option></option></select></td>
		<td style="width: 150px;"><select size="10" class="repertory bg2" name="SKILLSRCD"><option></option></select></td>
		<td style="width: 150px;"><select size="10" class="repertory bg1" name="SKILLSRCF"><option></option></select></td>
	</tr>
	<tr class="thicker-top">
		<td class="center"><a href="javascript:addskill('n')" class="large green">Add</a></td>
		<td class="center"><a href="javascript:remskill()" class="large red">Remove</a></td>
		<td class="center"><a href="javascript:addskill('d')" class="large green">Add</a></td>
		<td class="center"><a href="javascript:addskill('i')" class="large red">Add</a></td>
	</tr>
	</table>
	<p>You added a value of <input class="bg1" name="TEMP3" type="text" readonly="readonly" size="6" value="0" /> gp to your player.</p>
	<p><a href="javascript:setSkillchanges()">Validate</a> / <a href="javascript:hideLayer('skill_box')">Cancel</a></p>
</div>

<div id="background_box" class="element_hidden">
<p>Write a background for your team!</p>
<p><textarea name="BACKGROUND" cols="60" rows="7"><?php 

if (isset($_POST['upload']) && $_POST['upload'] == true) {
	print(nl2br($team['background']));
}
else {
	print(nl2br($race['background']));
}
?></textarea></p>
<p><a href="javascript:hideLayer('background_box')">Close</a></p>
</div>

<div id="inj_box" class="element_hidden">
	<p style="font-weight: bold;">Player: #<input class="bg1" name="TEMP2" type="text" readonly="readonly" size="2" /></p>
	<p><label><input type="checkbox" name="M" value="M" />Miss Next Game</label></p>
	<p>Niggling Injuries:
	<a class="green" href="javascript:setNiggling('minus')">&lt;</a>
		<input class="bg1 center" name="N" type="text" readonly="readonly" size="1" value="0" />
		<a class="red" href="javascript:setNiggling('plus')">&gt;</a>
	</p>
	<p>Your player has:<br />
		<input class="bg1" type="text" name="TEMPMA" readonly="readonly" size="1" value="0" /> MA reductions.<br />
		<input class="bg1" type="text" name="TEMPST" readonly="readonly" size="1" value="0" /> ST reductions.<br />
		<input class="bg1" type="text" name="TEMPAG" readonly="readonly" size="1" value="0" /> AV reductions.
	</p>
	<p><a href="javascript:setInjury()">Validate</a> / <a href="javascript:hideLayer('inj_box')">Cancel</a></p>
</div>

<p>
<?php
echo writeTitleList($race)."\n";
echo writeDisplayList($race);
?>
</p>
</form>

</body>
</html>