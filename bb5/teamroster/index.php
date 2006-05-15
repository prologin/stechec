<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<?
include("races.inc.php");

if($todo=="loader_xml" && $xmlfile && $r){
	$content = file($xmlfile);

	unset($nom_eq);
	unset($cla);
	unset($tre);
	unset($rel);
	unset($x_rel);
	unset($pop);
	unset($ass);
	unset($ppg);
	unset($apo);

	for($j=1;$j<=16;$j++){
		$tmp = "p".$j; unset($$tmp);
		$tmp = "n".$j; unset($$tmp);
		$tmp = "ma".$j; unset($$tmp);
		$tmp = "st".$j; unset($$tmp);
		$tmp = "ag".$j; unset($$tmp);
		$tmp = "av".$j; unset($$tmp);
		$tmp = "c".$j; unset($$tmp);
		$tmp = "bl".$j; unset($$tmp);
		$tmp = "reu".$j; unset($$tmp);
		$tmp = "td".$j; unset($$tmp);
		$tmp = "int".$j; unset($$tmp);
		$tmp = "sor".$j; unset($$tmp);
		$tmp = "jpv".$j; unset($$tmp);
		$tmp = "xp".$j; unset($$tmp);
		$tmp = "cout".$j; unset($$tmp);
	}

	while(list($nb,$ligne)=each($content)){
		if(ereg('<team name="([^"]*)" race="([^"]*)" coach="([^"]*)".* emblem="([^"]*)">',$ligne,$tok)){
			$nom_eq = $tok[1];
			$race = $tok[2];
			reset($race_singulier_tbt);
			while(list($rici,$rxml)=each($race_singulier_tbt)){
				if($rxml == $race) {$r = $rici;break;}
			}
			$coach = $tok[3];
			$emb = $tok[4];
		}
		else if(ereg('<rating>(.*)</rating>',$ligne,$tok)){
			$cla = $tok[1];
		}
		else if(ereg('<treasury>(.*)</treasury>',$ligne,$tok)){
			$tre = $tok[1];
		}
		else if(ereg('<reroll>(.*)</reroll>',$ligne,$tok)){
			$rel = $tok[1];
		}
		else if(ereg('<rerollcost>(.*)</rerollcost>',$ligne,$tok)){
			$x_rel = $tok[1];
		}
		else if(ereg('<fanfactor>(.*)</fanfactor>',$ligne,$tok)){
			$pop = $tok[1];
		}
		else if(ereg('<assistant>(.*)</assistant>',$ligne,$tok)){
			$ass = $tok[1];
		}
		else if(ereg('<cheerleader>(.*)</cheerleader>',$ligne,$tok)){
			$ppg = $tok[1];
		}
		else if(ereg('<apothecary>(.*)</apothecary>',$ligne,$tok)){
			$apo = $tok[1];
		}
		else if(ereg('<wizard>(.*)</wizard>',$ligne,$tok)){;;}
		else if(ereg('<formation>(.*)</formation>',$ligne,$tok)){;;}
		else if(ereg('<player name="([^"]*)" number="([^"]*)"',$ligne,$tok)){
			$player_id = $tok[2];
			$tmp = "n".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<ma>(.*)</ma>',$ligne,$tok)){
			$tmp = "ma".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<st>(.*)</st>',$ligne,$tok)){
			$tmp = "st".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<ag>(.*)</ag>',$ligne,$tok)){
			$tmp = "ag".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<av>(.*)</av>',$ligne,$tok)){
			$tmp = "av".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<positionid>(.*)</positionid>',$ligne,$tok)){
			$tmp = "p".$player_id; 
			$posid = $tok[1];
			$posids = array_flip($positionid[$race]);
			$$tmp = $posids[$posid];
		}
		else if(ereg('<cost>(.*)</cost>',$ligne,$tok)){
			$tmp = "cout".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<skill>(.*)</skill>',$ligne,$tok)){
			$tmp = "c".$player_id; 
			if($$tmp) $$tmp .= ",";
			$$tmp .= $tok[1];
		}
		else if(ereg('<bl>(.*)</bl>',$ligne,$tok)){
			$tmp = "bl".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<reu>(.*)</reu>',$ligne,$tok)){
			$tmp = "reu".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<td>(.*)</td>',$ligne,$tok)){
			$tmp = "td".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<int>(.*)</int>',$ligne,$tok)){
			$tmp = "int".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<sor>(.*)</sor>',$ligne,$tok)){
			$tmp = "sor".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<jpv>(.*)</jpv>',$ligne,$tok)){
			$tmp = "jpv".$player_id; $$tmp = $tok[1];
		}
		else if(ereg('<xp>(.*)</xp>',$ligne,$tok)){
			$tmp = "xp".$player_id; $$tmp = $tok[1];
		}
	}
}
else{
	$apo=0; 
	$ppg=0; 
	$ass=0; 
	$cla=100; 
	$pop=0; 
	$rel=0; 
	$tre=0; 
}

?>

<HTML>
<HEAD>
<TITLE>Editeur d'équipe pour Tow Bowl Tactics</TITLE>
<meta name="author" content="mathieu">
<META NAME="Keywords" CONTENT="toweld, blood bowl, tow bowl tactics, tbt, game, sdl">
<META NAME="Description" CONTENT="Tow Bowl - Tactics - Homepage">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<LINK rel="stylesheet" href="roster.css" type="text/css">
<LINK rel="stylesheet" href="/tbt.css" type="text/css">

<script language="Javascript" src="description_race.js" type="text/javascript"></script> 
<script language="Javascript" src="skills_en_to_fr.js" type="text/javascript"></script>
<script language="Javascript" src="races.js" type="text/javascript"></script> 
<script language="Javascript" src="roster.js" type="text/javascript"></script> 
<script language="Javascript" src="skills_traits.js" type="text/javascript"></script>

<script language="Javascript" type="text/javascript">
function set_skills(o,pos){
	var c="";
	var c_lg="";
	for(var i = 0; i < o.length; i++) {
		if (o.options[i] != null){
			if(i!=0) c += ",";
			if(i!=0) c_lg += ",";
			c += o.options[i].value;
			c_lg += o.options[i].text;
		}
	}
	document.getElementById('c'+pos).value = c;
	document.getElementById('c_lg'+pos).innerHTML = c_lg;
}

</script>
</HEAD>

<BODY BACKGROUND="/img/bg.jpg">
<CENTER><A HREF="/"><IMG SRC="/img/title_marbre.gif" WIDTH="320" HEIGHT="113" BORDER=0 ALT="Tow Bowl -Tactics- "></A></CENTER>

<BR>

<? if(!$r) { ?> 
<center>
<font class="texte">
Cette page permet de générer <br>
des fichiers d'équipe au format XML, directement <br>
utilisables dans TBT (jusqu'a la version O.5)<br>
<br>
Toweld, 7 janvier 2006<br>
</font>
</center>
<? } ?>

<BR>

<center>
<form id="feuille" action="/teamroster/" method="post" enctype="multipart/form-data">
<input type="hidden" id="todo" name="todo" value="">
<? if(!$r) { ?>
<input type="hidden" id="r" name="r" value="">
<font class="texte">Choissisez la race de l'équipe à créer : </font>
<select id="race" name="race" class="liste" onChange="change_description_race();">
	<option selected>---</option>	
	<option>Amazones</option>
	<option>Chaos</option>
	<option>Elfes</option>
	<option>Elfes Noirs</option>
	<option>Goblins</option>
	<option>Halflings</option>
	<option>Hauts Elfes</option>
	<option>Elfes Sylvains</option>
	<option>Hommes Lezards</option>
	<option>Humains</option>
	<option>Khemris</option>
	<option>Morts-Vivants</option>
	<option>Nains</option>
	<option>Nains Du Chaos</option>
	<option>Necromantiques</option>
	<option>Nordiques</option>
	<option>Ogres</option>
	<option>Orques</option>
	<option>Pourris de Nurgle</option>
	<option>Skavens</option>
	<option>Vampires</option>
</select>
<input type="button" id="valider_race" name="valider_race" value="Go" onClick="choix_race();" class="bouton">
<br>
<br>
<table><tr>
<td width="72" bgcolor='#FFFFFF'><img id="desc_emb_race" src="emblems/no_emblem.gif" width="70" height="93" border="1" style='border-color:#DDDDDD' alt=''></td>
<td width="628"><div align="center" id="texte_race" style='font-family:verdana,arial,helvetica,sans-serif;font-size:12px;background-color:#FFFFFF'>---</div></td></tr></table>

<? } else { 
include("teams/".$r.".inc.php");
?>

<div style='z-index:10;position:absolute;display:none;left:0px;top:0px;width:300px;height:200px;' id='div_skills'><center>

<TABLE border="1" bgcolor="#FFFFEE" style='border-color:#BBBBBB' cellspacing="0" cellpadding="1" width="100%">
<TR><td colspan='3' align=right><a style="width:10px;height:10px" class='bouton' href='javascript:close_skill_popup()'>X</a></td></tr>
<TR>
	<TD align="center" width="140"><font class="tres_petit_texte">Actuellement</font></TD><TD width=20>&nbsp;</TD><TD align="center" width="140"><font class="tres_petit_texte">Possibilité&nbsp;<font id='skill_cat_possible' class="tres_petit_texte"></font></font></TD>
</TR>
	<TD align="right" width="140"><SELECT size="10" id='liste_skill_avant' NAME="liste_skill_avant" class="liste"><option value=''></SELECT></TD>
	<TD align="center" width="20">
	<input style="width:16px;font-size:8px;" type="button" name="ajout_skill_btn" value="&lt;" onClick="ajouter_skill();" class="bouton"><br><br>
	<input style="width:16px;font-size:8px;" type="button" name="retirer_skill_btn" value="&gt;" onClick="retirer_skill();" class="bouton"></TD>
	<TD align="right" width="140">
	<SELECT size="10" id='liste_skill_apres' NAME="liste_skill_apres" class="liste"><option value=''></SELECT>
	</TD>
</TR>
<TR>
	<TD align="center" colspan="3"><input type="button" name="close_btn" value="Valider" onClick="save_and_close_skill();" class="bouton"></TD>
</TR>
</TABLE>
</center>
</div>

<input type="hidden" id="x_rel" name="x_rel" value="<?= $x_rel ?>">
<input type="hidden" id="r" name="r" value="<?= $r ?>">
<table border="1" cellspacing="0" cellpadding="1" bgcolor="#FFFFEE" style='border:1px solid #BBBBBB;'>
    <tr align="center" valign="middle"> 
      <td width="10">&nbsp; </td>
      <td colspan="2" ><font class="titre"><b>Nom du joueur</b></font></td>
      <td><font class="titre"><b>Position</b></font></td>
      <td><font class="titre"><b>MA</b></font></td>
      <td><font class="titre"><b>ST</b></font></td>
      <td><font class="titre"><b>AG</b></font></td>
      <td><font class="titre"><b>AV</b></font></td>
      <td><font class="titre"><b>Comp&eacute;tences</b></font></td>
      <td><font class="titre"><b>BL</b></font></td>
      <td><font class="titre"><b>REU</b></font></td>
      <td><font class="titre"><b>TD</b></font></td>
      <td><font class="titre"><b>INT</b></font></td>
      <td><font class="titre"><b>SOR</b></font></td>
      <td><font class="titre"><b>JPV</b></font></td>
      <td><font class="titre"><b>EXP</b></font></td>
      <td><font class="titre"><b>Co&ucirc;t</b></font></td>
    </tr>



	<?  
		for($j=1;$j<=16 ;$j++) { 

			$tmp = "p".$j; $var_p = $$tmp;
			$tmp = "n".$j; $var_n = $$tmp;
			$tmp = "ma".$j; $var_ma = $$tmp;
			$tmp = "st".$j;	$var_st = $$tmp;
			$tmp = "ag".$j;	$var_ag = $$tmp;
			$tmp = "av".$j;	$var_av = $$tmp;
			$tmp = "c".$j;	$var_c = $$tmp;
			$tmp = "bl".$j;	$var_bl = $$tmp;
			$tmp = "reu".$j; $var_reu = $$tmp;
			$tmp = "td".$j;	$var_td = $$tmp;
			$tmp = "int".$j; $var_int = $$tmp;			
			$tmp = "sor".$j; $var_sor = $$tmp;
			$tmp = "jpv".$j; $var_jpv = $$tmp;
			$tmp = "xp".$j;	$var_xp = $$tmp;
			$tmp = "cout".$j; $var_cout = $$tmp;

			$var_n = ($var_n)?($var_n):("");
			$var_p = ($var_p)?($var_p):("---");
			$var_ma = ($var_ma)?($var_ma):(0);
			$var_st = ($var_st)?($var_st):(0);
			$var_ag = ($var_ag)?($var_ag):(0);
			$var_av = ($var_av)?($var_av):(0);
			$var_c = ($var_c)?($var_c):("");
			$var_bl = ($var_bl)?($var_bl):(0);
			$var_reu = ($var_reu)?($var_reu):(0);
			$var_td = ($var_td)?($var_td):(0);
			$var_int = ($var_int)?($var_int):(0);
			$var_sor = ($var_sor)?($var_sor):(0);
			$var_jpv = ($var_jpv)?($var_jpv):(0);
			$var_xp = ($var_xp)?($var_xp):(0);			
			$var_cout = ($var_cout)?($var_cout):(0);

//			$var_n = stripslashes($var_n);
	?>
	<tr id='tr_<?=$j?>'>
		<td align="center" valign="middle"><font class="titre"><b><?= $j ?></b></font></td>
		<td align="center" valign="middle" colspan="2"><input type="text" id="n<?= $j ?>" name="n<?= $j ?>"	size="25" class="texte" value="<?= $var_n ?>"></td>
		<td align="center" valign="middle"><select id="p<?= $j ?>" name="p<?= $j ?>" class="liste" onChange="if(this.selectedIndex!=<?= sizeof($position) ?>) set_default_value(this,'<?= $j ?>');else reset_value('<?= $j ?>'); calculer_total_equipe();">	
		<?
			$selected = false;
			reset($position);
			while (list ($key, $value) = each ($position)) { 
				echo "<option ".(($value===$var_p)?"selected":"").">$value</option>";
			}
		?>
		<option <?= ($var_p==="---")?"selected":"" ?>>---</option></select></td>
		<td align="center" valign="middle"><input type='text' maxlength=2 id="ma<?= $j ?>" name="ma<?= $j ?>" size=2 class="nombre" value='<?=$var_ma?>'></td>
		<td align="center" valign="middle"><input type='text' maxlength=2 id="st<?= $j ?>" name="st<?= $j ?>" size=2 class="nombre" value='<?=$var_st?>'></td>
		<td align="center" valign="middle"><input type='text' maxlength=2 id="ag<?= $j ?>" name="ag<?= $j ?>" size=2 class="nombre" value='<?=$var_ag?>'></td>
		<td align="center" valign="middle"><input type='text' maxlength=2 id="av<?= $j ?>" name="av<?= $j ?>" size=2 class="nombre" value='<?=$var_av?>'></td>
		<td align="left" valign="middle" nowrap><input type='hidden' name='c<?=$j?>' id='c<?=$j?>' value='<?= $var_c ?>'><table cellspacing=0 cellpadding=0 border=0 width='100%' style='line-height:11px'><tr><td align='left'><font id="c_lg<?= $j ?>"  class="texte_sans_w"></font></td><td align='right'><input type="button" value="+" style='font-size:8px;width:16px;height:16px' class="bouton" onClick="show_skill_popup('<?= $j ?>','<?= $r ?>',document.getElementById('p<?= $j ?>').selectedIndex,document.getElementById('c<?=$j?>').value)"></td></tr></table><script language='Javascript' type='text/javascript'>document.getElementById('c_lg<?=$j?>').innerHTML = translate('<?= $var_c ?>');</script></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="bl<?= $j ?>" name="bl<?= $j ?>" size=2 class="nombre" value='<?=$var_bl?>'></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="reu<?= $j ?>" name="reu<?= $j ?>" size=2 class="nombre" value='<?=$var_reu?>'></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="td<?= $j ?>" name="td<?= $j ?>" size=2 class="nombre" value='<?=$var_td?>'></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="int<?= $j ?>" name="int<?= $j ?>" size=2 class="nombre" value='<?=$var_int?>'></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="sor<?= $j ?>" name="sor<?= $j ?>" size=2 class="nombre" value='<?=$var_sor?>'></td>
		<td align="center" valign="middle"><input onChange="calculer_xps(<?= $j ?>);" maxlength=2 type='text' id="jpv<?= $j ?>" name="jpv<?= $j ?>" size=2 class="nombre" value='<?=$var_jpv?>'></td>
		<td align="center" valign="middle"><input type='hidden' id='xp_v<?=$j?>' name='xp_v<?=$j?>' value='<?=$var_xp?>'><font id="xp<?= $j ?>" class="nombre"><?= $var_xp ?></font></td>
		<td align="right" valign="middle"><input type='hidden' id='cout_v<?=$j?>' name='cout_v<?=$j?>' value='<?=$var_cout?>'><font id="cout<?= $j ?>" class="nombre"><?= $var_cout ?></font></td>
	</tr>
	
<?
}
	?>
    <tr> 
      <td colspan="2" rowspan="6" align="center">
<?	
		if($race_tbt[$r]=='') 
			$filename="no_emblem.gif";
		else 
			$filename=$race_tbt[$r].".jpg";
	
		if( $emb ) $filename=$emb;
?>	
		<img id="emb_img" src="emblems/<?= $filename ?>" width="70" height="93" style='border:1px solid #000000;cursor:hand' onClick="popup_embleme(document.getElementById('emb').value);" alt="<?= $filename ?>"><br>
		<input type="hidden" id="emb" name="emb" value="<?=$filename?>">
		<input type="button" value="Changer" id="btn_changer_image" name="btn_changer_image" class="bouton" onClick="popup_embleme(document.getElementById('emb').value);">
	  </td>
      <td colspan="3" valign="middle" align="right"><font class="titre">Coach&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" id="coach" name="coach"  class="texte" size="20" value="<?= $coach ?>">
      </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Relance&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type='text'  maxlength=1 id="rel" name="rel" size=2 class="nombre" value='<?=$rel?>' onChange="calculer_total_equipe()">
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre"><?= $x_rel?> PO&nbsp;</font></td>
      <td align="right" valign="middle">
		<input type='hidden' name='cout_rel_v' id='cout_rel_v' value='<?=$cout_rel?>'>
        <font id="cout_rel" class="nombre"><?= $cout_rel ?></font>
      </td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Race&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $r ?></font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Popularit&eacute;&nbsp;</font></td>
      <td align="right" valign="middle"> 
        <input type='text' id="pop" maxlength=1 name="pop" size=2 class="nombre" value='<?=$pop?>' onChange="calculer_total_equipe()">
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle">
		<input type='hidden' name='cout_pop_v' id='cout_pop_v' value='<?=$cout_pop?>'>
        <font id="cout_pop" class="nombre"><?= $cout_pop ?></font>
      </td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Classement&nbsp;</font></td>
      <td colspan="4">
		<input id="cla_v" name="cla_v" type="hidden" value='<?= $cla ?>'>
		<font id="cla" class="nombre" style='text-align:left'><?= $cla ?></font>
  	  </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Assistants&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type='text' id="ass" maxlength=1 name="ass" size=2 class="nombre" value='<?=$ass?>' onChange="calculer_total_equipe()">
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle">
		<input id="cout_ass_v" name="cout_ass_v" type="hidden" value='<?= $cout_ass ?>'>
        <font id="cout_ass" class='nombre'><?= $cout_ass ?></font>
      </td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Tr&eacute;sor&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" id="tre" name="tre" class="nombre" style='width:50px;text-align:left' value="<?= $tre ?>">
      </td>
      <td colspan="3" align="right" valign="middle" nowrap><font class="titre">Pom-pom girls&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type='text' maxlength=1 id="ppg" name="ppg" size=2 class="nombre" value='<?=$ppg?>' onChange="calculer_total_equipe()">
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle" > 
		<input id="cout_ppg_v" name="cout_ppg_v" type="hidden" value='<?= $cout_ppg ?>'>
        <font id="cout_ppg" class="nombre"><?= $cout_ppg ?></font>
      </td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Nom de 
        l'&eacute;quipe&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" id="nom_eq" name="nom_eq" class="texte" size="35"  value="<?= $nom_eq ?>">
      </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Apothiquaire&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type='text' id="apo" maxlength=1 name="apo" size=2 class="nombre" value='<?=$apo?>' onChange="calculer_total_equipe()">
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">50000 PO&nbsp;</font></td>
      <td align="right" valign="middle" > 
		<input type='hidden' id="cout_apo_v" name="cout_apo_v" value="<?= $cout_apo ?>">
        <font id="cout_apo" class="nombre"><?= $cout_apo ?></font>
      </td>
    </tr>
    <tr> 
      <td colspan="7" valign="middle" align="left" nowrap width=290>

<div style='display:none' id='xmlfile_div'><input type="file" id="xmlfile" name="xmlfile" class="bouton">&nbsp;<input type='button' class='bouton' id='xmlfile_btn' name='xmlfile_btn' value="OK" onclick='importer_xml_team()'>&nbsp;<a href='javascript:close_xmlfile_picker()' class='bouton'>x</a></div>

		<input type="hidden" id="dl_xml" name="dl_xml" value="0">
		<img alt='Importer xml' onMouseOver="set_action_label('Importer équipe xml')" onMouseOut="set_action_label('')" id="load_btn"  class="bouton" onClick="show_xmlfile_picker();" align="middle" src='load.gif'>
		<img alt='Sauver xml' onMouseOver="set_action_label('Sauver équipe xml')" onMouseOut="set_action_label('')" id="xml_btn" class="bouton" onClick="document.getElementById('dl_xml').value=1;generer_xml();" align="middle" src='save.gif'>
		<img alt='Apercu xml' onMouseOver="set_action_label('Voir fichier xml')" onMouseOut="set_action_label('')" id="voir_btn" class="bouton" onClick="document.getElementById('dl_xml').value=0;generer_xml();" align="middle" src='preview.gif'>
		<img alt='Version imprimable' onMouseOver="set_action_label('Version imprimable')" onMouseOut="set_action_label('')" id="html_btn" class="bouton" onClick="version_imprimable();" align="middle" src='print.gif'>&nbsp;<font id='action_label' class='titre' style='font-weight:bold'></font></td>
      <td colspan="7" valign="middle" align="right"><font class="titre"><input type="button" id="btn_calc_total" name="btn_calc_total" value="Calculer" onClick="calculer_total_equipe();" class="bouton">&nbsp;&nbsp;&nbsp;<b>Co&ucirc;t 
        total de l'&eacute;quipe</b></font></td>
		<td align="right" valign="middle"><input type='hidden' name='cout_eq_v' id='cout_eq_v' value='<?=$cout_eq?>'><font id="cout_eq" class="nombre"><?= $cout_eq ?></font></td>
    </tr>
  </table>

<script language="javascript" type="text/javascript">calculer_total_equipe();</script>
<br>
<pre>A tout moment, vous pouvez aussi configurer vos formations... 
<input type="button" id="edit_formation_btnl" name="edit_formation_btnl" value="Editeur de formation" onClick="open_editeur_formation();" class="bouton">
</pre>
<? } ?>



</form>
</center>
<p><a href="http://validator.w3.org/check/referer"><img border="0" src="valid-html40.gif" alt="Valid HTML 4.0!" height="31" width="88"></a></p>

<A HREF="/forum/">TBT - Forum de discussion</A><br>
<A HREF="/">TBT - Index</A><br>
</BODY>
</HTML>

