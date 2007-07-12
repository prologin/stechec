<script type="text/javascript">

skills_g = new Array({foreach from=$skills->General->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})
skills_p = new Array({foreach from=$skills->Passing->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})
skills_a = new Array({foreach from=$skills->Agility->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})
skills_s = new Array({foreach from=$skills->Strength->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})
skills_m = new Array({foreach from=$skills->Mutation->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})
skills_e = new Array({foreach from=$skills->Extraordinary->skill item=v name=skills}"{$v}"{if $smarty.foreach.skills.last == false},{/if}{/foreach})

warning = new Array()
{counter start=-1 print=false}{foreach from=$text->javascript->warning item=w}
warning[{counter}] = "{$w}"
{/foreach}

var MA = "{$text->MA}"
var ST = "{$text->ST}"
var AG = "{$text->AG}"
var AV = "{$text->AV}"

stats = new Array
stats[{counter start=0}] = new Array("","","","","","",99,16)
{foreach from=$race->positions->position item=p}
stats[{counter}] = new Array("{$p->title}",{$p->ma},{$p->st},{$p->ag},{$p->av},{$p->cost},{$p->qty},0,"{$p->normal}","{$p->double}","{$p->display}")
{/foreach}

skills = new Array
skills[{counter start=0}] = new Array()
{foreach from=$race->positions->position item=p}
skills[{counter}] = new Array({foreach from=$p->skills->skill item=s name=sk}"{$s}"{if $smarty.foreach.sk.last == false},{/if}{/foreach})
{/foreach}

var positions = {$race->positions->position|@count}
var apothecary = new Boolean({$race->apothecary})
var reroll_cost = {$race->reroll->cost}
var box_visible = new Boolean(false) 

</script>