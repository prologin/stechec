<div id="jm_box" class="element_hidden">
<p><a href="javascript:hide('jm_box')">{$text->close}</a></p>

<h2>{$text->journeymen->jm|capitalize}</h2>
<p>{$text->journeymen->text}</p>

<table>
<tr>
{section name=journeymen start=0 loop=8 step=1}
{assign var='index' value=$smarty.section.journeymen.index}
{assign var='number' value=`$index+1`}
<td class= "button"><a id="jm{$index}" class="neutral" href="javascript:legalize({$index})">{$number}</a></td>
{/section}
</tr>
<p>&nbsp;</p>
<tr>
{section name=journeymen start=8 loop=16 step=1}
{assign var='index' value=$smarty.section.journeymen.index}
{assign var='number' value=`$index+1`}
<td class= "button"><a id="jm{$index}" class="neutral" href="javascript:legalize({$index})">{$number}</a></td>
{/section}
</tr>
</table>

</div>