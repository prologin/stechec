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
<TITLE>Team Editor for Tow Bowl Tactics - Printable Version</TITLE>
<META NAME="Author" CONTENT="toweld@wanadoo.fr">
<META NAME="Keywords" CONTENT="toweld, blood bowl, tow bowl tactics, tbt, game, sdl">
<META NAME="Description" CONTENT="Tow Bowl - Tactics - Homepage">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<LINK rel="stylesheet" href="printable_roster.css" type="text/css">

<script language="Javascript" src="description_race.js"></script> 
<script language="Javascript" src="skills_en_to_fr.js"></script> 
<script language="Javascript" src="races.js"></script> 
<script language="Javascript" src="roster.js"></script> 

<script language="Javascript">

function rotate() {
	if(rot.style.filter=='progid:DXImageTransform.Microsoft.BasicImage(rotation=1)'){
		rot.style.filter='';
	}
	else {
		rot.style.filter='progid:DXImageTransform.Microsoft.BasicImage(rotation=1)';
	}
}

window.onbeforeprint =
  function () {
    input_td.innerHTML = "&nbsp;";
  };

window.onafterprint = 
  function () {
	input_td.innerHTML = '<input  type="button" name="Imprimer_btn" value="Imprimer la feuille" class="bouton" onClick="imprimer();">&nbsp;<input  type="button" name="close_btn" value="Close Window" class="bouton" onClick="window.close();">';
  };


function imprimer(){	
	if(window.print) window.print();
}

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
</script>

</HEAD>

<BODY topmargin="5" leftmargin="5" marginwidth="0" marginheight="0" name="rot" id="rot">

<center>

<?	
include("teams/".$r.".inc.php");
include("races.inc.php");
?>

<input type="hidden" name="x_rel" value="<?= $x_rel ?>">
<input type="hidden" name="r" value="<?= $r ?>">
<input type="hidden" name="editable" value="<?= $editable?>">
  <table align="center" valign="middle" border="1" cellspacing="0" cellpadding="1" bgcolor="#FFFFFF" bordercolor="#BBBBBB" width="800">
    <tr align="center" valign="middle"> 
      <td width="10">&nbsp; </td>
      <td colspan="2" ><font class="titre"><b>Player's Name</b></font></td>
      <td><font class="titre"><b>Position</b></font></td>
      <td><font class="titre"><b>MA</b></font></td>
      <td><font class="titre"><b>ST</b></font></td>
      <td><font class="titre"><b>AG</b></font></td>
      <td><font class="titre"><b>AV</b></font></td>
      <td><font class="titre"><b>Skills/Traits</b></font></td>
      <td><font class="titre"><b>INJ</b></font></td>
      <td><font class="titre"><b>COM</b></font></td>
      <td><font class="titre"><b>TD</b></font></td>
      <td><font class="titre"><b>INT</b></font></td>
      <td><font class="titre"><b>CAS</b></font></td>
      <td><font class="titre"><b>MVP</b></font></td>
      <td><font class="titre"><b>EXP</b></font></td>
      <td><font class="titre"><b>Cost</b></font></td>
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
?>
	<tr>

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

	?>
    <tr> 
      <td colspan="2" rowspan="6" align="center" valign="middle">
<?	
		if($race_tbt[$r]=='') 
			$filename="no_emblem.gif";
		else 
			$filename=$race_tbt[$r].".jpg";
	
		if( $emb ) $filename=$emb;
?>	
		<img name="emb_img" src="../emblems/<?= $filename ?>" width="70" height="93" border="1" bordercolor="#DDDDDD">
	  <br><font class="titre" id="emb_name"><?= $filename ?></font>
	  <input type="hidden" name="emb" value="<?=$filename?>">
	  </td>
      <td colspan="3" valign="middle" align="right"><font class="titre">Coach&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $coach ?>&nbsp;</font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Re-Rolls&nbsp;</font></td>
      <td align="center" valign="middle"><font class="titre"><?= $rel ?></font></td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre"><?= $x_rel?> PO&nbsp;</font></td>
      <td align="right" valign="middle"><font class="titre"><?= $cout_rel ?></font></td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Race&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $r ?></font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Fan Factor&nbsp;</font></td>
      <td align="center" valign="middle"><font class="titre"><?= $pop ?></font></td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle"><font class="titre"><?= $cout_pop ?></font></td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Rank&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $cla ?></font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Assistants&nbsp;</font></td>
      <td align="center" valign="middle"><font class="titre"><?= $ass ?></font></td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle"><font class="titre"><?= $cout_ass ?></font></td>
    </tr>
    <tr> 
      <td colspan="3" align="right" valign="middle"><font class="titre">Treasury&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $tre ?>&nbsp;</font></td>
      <td colspan="3" align="right" valign="middle" nowrap><font class="titre">Cheerleaders&nbsp;</font></td>
      <td align="center" valign="middle"><font class="titre"><?= $ppg ?></font></td>
      <td valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">10000 PO&nbsp;</font></td>
      <td align="right" valign="middle"><font class="titre"><?= $cout_ppg ?></font></td>
    </tr>
    <tr> 
      <td colspan="3" valign="middle" align="right"><font class="titre">Team Name&nbsp;</font></td>
      <td colspan="4"><font class="titre"><?= $nom_eq ?>&nbsp;</font></td>
      <td colspan="3" align="right" valign="middle"><font class="titre">Apothecaries&nbsp;</font></td>
      <td align="center" valign="middle" ><font class="titre"><?= $apo ?></font></td>
      <td   valign="middle" align="center"><font class="titre">X</font></td>
      <td colspan="2" nowrap align="right"><font class="titre">50000 PO&nbsp;</font></td>
      <td align="right" valign="middle" ><font class="titre"><?= $cout_apo ?></font></td>
    </tr>
    <tr> 
      <td colspan="7" valign="middle" align="center" name="input_td" id="input_td"><input  type="button" name="Imprimer_btn" value="Print sheet" class="bouton" onClick="imprimer();">&nbsp;<input  type="button" name="close_btn" value="Close Window" class="bouton" onClick="window.close();"></td>
      <td colspan="7" valign="middle" align="right"><font class="titre">&nbsp;<b>Total 
        cost of Team&nbsp;</b></font></td>
	  <td align="right" valign="middle"><font class="titre"><?= $cout_eq ?></font></td>
    </tr>
  </table>

</center>
</BODY>
</HTML>

