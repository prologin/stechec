<?php header("Content-type: text/html; charset=utf-8"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<title>TBT Snore</title>
<link rel="stylesheet" type="text/css" media="screen" href="roster.css" title="Default" />
<script type="text/javascript" src="onstage/functions.js"></script>
<script type="text/javascript" src="onstage/subroutines.js"></script>
<script type="text/javascript" src="onstage/roster.js"></script>
<script type="text/javascript">

<?php
error_reporting(E_ALL);

require_once('backstage/minixml.php');
require_once('backstage/parse_xml.php');
require_once('backstage/echo_html.php');
require_once('backstage/echo_js.php');
require_once('backstage/helper.php');

$lang = checkLang();
$interface = parseInterface('data/'.$lang.'_interface.xml');

if (isset($_POST['upload']) && $_POST['upload'] == true) {
	$team = parseTeamRoster($_FILES['userfile']['tmp_name'], $lang);
	$chosen_race = $team['race'];
}
else {
	$team = 0;
	$chosen_race = htmlentities($_GET['race']);
}

$races = parseRaces('data/'.$lang.'_races.xml');
$race = extractRace($races, $chosen_race);

if (isset($_POST['upload']) && $_POST['upload'] == true) {
		echo writeTeamRoster($team,$race);
}
$skill_list = parseSkills('data/'.$lang.'_skills.xml');

echo writePosStats($race)."\n";
echo writePosSkills($race)."\n";
echo writeTeamInfo($race)."\n";
echo writeSkillArrays($skill_list)."\n";
echo writeWarnings($interface)."\n";
echo writeStatNames($interface)."\n";

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

<form id="ROSTER" action="save_team.php" method="post" enctype="multipart/form-data">
<table>

	<tr class="bg1 center thicker-bottom">
		<td>#</td><td><?php sprint($interface['roster']['name']) ?></td>
		<td><?php sprint($interface['roster']['position']) ?></td>
		<td><?php sprint($interface['roster']['MA']) ?></td>
		<td><?php sprint($interface['roster']['ST']) ?></td>
		<td><?php sprint($interface['roster']['AG']) ?></td>
		<td><?php sprint($interface['roster']['AV']) ?></td>
		<td><?php sprint($interface['roster']['skills']) ?></td>
		<td><?php sprint($interface['roster']['inj']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['comp']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['td']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['int']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['cas']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['mvp']) ?></td>
		<td class="bg3"><?php sprint($interface['roster']['spp']) ?></td>
		<td><?php sprint($interface['roster']['value']) ?></td>
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
      <td rowspan="6" colspan="2" style="width: 200px">
      	<p><a href="javascript:show('pic_box')"><?php sprint($interface['roster']['teampics']['label']) ?></a><br />
      	<a href="javascript:show('background_box')"><?php sprint($interface['roster']['background']['label']) ?></a><br /><br />
      	<input name="VERBOSE" type="checkbox" checked="checked" /><?php sprint($interface['roster']['statchanges']) ?><br />
      	<a href="javascript:save()"><?php sprint($interface['roster']['save']) ?></a><br />
      	<a href="index.php"><?php sprint($interface['roster']['return']) ?></a>
      </td>
      
      <td rowspan="6" colspan="2" style="text-align: center">
      	<p><img alt="a colorful picture" id="BADGE" style="width:104px;height:140px;" src="" /></p>
      </td>
      
      <td colspan="3" rowspan="2"><?php sprint($interface['roster']['team']) ?></td>
      
      <td rowspan="2">
      	<input name="TEAM" type="text" size="20" value="<?php ifLoadedEcho('name',$team); ?>" />
      </td>
      
      <td colspan="3" class="bg1"><?php sprint($interface['roster']['rerolls']) ?></td>
      
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
      <td colspan="3" class="bg1"><?php sprint($interface['roster']['fanfactor']) ?></td>
      <td colspan="1">
      	<input class="center" name="FANFACTOR" onchange="calcExtraValue(17)" type="text" size="2" maxlength="1" value="<?php ifLoadedEcho('fanfactor',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>
	  <td colspan="3"><?php sprint($interface['roster']['journeymen']['label']) ?> (<a class="small blue" href="javascript:showJmBox()"><?php sprint($interface['roster']['journeymen']['button']) ?></a>)</td>
	  <td colspan="1"><input name="HEALTHY" type="text" size="2" readonly="readonly" /></td>
      <td colspan="3" class="bg1"><?php sprint($interface['roster']['assistants']) ?></td>
      <td colspan="1">
      	<input class="center" name="COACHES" onchange="calcExtraValue(18)" type="text" size="2" maxlength="2" value="<?php ifLoadedEcho('assistant',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


      <td colspan="3"><?php sprint($interface['roster']['race']) ?></td>
      <td>
      	<input name="RACE" type="text" size="8" value="<?php echo $chosen_race ?>" readonly="readonly" />
      </td>
      <td colspan="3" class="bg1"><?php sprint($interface['roster']['cheerleaders']) ?></td>
      <td colspan="1">
      	<input class="center" name="CHEERLEADERS" onchange="calcExtraValue(19)" type="text" size="2" maxlength="2" value="<?php ifLoadedEcho('cheerleader',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


 <td colspan="3"><?php sprint($interface['roster']['treasury']) ?></td>
      <td><input name="TREASURY" type="text" size="6" value="" /></td>
      <td colspan="3" class="bg1"><?php sprint($interface['roster']['apothecary']) ?></td>
      <td colspan="1">
      	<input class="center" name="APOTHECARY" onchange="calcExtraValue(20)" type="text" size="2" maxlength="1" value="<?php ifLoadedEcho('apothecary',$team); ?>" />
      </td>
      <td colspan="3" class="bg1">x 50.000 gp</td>
      <td>
      	<input name="VALUE[]" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
<tr>


      <td colspan="3"><?php sprint($interface['roster']['headcoach']) ?></td>
      <td>
      	<input name="HEADCOACH" type="text" size="8" value="" />
      </td>
      <td colspan="7" class="bg1"><?php sprint($interface['roster']['teamvalue']) ?></td>
      <td>
      	<input name="TEAMVALUE" type="text" size="6" readonly="readonly" value="" />
      </td>
</tr>
</table>


<div id="pic_box" class="element_hidden">
	<p><?php sprint($interface['roster']['teampics']['text']) ?></p>
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
<p><?php sprint($interface['roster']['journeymen']['text']) ?></p>
<table>
<tr>
<?php writeLegalize(); ?>
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
