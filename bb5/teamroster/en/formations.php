<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<TITLE>Editeur de formation</TITLE>
<META NAME="Generator" CONTENT="EditPlus">
<META NAME="Author" CONTENT="toweld@rocketmail.com">
<META NAME="Keywords" CONTENT="tbt,tow,bowl,tactics,editeur,formation,xml">
<META NAME="Description" CONTENT="Editeur de formation pour Tow Bowl Tactics">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 

</HEAD>

<BODY>
<form method="post" action="export_xml_formation.php">
<font face="Arial,helvetica,sans-serif" size="2">Formation #1</font>
<TABLE border="1">
<? for($y=0;$y<13;$y++) { ?>
<TR>
	<? for($x=0;$x<15;$x++) { ?>
<TD><input size="1" type="text" name="a<?=$x?>_<?=$y?>"></TD>
	<? } ?>
</TR>
<? } ?>
</TABLE>
<br>
<font face="Arial,helvetica,sans-serif" size="2">Formation #2</font>
<TABLE border="1">
<? for($y=0;$y<13;$y++) { ?>
<TR>
	<? for($x=0;$x<15;$x++) { ?>
<TD><input size="1" type="text" name="b<?=$x?>_<?=$y?>"></TD>
	<? } ?>
</TR>
<? } ?>
</TABLE>
<br>
<font face="Arial,helvetica,sans-serif" size="2">Formation #3</font>
<TABLE border="1">
<? for($y=0;$y<13;$y++) { ?>
<TR>
	<? for($x=0;$x<15;$x++) { ?>
<TD><input size="1" type="text" name="c<?=$x?>_<?=$y?>"></TD>
	<? } ?>
</TR>
<? } ?>
</TABLE>
<br>
<font face="Arial,helvetica,sans-serif" size="2">Formation #4</font>
<TABLE border="1">
<? for($y=0;$y<13;$y++) { ?>
<TR>
	<? for($x=0;$x<15;$x++) { ?>
<TD><input size="1" type="text" name="d<?=$x?>_<?=$y?>"></TD>
	<? } ?>
</TR>
<? } ?>
</TABLE>

<br>
<input type="submit" name="voir_btn" value="View xml">

</form>

</BODY>
</HTML>
