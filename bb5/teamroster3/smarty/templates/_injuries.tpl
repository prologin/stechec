<div id="inj_box" class="element_hidden">
<a href="javascript:hide('inj_box')">{$text->cancel}</a>

<h2>{$text->player} <input name="TEMP2" type="text" readonly="readonly" size="2" /></h2>
<h3>{$text->injury->inj}</h3>

<table>
<tr>
	<td class="button" colspan="2">
		<a id="miss" class="negative" href="javascript:setInjury('M')">{$text->injury->miss}</a>
		<a id="ok" class="positive" href="javascript:setInjury('')">{$text->injury->ok}</a>	
	</td>
	<td class="button" colspan="2">
		<a class="negative" href="javascript:setInjury('N')">{$text->injury->niggling}</a>
	</td>
</tr>
<tr>
	<td class="button"><a class="negative" href="javascript:changeStat('MA','m')">-{$text->MA}</a></td>
	<td class="button"><a class="negative" href="javascript:changeStat('ST','m')">-{$text->ST}</a></td>
	<td class="button"><a class="negative" href="javascript:changeStat('AG','m')">-{$text->AG}</a></td>
	<td class="button"><a class="negative" href="javascript:changeStat('AV','m')">-{$text->AV}</a></td>
</tr>
</table>

</div>