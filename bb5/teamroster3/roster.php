<?php

header("Content-type: text/html; charset=utf-8");

$teamname = htmlentities($_GET['teamname']);
require_once('parse_races.php');
require_once('parse_skills.php');

?>
<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>TBT Snore</title>
<link rel="stylesheet" type="text/css" media="screen" href="styles.css" title="Default" />
<script type="text/javascript" src="functions.js"></script>
<script type="text/javascript" src="subroutines.js"></script>
<script type="text/javascript" src="roster.js"></script>
<script type="text/javascript">

<?php require_once('generate_javascript.php'); ?>

</script>

</head>

<body onload="setlogo()">
<h1>Tow Bowl Tactics Teamroster</h1>

<form id="ROSTER" action="roster_save.php" method="post" enctype="multipart/form-data">
<table>

	<tr class="bg1 center thicker-bottom">
		<td>#</td><td>PLAYER'S NAME</td><td>POSITION</td>
		<td>MA</td><td>ST</td><td>AG</td><td>AV</td>
		<td>SKILLS</td><td>INJ</td>
		<td class="bg3">COM</td><td class="bg3">TD</td><td class="bg3">INT</td>
		<td class="bg3">CAS</td><td class="bg3">MVP</td><td class="bg3">SPP</td><td>VALUE</td>
	</tr>
		
<?php

for ($i=0;$i<=15;$i++) // display this form once for each player
{
$num = $i + 1; // we want to display the numbers from 1 to 16, and not from 0 to 15
echo <<<END

<!-- ++++++ The row for player number $i. +++++++++++++ -->
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
<tr class="thicker-top">
      <td rowspan="6" colspan="2"><p><a href="javascript:show('pic_box')">Customize Teampics</a><br />
      <a href="javascript:show('background_box')">Write Background</a><br /><br />
      <input name="VERBOSE" type="checkbox" checked="checked" />Write statchanges to roster<br />
      <a href="javascript:save()">Save Roster</a></p>
      </td>
      <td rowspan="6" colspan="2" style="text-align: center"><p><img alt="a colorful picture" id="BADGE" style="width:104px;height:140px;" src="" /></p></td>
      <td colspan="3" rowspan="2">TEAM</td>
      <td rowspan="2"><input name="TEAM" type="text" size="20" value="" /></td>
      <td colspan="3" class="bg1">RE-ROLLS</td>
      <td colspan="1"><input class="center" name="REROLLS" onchange="calcExtraValue(16)" type="text" size="2" maxlength="1" value="" /></td>
      <td colspan="3" class="bg1">x <input class="bg1" name="REROLLCOST" type="text" size="5" value="<?php echo number_format($race['reroll_cost'], 0, ",", "."); ?>" readonly="readonly" /> gp</td>
      <td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
    <tr>
      <td colspan="3" class="bg1">FAN FACTOR</td>
      <td colspan="1"><input class="center" name="FANFACTOR" onchange="calcExtraValue(17)" type="text" size="2" maxlength="1" value="" /></td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
    <tr>
      <td colspan="3" rowspan="2">RACE</td>
      <td rowspan="2"><input name="RACE" type="text" size="8" value="<?php echo $teamname ?>" readonly="readonly" /></td>
      <td colspan="3" class="bg1">ASSISTANTS</td>
      <td colspan="1"><input class="center" name="COACHES" onchange="calcExtraValue(18)" type="text" size="2" maxlength="2" value="" /></td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
    <tr>
      <td colspan="3" class="bg1">CHEERLEADERS</td>
      <td colspan="1"><input class="center" name="CHEERLEADERS" onchange="calcExtraValue(19)" type="text" size="2" maxlength="2" value="" /></td>
      <td colspan="3" class="bg1">x 10.000 gp</td>
      <td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
    <tr>
      <td colspan="3">TREASURY</td>
      <td><input name="TREASURY" type="text" size="6" value="" /></td>
      <td colspan="3" class="bg1">APOTHECARY</td>
      <td colspan="1"><input class="center" name="APOTHECARY" onchange="calcExtraValue(20)" type="text" size="2" maxlength="1" value="" /></td>
      <td colspan="3" class="bg1">x 50.000 gp</td>
      <td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
    <tr>
      <td colspan="3">HEADCOACH</td>
      <td><input name="HEADCOACH" type="text" size="8" value="" /></td>
      <td colspan="7" class="bg1">TOTAL VALUE OF TEAM</td>
      <td><input name="TEAMVALUE" type="text" size="6" readonly="readonly" value="" /></td>
    </tr>
</table>


<div id="pic_box" class="element_hidden">
	<p>Why don't you just enter the filenames of the pictures you want to use.<br />
	Player without custom pics will use the default pictures.</p>
	<table>
	<?php
		echo "<tr><td>Team</td>";
		echo "<td><input name=\"TEAMLOGO\" type=\"text\" value=\"{$race['logo']}\" /></td></tr>\n";
		for($i=1; $i <= 16; $i++) {
			echo "<tr><td>$i</td><td><input name=\"DISPLAY[]\" ";
			echo "type=\"text\" value=\"\" /></td></tr>\n";
		}
	?>
	</table>
	<p><a href="javascript:hideLayer('pic_box')">Close</a></p>
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
<p><textarea name="BACKGROUND" cols="60" rows="7"><?php print(nl2br($race['background'])); ?></textarea></p>
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

<input type="hidden" name="PosNames" value="<?php for ( $i=0; $i<$race['player_num']; $i++) { echo $race['player_'.$i]['title']." ";} ?>" />
<input type="hidden" name="PosPics" value="<?php for ( $i=0; $i<$race['player_num']; $i++) { echo $race['player_'.$i]['display']." ";} ?>" />
</form>

</body>
</html>
