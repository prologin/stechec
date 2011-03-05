<?php

include("races.inc.php");

$filename=$emb;

if($todo == "preview"){
	if ($embleme_name) {	
		copy($embleme,"../emblems/".$embleme_name);
		$filename=$embleme_name;
	}
}

?>

<html>
<head>
<title>Tow Bowl Tactics - Team Editor</title>
<LINK rel="stylesheet" href="roster.css" type="text/css">
<script language="Javascript">
function preview(){
	document.emblem_form.todo.value = "preview";
	document.emblem_form.submit('');
}

function retour(){
	window.opener.changer_embleme('<? echo $filename ?>');
	window.close();
}
</script>
</head>
<body topmargin="5" leftmargin="5" marginwidth="0" marginheight="0">
<center>
<form name="emblem_form" method="post" enctype="multipart/form-data" action="emblem_loader.php3">
<input type="hidden" name="todo" value="">

<img src="../emblems/<? echo $filename?>" width="210" height="280" BORDER="1" bordercolor="#000000" ALT="Team embleme">
<br>
<? if ($todo=="preview") { ?>
	<font face="Arial,helvetica,sans-serif" size="2"><b><? echo  $embleme_name ?></b><br>(<? echo  $embleme_size ?> 
	bytes, <? echo $embleme_type ?>).</font><br>

	<input type="button" name="close_btn" class="bouton" value="Return to Team Editor" onClick="retour();">

<? } else { ?>
<input type="hidden" name="MAX_FILE_SIZE" value="1000000">
<input type="file" name="embleme" class="texte" size="25"><br>
<input type="button" name="preview_btn" class="bouton" value="View the new embleme" onClick="preview();">
<? } ?>

</center>
</form>

</body>
</html>
