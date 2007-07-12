<div id="pic_box" class="element_hidden">
	
<p><a href="javascript:hide('pic_box')">{$text->close}</a></p>

<h2>{$text->teampics->label}</h2>
<p>{$text->teampics->text}</p>

<table>

<tr>
<td class="label">{$text->team}</td>
<td><input name="TEAMLOGO" type="text" value="{$race->emblem}" /></td>
</tr>

{section name=pictures start=0 loop=11 step=1}
{assign var='number'  value=`$smarty.section.pictures.index+1`}
<tr>
<td class="label">{$number}</td>
<td><input name="DISPLAY[]" type="text" value=""></td>
</tr>
{/section}

</table>

</div>