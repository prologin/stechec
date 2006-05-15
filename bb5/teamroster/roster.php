<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<?

if(!$editable) $editable=0; 
//if(!$sorc) $sorc=0; 
if(!$apo) $apo=0; 
if(!$ppg) $ppg=0; 
if(!$ass) $ass=0; 
if(!$cla) $cla=100; 
if(!$pop) $pop=0; 
if(!$rel) $rel=0; 
if(!$x_rel) $x_rel=0; 
//if(!$cout_sorc) $cout_sorc=0; 
if(!$cout_apo) $cout_apo=0; 
if(!$cout_ppg) $cout_ppg=0; 
if(!$cout_ass) $cout_ass=0; 
if(!$cout_pop) $cout_pop=0; 
if(!$cout_rel) $cout_rel=0; 
if(!$cout_eq) $cout_eq=0; 
if(!$tre) $tre=0; 
if(!$r) $r=0; 

?>

<HTML>
<HEAD>
<TITLE>Editeur d'équipe pour Tow Bowl Tactics</TITLE>
<META NAME="Author" CONTENT="toweld@wanadoo.fr">
<META NAME="Keywords" CONTENT="toweld, blood bowl, tow bowl tactics, tbt, game, sdl">
<META NAME="Description" CONTENT="Tow Bowl - Tactics - Homepage">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<LINK rel="stylesheet" href="roster.css" type="text/css">

<script language="Javascript" src="description_race.js"></script> 
<script language="Javascript" src="skills_en_to_fr.js"></script> 
<script language="Javascript" src="races.js"></script> 
<script language="Javascript" src="roster.js"></script> 

<script language="Javascript">
function set_skills(o,pos){
	var c="";
	var c_orig="";
	for(var i = 0; i < o.length; i++) {
		if (o.options[i] != null){
			if(i!=0) c += ",";
			if(i!=0) c_orig += ",";
			c += o.options[i].text;
			c_orig += o.options[i].value;
		}
	}

	eval('document.feuille.c_lg'+pos+'.value = c');
	eval('document.feuille.c'+pos+'.value = c_orig');
}

var myimages=new Array();
function preloadimages(){
	for (i=0;i<emblem_tbt.length;i++){
		window.status = "Loading emblems/" + emblem_tbt[i] + ".jpg ...";
		myimages[i]=new Image();
		myimages[i].src="emblems/" + emblem_tbt[i] + ".jpg";
	}

	myimages[i+1]=new Image();
	myimages[i+1].src="../img/fr_over.jpg";
	myimages[i+2]=new Image();
	myimages[i+2].src="../img/en_over.jpg";
	window.status = "";
}

</script>

</HEAD>

<BODY BACKGROUND="../img/bg.jpg" <? if( !$r ) echo "onLoad='preloadimages()'" ?> >
<CENTER><IMG SRC="../img/title_marbre.gif" WIDTH="320" HEIGHT="113" BORDER=0 ALT="Tow Bowl -Tactics-"></CENTER>

<BR>

<center>

<? if(!$r) { ?> 
<table cellspacing="10" bgcolor="#FFFFFF"><tr>
<td align="center"><a href="http://toweld.free.fr/towbowltactics/teamroster/"><img border="0" src="../img/fr.jpg" onmousemove="this.src='../img/fr_over.jpg'" onmouseout="this.src='../img/fr.jpg'"><br>
<font face="Arial,helvetica,sans-serif" size="1">Version francaise</font></a>
</td>
<td align="center">
<font face="Arial,helvetica,sans-serif" size="2">
Cette page permet de générer <br>
des fichiers d'équipe au format XML, directement <br>
utilisables dans Tow Bowl Tactics...<br>
<br>
Toweld, 30 aout 2002<br>
</font>
</td>
<td align="center"><a href="http://toweld.free.fr/towbowltactics/teamroster/en/"><img border="0" src="../img/en.jpg" onmousemove="this.src='../img/en_over.jpg'" onmouseout="this.src='../img/en.jpg'"><br>
<font face="Arial,helvetica,sans-serif" size="1">English version</font></a>
</td>
</tr></table>

<? } ?>

</center>


<BR>

<center>
<form name="feuille" action="roster.php" method="post">

<? if(!$r) { ?>
<input type="hidden" name="r" value="">
<font class="text">Choissisez la race de l'équipe à créer : </font>
<select name="race" class="liste" onChange="change_description_race();">
	<option selected>---</option>	
<!--	<option>Amazones</option> -->
	<option>Chaos</option>
	<option>Elfes Noirs</option>
	<option>Goblins</option>
	<option>Halflings</option>
	<option>Hauts Elfes</option>
	<option>Elfes Sylvains</option>
<!--	<option>Hommes Lezards</option> -->
	<option>Humains</option>
	<option>Morts-Vivants</option>
	<option>Nains</option>
	<option>Nains Du Chaos</option>
	<option>Nordiques</option>
	<option>Orques</option>
	<option>Skavens</option>
</select>
<input type="button" name="valider_race" value="Go" onClick="choix_race();" class="bouton">
<br>
<br>
<table width="700" border="0"><tr>
<td width="72" ><div align="center" class="texte" style="background-color:#FFFFFF" name="img_race" id="img_race" ><img name="desc_emb_race"  id="desc_emb_race" src="emblems/no_emblem.gif" width="70" height="93" border="1" bordercolor="#DDDDDD"></div></td>
<td width="628"><div align="center" class="texte" style="background-color:#FFFFFF" name="texte_race" id="texte_race" >---</div></td></tr></table>

<? } else { 
	
include("teams/".$r.".inc.php");
include("races.inc.php");
?>

<input type="hidden" name="x_rel" value="<?= $x_rel ?>">
<input type="hidden" name="r" value="<?= $r ?>">
<input type="hidden" name="editable" value="<?= $editable?>">
  <table border="1" cellspacing="0" cellpadding="1" bgcolor="#FFFFEE" bordercolor="#BBBBBB" width="800">
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

			$var_n = stripslashes($var_n);

			if( $j==$editable ) {
	?>
		<tr>
		<td align="center" valign="middle" onClick="editer('0');" style="cursor: hand"><font class="titre" color=""><b><?= $j ?></b></font></td>
		<td align="center" valign="middle" colspan="2"> 
        <input type="text" name="n<?= $j ?>" size="25" class="texte" value="<?= $var_n ?>">
      </td>
      <td   align="right" valign="middle" > 
        <select name="p<?= $j ?>" class="liste" onChange="if(this.selectedIndex!=<?= sizeof($position) ?>) set_default_value(this,'<?= $j ?>');else reset_value('<?= $j ?>'); calculer_total_equipe();">	
		<?
			$selected = false;
			while (list ($key, $value) = each ($position)) { 
				echo "<option ".(($value===$var_p)?"selected":"").">$value</option>";
			}
		?> 
		<option <?= ($var_p==="---")?"selected":"" ?>>---</option>
        </select>
      </td>
	  <td align="right" valign="middle" > 
        <select name="ma<?= $j ?>" class="liste">
          <? for($i=0;$i<=10;$i++) { echo "<option ".(($i==$var_ma)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
		<td align="right" valign="middle" > 
        <select name="st<?= $j ?>" class="liste">
          <? for($i=0;$i<=10;$i++) { echo "<option ".(($i==$var_st)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td align="right" valign="middle" > 
        <select name="ag<?= $j ?>" class="liste">
          <? for($i=0;$i<=10;$i++) { echo "<option ".(($i==$var_ag)?"selected":"").">$i</option>"; } ?>
		</select>
      </td>
      <td align="right" valign="middle" > 
        <select name="av<?= $j ?>" class="liste">
          <? for($i=0;$i<=10;$i++) { echo "<option ".(($i==$var_av)?"selected":"").">$i</option>"; } ?>
		</select>
      </td>
		<td align="center" valign="middle" nowrap> 
        <input type="text" name="c_lg<?= $j ?>" size="40" class="texte" value="" onfocus="this.blur();"><script language='Javascript'>document.feuille.c_lg<?= $j ?>.value = translate('<?= $var_c ?>');</script><input type="hidden" name="c<?= $j ?>" value="<?= $var_c ?>"><input type="button" name="add_skill" value="+" class="bouton" onClick="add_skill_popup('<?= $j ?>','<?= $r ?>',	document.feuille.p<?= $j ?>.selectedIndex,document.feuille.c<?= $j ?>.value)">
		</td>
      <td align="right" valign="middle" > 
        <select name="bl<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
          <? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_bl)?"selected":"").">$i</option>"; } ?>
          <option>50+</option>
        </select>
      </td>
      <td align="right" valign="middle" > 
        <select name="reu<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
		<? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_reu)?"selected":"").">$i</option>"; } ?>
          <option>50+</option>
		</select>
      </td>
      <td align="right" valign="middle" > 
        <select name="td<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
		<? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_td)?"selected":"").">$i</option>"; } ?>
          <option>50+</option>
		</select>
      </td>
	  <td align="right" valign="middle" > 
        <select name="int<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
			<? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_int)?"selected":"").">$i</option>"; } ?>
		  <option>50+</option>
		</select>
	  </td>		  
		  <td   align="right" valign="middle" > 
        <select name="sor<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
		  <? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_sor)?"selected":"").">$i</option>"; } ?>
          <option>50+</option>
		</select>
      </td>
      <td   align="right" valign="middle" > 
        <select name="jpv<?= $j ?>" class="liste" onChange="calculer_xps(<?= $j ?>);">
		<? for($i=0;$i<50;$i++) { echo "<option ".(($i==$var_jpv)?"selected":"").">$i</option>"; } ?>
		<option>50+</option>
        </select>
      </td>
      <td align="right" valign="middle" >
		<input type="text" name="xp<?= $j ?>" size="8" class="texte" onfocus="this.blur();" value="<?= $var_xp ?>">
      </td>
      <td align="right" valign="middle"> 
        <input type="text" name="cout<?= $j ?>" size="8" class="texte" onfocus="this.blur();" value="<?= $var_cout ?>">
      </td>
	  </tr>
<? } // fin de la ligne editable 
else { 
?>
	<tr onClick="editer('<?= $j ?>');" style="cursor: hand" onMouseMove="this.style.backgroundColor='#F7F2D4'" onMouseOut="this.style.backgroundColor='#FFFFEE'">
	  <input type="hidden" name="n<?= $j ?>" value="<?= $var_n ?>">
	  <input type="hidden" name="p<?= $j ?>" value="<?= $var_p ?>">
	  <input type="hidden" name="ma<?= $j ?>" value="<?= $var_ma ?>">
	  <input type="hidden" name="st<?= $j ?>" value="<?= $var_st ?>">
	  <input type="hidden" name="ag<?= $j ?>" value="<?= $var_ag ?>">
	  <input type="hidden" name="av<?= $j ?>" value="<?= $var_av ?>">
	  <input type="hidden" name="c<?= $j ?>" value="<?= $var_c ?>">
	  <input type="hidden" name="bl<?= $j ?>" value="<?= $var_bl ?>">
	  <input type="hidden" name="reu<?= $j ?>" value="<?= $var_reu ?>">
	  <input type="hidden" name="td<?= $j ?>" value="<?= $var_td ?>">
	  <input type="hidden" name="int<?= $j ?>" value="<?= $var_int ?>">
	  <input type="hidden" name="sor<?= $j ?>" value="<?= $var_sor ?>">
	  <input type="hidden" name="jpv<?= $j ?>" value="<?= $var_jpv ?>">
	  <input type="hidden" name="xp<?= $j ?>" value="<?= $var_xp ?>">
	  <input type="hidden" name="cout<?= $j ?>" value="<?= $var_cout ?>">

	  <td align="center" valign="middle"><font class="titre" color=""><b><?= $j ?></b></font></td>
	  <td colspan="2" align="left" valign="middle"><font class="titre"><?= $var_n?$var_n:"&nbsp;" ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_p ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_ma ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_st ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_ag ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_av ?></font></td>
	  <td align="left" valign="middle"><font class="titre"><script language='Javascript'>document.write(translate('<?= $var_c ?>'));</script>&nbsp;</font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_bl ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_reu ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_td ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_int ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_sor ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_jpv ?></font></td>
	  <td align="center" valign="middle"><font class="titre"><?= $var_xp ?></font></td>
	  <td align="right" valign="middle"><font class="titre"><?= $var_cout ?></font></td>
	  </tr>
	  <?
	}
}

	?>
    <tr> 
      <td colspan="2" rowspan="6" align="center" >
<?	
		if($race_tbt[$r]=='') 
			$filename="no_emblem.gif";
		else 
			$filename=$race_tbt[$r].".jpg";
	
		if( $emb ) $filename=$emb;
?>	
		<img name="emb_img" src="emblems/<?= $filename ?>" width="70" height="93" border="1" bordercolor="#DDDDDD" onClick="popup_embleme('<?= $filename ?>');" style="cursor: hand"><br><font class="titre" id="emb_name"><?= $filename ?></font><br>
	  <input type="hidden" name="emb" value="<?=$filename?>">
	  <br><input type="button" value="Changer" name="btn_changer_image" class="bouton" onClick="popup_embleme('<?= $filename ?>');">
	  </td>
      <td colspan="3" valign="middle" align="right"><font class="titre">Coach&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" name="coach"  class="texte" size="20" value="<?= $coach ?>">
      </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Relance&nbsp;</font></td>
      <td   align="right" valign="middle" > 
        <select name="rel" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=8;$i++) { echo "<option ".(($i==$rel)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td   valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre"><?= $x_rel?> PO&nbsp;</font></td>
      <td align="right" valign="middle"> 
        <input type="text" name="cout_rel" size="8" class="texte" onfocus="this.blur();" value="<?= $cout_rel ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Race&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $r ?></font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Popularit&eacute;&nbsp;</font></td>
      <td align="right" valign="middle"> 
        <select name="pop" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=9;$i++) { echo "<option ".(($i==$pop)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle"> 
        <input type="text" name="cout_pop" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_pop ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Classement&nbsp;</font></td>
      <td colspan="4"> 
		<input type="text" name="cla" size="8" class="texte" onfocus="this.blur();" value="<?= $cla ?>">
  	  </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Assistants&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <select name="ass" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=8;$i++) { echo "<option ".(($i==$ass)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle"> 
        <input type="text" name="cout_ass" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_ass ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Tr&eacute;sor&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" name="tre" class="texte" size="8"  value="<?= $tre ?>">
      </td>
      <td colspan="3" align="right" valign="middle" nowrap><font class="titre">Pom-pom girls&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <select name="ppg" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=8;$i++) { echo "<option ".(($i==$ppg)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type="text" name="cout_ppg" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_ppg ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Nom de 
        l'&eacute;quipe&nbsp;</font></td>
      <td colspan="4"> 
        <input type="text" name="nom_eq" class="texte" size="35"  value="<?= $nom_eq ?>">
      </td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Apothiquaire&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <select name="apo" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=1;$i++) { echo "<option ".(($i==$apo)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td   valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">50000 PO&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type="text" name="cout_apo" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_apo ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="7" valign="middle" align="center"><input type="hidden" name="dl_xml" value="0"><input type="button" name="voir_btn" value="Voir xml" class="bouton" onClick="document.feuille.dl_xml.value=0;generer_xml();">&nbsp;<input type="button" name="xml_btn" value="Downloader xml" class="bouton" onClick="document.feuille.dl_xml.value=1;generer_xml();">&nbsp;<input type="button" name="html_btn" value="Version imprimable" class="bouton" onClick="version_imprimable();"></td>
      <td colspan="7" valign="middle" align="right"><font class="titre"><input type="button" name="btn_calc_total" value="Calculer" onClick="calculer_total_equipe();" class="bouton">&nbsp;&nbsp;&nbsp;<b>Co&ucirc;t 
        total de l'&eacute;quipe</b></font></td>
      <td align="right" valign="middle" > 
        <input type="text" name="cout_eq" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_eq ?>">
      </td>

	  <!--
	  <td colspan="3" align="right" valign="middle"><font class="titre">Sorcier&nbsp;</font></td>
      <td   align="right" valign="middle"> 
        <select name="sorc" class="liste" onChange="calculer_total_equipe();">
		<? for($i=0;$i<=1;$i++) { echo "<option ".(($i==$sorc)?"selected":"").">$i</option>"; } ?>
        </select>
      </td>
      <td   valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">150000 PO&nbsp;</font></td>
      <td align="right" valign="middle" > 
        <input type="text" name="cout_sorc" size="8" class="texte"  onfocus="this.blur();" value="<?= $cout_sorc ?>">
      </td>
	  -->
    </tr>
  </table>

<script language="javascript">calculer_total_equipe();</script>

<? } ?>


<? 
	if (stristr($_SERVER['HTTP_USER_AGENT'],"MSIE") && !stristr($_SERVER['HTTP_USER_AGENT'],"Opera")) { 
?>
<br>
<br>
<pre>A tout moment, vous pouvez aussi configurer 
vos formations...</pre>
<input type="button" name="edit_formation_btnl" value="Editeur de formation" onClick="open_editeur_formation();" class="bouton">
<?
	}
else {
?>
<br>
<br>
<pre>A tout moment, vous pouvez aussi <a href="formations.html" target="_blank">configurer 
vos formations</a>...</pre>

<?
}
?>

</form>
</center>
<BR>

<A HREF="../index.html">RETOUR</A>
<script language='javascript' src='../footer.js'></script>
</BODY>
</HTML>

