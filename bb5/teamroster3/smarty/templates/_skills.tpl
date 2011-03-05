<div id="skill_box" class="element_hidden">
<a href="javascript:hide('skill_box')">{$text->cancel}</a>

<h2>{$text->player} <input name="TEMP1" type="text" readonly="readonly" size="2" /></h2>
<input style="display: none;" name="TEMP3" type="text" readonly="readonly" size="6" value="0" />

<h3>{$text->skills}</h3>

<table>
	<tr>
		<td class="label">{$text->skill->own}</td>
		<td><select class="wide" name="SKILLDEST"><option></option></select></td>
		<td class="button"><a href="javascript:remskill()" class="minus">-</a></td>
	</tr>
	<tr>
		<td class="label">{$text->skill->normal}</td>
		<td><select class="wide" name="SKILLSRCN"><option></option></select></td>
		<td class="button"><a href="javascript:addskill('n')" class="plus">+</a></td>
	</tr>
	<tr>
		<td class="label">{$text->skill->double}</td>
		<td><select class="wide" name="SKILLSRCD"><option></option></select></td>
		<td class="button"><a href="javascript:addskill('d')" class="plus">+</a></td>
	</tr>
	<tr>
		<td class="label">{$text->skill->forbidden}</td>
		<td><select class="wide" name="SKILLSRCF"><option></option></select></td>
		<td class="button"><a href="javascript:addskill('i')" class="minus">+</a></td>
	</tr>
</table>

<h3>{$text->skill->stat}</h3>

<table>
	<tr>
		<td class="button"><a class="positive" href="javascript:changeStat('MA','p')">+{$text->MA}</a></td>
		<td class="button"><a class="positive" href="javascript:changeStat('ST','p')">+{$text->ST}</a></td>
		<td class="button"><a class="positive" href="javascript:changeStat('AG','p')">+{$text->AG}</a></td>
		<td class="button"><a class="positive" href="javascript:changeStat('AV','p')">+{$text->AV}</a></td>
	</tr>
</table>

</div>