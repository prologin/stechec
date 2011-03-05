<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<TITLE>Editeur de formations pour Tow Bowl Tactics</TITLE>
<META NAME="Author" CONTENT="toweld(at)free(dot)fr">
<META NAME="Keywords" CONTENT="toweld, blood bowl, tow bowl tactics, tbt, game, sdl">
<META NAME="Description" CONTENT="Tow Bowl - Tactics - Homepage">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<LINK rel="stylesheet" href="roster.css" type="text/css">

<?
if(!$formation_id) $formation_id=1;
?>


<script language="Javascript" src="edit_formation.js" type="text/javascript"></script>

</HEAD>

<BODY onLoad='preloadimages();' topmargin="0" leftmargin="0" marginwidth="5" marginheight="5">
<center>

<form name="editeur_formation">
<input type="hidden" name="dl_xml" id="dl_xml" value="0">

<table bgcolor="#FFFFFF">
<tr><td align="left" valign="top">

<TABLE border="1" cellspacing="0" cellpadding="0" bordercolor="#EEEEEE" bgcolor="#FFFFEE" id="table_membre" height="390">
<TR height="20">
	<TD colspan="3" align="center"><font class="petit_titre"><b>Membres de l'équipe</b></font></TD>
</TR>
<?	
	for($j=1;$j<=16 ;$j++) { 
		$tmp = "n".$j; 
		$var_n = stripslashes($$tmp);
		$tmp = "p".$j; 
		$var_p = $$tmp;
?>
<TR height="10" name="<?= $j ?>" id="<?= $j ?>" onClick="toggle_selected(<?= $j ?>,this);" width="210" onMouseOver="if(player_placed[current_formation][<?= $j ?>]==1) this.style.cursor=''; else this.style.cursor='pointer';" >
	<TD bgcolor="#FFFFFF" name="td_joueur<?= $j ?>" id="td_joueur<?= $j ?>"  width="5%" align="center"><font class="petit_titre"><?= $j ?></font></TD>
	<TD width="60%" align="left" nowrap><font class="petit_texte"><?= ($var_n=="")?"---":$var_n ?></font></TD>
	<TD width="35%" align="left" nowrap><font class="petit_texte"><?= ($var_p=="")?"---":$var_p ?></font></TD>
</TR>
<? 
	} 
?>
<TR>
<TD colspan="3">
<table width="100%">
<tr align="center">
	<td align="right" width="50%"><font class="petit_texte">ID Formation&nbsp;</font></td>
	<td align="left" width="50%"><select name="formation_id" class="liste" onChange="change_formation(this.selectedIndex);"><? for($i=1;$i<=4;$i++) { echo "<option  ".(($formation_id==$i)?"selected":"").">".$i."</option>"; } ?></select></td>
</tr>
<tr align="center">
	<td colspan="2" align="center" >
	<input type="button" name="check_formation_btn" value="Vérifier cette formation" class="bouton" onClick="check_formation();">
	</td>
</tr>
<tr align="center">
	<td colspan="2" align="center"><input type="button" name="voir_btn" value="Voir xml" class="bouton" onClick="document.getElementById('dl_xml').value='0';generer_xml();"></td>
</tr>
<tr align="center">
	<td colspan="2" align="center"><input type="button" name="xml_btn" value="Downloader xml" class="bouton" onClick="document.getElementById('dl_xml').value='1';generer_xml();"></td>
</tr>
<tr align=center>
	<td colspan=2 align=center height=25 valign=middle><div align=center class="texte" id='div_a_placer'>&nbsp;</div></td>
</tr>
</table>




</TR>
</TABLE>
</td>

<td width="1"></td>

<td valign="top">
<TABLE id="table_terrain" border="0" cellspacing="0" cellpadding="0" bordercolor="#EEEEEE" bgcolor="#FFFFFF" background="terrain_grid30.jpg" width="450" height="390" style='cursor:pointer'>

<? for($row=0;$row<13;$row++) { ?>
	<TR height="30">
	<? for($col=0;$col<15;$col++) { ?>
		<TD width="30" align="center" valign="middle" onClick="change_bg(this,<?= $col ?>,<?= $row ?>);" ><table width="99%" height="99%"><tr><td onMouseOut="m_out(this,event)" onMouseOver="m_over(this,event)" align="center" valign="middle" id='td_case<?= $col ?>_<?= $row ?>' name='td_case<?= $col ?>_<?= $row ?>'></td></tr></table></TD>
	<? } ?>
	</TR>
<? } ?>

</TABLE>
</td>
</tr>
</table>
<font class="texte">NB: Le fichier xml ainsi crée doit être placé dans le répertoire <b>data/formations/</b> et<br>vous devez positionner correctement la variable <b>&lt;formation&gt;</b> dans votre fichier d'équipe.<br>Ex: <i><b>&lt;formation&gt;</b>mes_formations.xml<b>&lt;/formation&gt;</b></i>
</font>

</form>
</center>

<script language="javascript" type="text/javascript">change_formation(0);</script>
</BODY>
</HTML>
