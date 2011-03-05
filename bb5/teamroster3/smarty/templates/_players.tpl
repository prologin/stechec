{section name=players start=0 loop=16 step=1}
{assign var='index'  value=$smarty.section.players.index}
{assign var='number' value=`$index+1`}

<!-- The row for player number {$number}. -->

<tr>
<td class="label">{$number}</td>
<td><input name="NAME[]" onchange="checkName({$index})" type="text" size="23" maxlength="23" value="" /></td>

<td>
<select name="POSITION[]" onchange="fillPlayerStats({$index});countHealthyPlayers()">
<option value="{counter start=0}" selected="selected"></option> 
{foreach from=$race->positions->position item=p}
<option value="{counter}">{$p->title} (max. {$p->qty})</option>
{/foreach}
</select>
</td>

<td><input name="MA[]" type="text" size="2" readonly="readonly" /></td>
<td><input name="ST[]" type="text" size="2" readonly="readonly" /></td> 
<td><input name="AG[]" type="text" size="2" readonly="readonly" /></td> 
<td><input name="AV[]" type="text" size="2" readonly="readonly" /></td>

<td>
<input name="SKILLS[]" type="text" readonly="readonly" />
<a href="javascript:showSkillBox({$index})">+</a>
</td>

<td>
<input name="INJURIES[]" type="text" size="9" readonly="readonly" />&nbsp;<a class="blue" href="javascript:showInjBox({$index})">+</a>
</td> 

<td><input name="COMP[]" onchange="calcPlayerSPP({$index})" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="TD[]" onchange="calcPlayerSPP({$index})" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="INT[]" onchange="calcPlayerSPP({$index})" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="CAS[]" onchange="calcPlayerSPP({$index})" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="MVP[]" onchange="calcPlayerSPP({$index})" type="text" size="2" maxlength="2" value="" /></td> 
<td><input name="SPP[]" onchange="calcPlayerSPP({$index})" type="text" size="3" maxlength="3" value="" readonly="readonly" /></td>
<td><input name="VALUE[]" onchange="calcPlayerSPP(0)" type="text" size="6" maxlength="6" value="" readonly="readonly" class="healthy" /></td>

</tr>
{/section}