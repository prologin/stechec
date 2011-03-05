<?php

function parse($xmlFile)
{
	$xml = simplexml_load_file($xmlFile);
	return $xml;
}

function returnRace($chosen_race, $lang)
{	
	$races = parse('data/'.$lang.'/races.xml');

	foreach ( $races->race as $race )
	{
		if ( $chosen_race == (string) $race->name )
		{
			return $race;
		}
	}
	
	return NULL; // 'else'
}

function listRaces($lang)
{
	$list = array();
	
	$races = parse('data/'.$lang.'/races.xml');
	
	foreach ( $races->race as $race )
	{
			$list[] = (string) $race->name;
	}
	
	return $list;	
}

function listSkills($lang)
{
	$list = array();
	
	$xml = parse('data/'.$lang.'/skills.xml');
	$skills = &$xml->skills;
	
	foreach ( $skills->children() as $type ) 
	{
		foreach ( $type->children() as $skill )
		{
			$list[] = (string) $skill;
		}
	}
	
	return $list;
}

function listChildren($parent)
{
	$list = array();
	
	foreach ( $parent->children() as $child )
	{
		$list[] = (string) $child;
	}
	
	return $list;
}

function listPositions($teamname, $lang)
{
	$list = array();
	
	$races = parse('data/'.$lang.'/races.xml');
	
	foreach ( $races->race as $race )
	{
		if ( $teamname == (string) $race->name )
		{
			foreach ( $race->positions->position as $position)
			{
				$list[] = (string) $position->title;
			}
			
			break;
		}
	}
	
	return $list;
}



function buildTranslation($teamname, $lang_from, $lang_to)
{
	$i1 = parse('data/'.$lang_from.'/roster.xml');
	$i2 = parse('data/'.$lang_to.'/roster.xml');
	
	$stats_from = array($i1->MA, $i1->ST, $i1->AG, $i1->AV);
	$stats_to = array((string)$i2->MA, (string)$i2->ST, (string)$i2->AG, (string)$i2->AV);
	
	$skills_from = listSkills($lang_from);
	$skills_to = listSkills($lang_to);
	
	$races_from = listRaces($lang_from);
	$races_to = listRaces($lang_to);
	
	/* 	a problem arises for the translation of the
		positions of race R1 in langage L1:
		how to know the name of R1 in langage L2?
		-> a temporary translation is used
	*/
	
	$translation = array_combine($races_from, $races_to);
		
	$positions_from = listPositions($teamname, $lang_from);
	$positions_to = listPositions($translation[(string)$teamname], $lang_to);

	$from = array_merge($skills_from, $races_from, $positions_from, $stats_from);
	$to = array_merge($skills_to, $races_to, $positions_to, $stats_to);
	
	$translation = array_combine($from, $to);
	return $translation;
}
?>