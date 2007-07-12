<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<title>TBT Snore</title>
<link rel="stylesheet" type="text/css" media="screen" href="roster.css" title="Default" />
<link rel="stylesheet" type="text/css" media="print" href="roster_print.css" title="Print">
<script type="text/javascript" src="onstage/functions.js"></script>
<script type="text/javascript" src="onstage/subroutines.js"></script>
<script type="text/javascript" src="onstage/roster.js"></script>
{include file='_javascript.tpl'}
</head>

<body onload="">
<h1>Tow Bowl Tactics Teamroster</h1>

<form id="ROSTER" action="save_team.php" method="post" enctype="multipart/form-data">
<table>

<tr class="label">
<td>#</td>
<td>{$text->name}</td>
<td>{$text->position}</td>
<td>{$text->MA}</td>
<td>{$text->ST}</td>
<td>{$text->AG}</td>
<td>{$text->AV}</td>
<td>{$text->skill->skills|upper}</td>
<td>{$text->injury->inj|upper}</td>
<td>{$text->comp}</td>
<td>{$text->td}</td>
<td>{$text->int}</td>
<td>{$text->cas}</td>
<td>{$text->mvp}</td>
<td>{$text->spp}</td>
<td>{$text->value}</td>
</tr>

{include file='_players.tpl'}

<!-- End of players. -->

<tr class="separator">

<td rowspan="6" colspan="2" style="width: 200px">
<p>
<a href="javascript:showBox('pic_box')">{$text->teampics->label}</a><br />
<a href="javascript:showBox('background_box')">{$text->background->label}</a><br />
<input name="VERBOSE" type="checkbox" checked="checked" />{$text->statchanges}<br />
<a href="javascript:save()">{$text->save}</a><br />
</p>
</td>

<td rowspan="6" colspan="2" style="text-align: center">
<p><img alt="a colorful picture" style="width:104px;height:140px;" src="data/logos/{$race->emblem}" /></p>
</td>

<td class="label" colspan="3" rowspan="2">{$text->team}</td>
<td rowspan="2"><input name="TEAM" type="text" size="20" value="" /></td>

<td class="label" colspan="3">{$text->rerolls}</td>
<td colspan="1"><input name="REROLLS" onchange="calcExtraValue(16)" type="text" size="2" maxlength="1" value="" /></td>
<td colspan="3" class="label">x {$race->reroll->cost}</td>
<td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

<tr>

<td class="label" colspan="3" class="bg1">{$text->fanfactor}</td>
<td colspan="1"><input  name="FANFACTOR" onchange="calcExtraValue(17)" type="text" size="2" maxlength="1" value="" /></td>
<td colspan="3" class="label">x 10000</td>
<td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

<tr>
	
<td class="label" colspan="3">{$text->journeymen->label} <a class="blue" href="javascript:showJmBox()">({$text->journeymen->button})</a></td>
<td colspan="1"><input name="HEALTHY" type="text" size="2" readonly="readonly" /></td>

<td colspan="3" class="label">{$text->assistants}</td>
<td colspan="1"><input  name="COACHES" onchange="calcExtraValue(18)" type="text" size="2" maxlength="2" value="" /></td>
<td colspan="3" class="label">x 10000</td>
<td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

<tr>

<td colspan="3" class="label">{$text->race}</td>
<td><input name="RACE" type="text" size="8" value="{$race->name}" readonly="readonly" /></td>

<td colspan="3" class="label">{$text->cheerleaders}</td>
<td colspan="1"><input  name="CHEERLEADERS" onchange="calcExtraValue(19)" type="text" size="2" maxlength="2" value="" /></td>
<td colspan="3" class="label">x 10000</td>
<td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

<tr>

<td colspan="3" class="label">{$text->treasury}</td>
<td><input name="TREASURY" type="text" size="6" value="" /></td>

<td colspan="3" class="label">{$text->apothecary}</td>
<td colspan="1"><input  name="APOTHECARY" onchange="calcExtraValue(20)" type="text" size="2" maxlength="1" value="" /></td>
<td colspan="3" class="label">x 50000</td>
<td><input name="VALUE[]" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

<tr>

<td colspan="3" class="label">{$text->headcoach}</td>
<td><input name="HEADCOACH" type="text" size="8" value="" /></td>

<td colspan="7" class="label">{$text->teamvalue}</td>
<td><input name="TEAMVALUE" type="text" size="6" readonly="readonly" value="" /></td>

</tr>

</table>

<!-- End of table -->

<!-- Skills -->

{include file='_skills.tpl'}

<!-- Injuries -->

{include file='_injuries.tpl'}

<!-- Journeymen -->

{include file='_journeymen.tpl'}

<!-- Pictures -->

{include file='_pictures.tpl'}

<!-- Background -->

<div id="background_box" class="element_hidden">
	<a href="javascript:hide('background_box')">{$text->close}</a>
	<h2>{$text->background->label}</h2>
	<p>{$text->background->text}</p>
	<p>
		<textarea name="BACKGROUND" cols="60" rows="7">{$race->background|nl2br}</textarea>
	</p>
</div>

<!-- End of boxes -->

</form>

</body>
</html>