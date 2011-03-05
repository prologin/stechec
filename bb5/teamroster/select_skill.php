<!-- select_skill.php?r='+race+'&n='+numero+'&p='+position+'&c='+liste_comp-->

<? 
include("trad_skills.inc.php"); 
include("races.inc.php"); 
include("teams/".$r.".inc.php");
?>

<html>
<head>
<title>Tow Bowl Tactics - Editeur d'équipe</title>
<LINK rel="stylesheet" href="roster.css" type="text/css">
<script language="Javascript" src="skills_traits.js"></script>
<script language="Javascript" src="skills_en_to_fr.js"></script>
<script language="Javascript">

function ajouter() {
	destList = document.skill_form.avant;
	srcList = document.skill_form.apres; 
	var len = destList.length;
	for(var i = 0; i < srcList.length; i++) {
		if ((srcList.options[i] != null) && (srcList.options[i].selected)) {
			var found = false;
			for(var count = 0; count < len; count++) {
				if (destList.options[count] != null) {
					if (srcList.options[i].text == destList.options[count].text) {
						found = true;
						break;
					}
				}
			}

			if (found != true) {
				destList.options[len] = new Option(srcList.options[i].text,srcList.options[i].value ); 
				srcList.options[i] = null;
				len++;
			}
		}
   }
}

function retirer() {
	destList = document.skill_form.apres;
	srcList = document.skill_form.avant; 
	var len = destList.length;
	for(var i = 0; i < srcList.length; i++) {
		if ((srcList.options[i] != null) && (srcList.options[i].selected)) {
			var found = false;
			for(var count = 0; count < len; count++) {
				if (destList.options[count] != null) {
					if (srcList.options[i].text == destList.options[count].text) {
						found = true;
						break;
					}
				}
			}

			if (found != true) {
				destList.options[len] = new Option(srcList.options[i].text,srcList.options[i].value ); 
				srcList.options[i] = null;
				len++;
			}
			else srcList.options[i] = null;
		}
   }
}

function save_and_close(){
	window.opener.set_skills(document.skill_form.avant,'<?= $n ?>');
	window.close();
}
	
</script>
</head>
<body topmargin="2" leftmargin="2" marginwidth="0" marginheight="0">
<center>
<form name="skill_form">
<TABLE border="1" bgcolor="#FFFFEE" bordercolor="#BBBBBB" cellspacing="0" cellpadding="1" width="100%" height="100%">
<TR>
	<TD align="center" width="140">
	<font size="1" face="Arial, helvetica, sans-serif">Actuellement</font><br>
	<SELECT size="8" NAME="avant" class="zone">
	<?
	if($c){
		$t = split(",",$c);
		for($i=0;$i<count($t);$i++){
			echo '<option value="'.$t[$i].'">'.$skills_en_to_fr[$t[$i]];
		}
	}
	?>
	</SELECT>
	</TD>
	<TD align="center" width="20">
	<input type="button" name="ajout_btn" value="<<" onClick="ajouter();" class="bouton"><br><br>
	<input type="button" name="retirer_btn" value=">>" onClick="retirer();" class="bouton"></TD>
	<TD align="center" width="140">
	<font size="1" face="Arial, helvetica, sans-serif">Possibilité&nbsp;<script language="Javascript">document.write('('+skills[<?= $p ?>]+')');</script></font><br>
	<SELECT size="8" NAME="apres" class="zone">
	<script language="Javascript">
		var cat = skills[<?= $p ?>].split(",");
			
		for (var j=0; j<cat.length; j++){
				
			var c = cat[j];
			if(skill_list[c]) {
				for(var h=0;h<skill_list[c].length;h++){
					if(skills_en_to_fr[skill_list[c][h]]==undefined) alert(skill_list[c][h] + " " + skills_en_to_fr[skill_list[c][h]]);
					document.write('<option value="'+skill_list[c][h]+'">'+skills_en_to_fr[skill_list[c][h]]);	
				}
			}

			if(trait_list[c]){
				for(var h=0;h<trait_list[c].length;h++){
					if(skills_en_to_fr[trait_list[c][h]]==undefined) alert(trait_list[c][h] + " " + skills_en_to_fr[trait_list[c][h]]);
					document.write('<option value="'+trait_list[c][h]+'">'+skills_en_to_fr[trait_list[c][h]]);	
				}	
			}
		}			
	</script>
	</SELECT>
	</TD>
</TR>
<TR>
	<TD align="center" colspan="3"><input type="button" name="close_btn" value="Valider" onClick="save_and_close();" class="bouton"></TD>
</TR>
</TABLE>
</form>
</center>
</body>
</html>