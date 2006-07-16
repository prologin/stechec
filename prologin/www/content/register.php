<?php
/*

    ACU Intranet project is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ACU Intranet project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ACU Intranet project; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Copyright ACU EPITA Promo 2004

	* Becoulet Alexandre <becoul_a@epita.fr>
	* Bizon Maxime <bizon_m@epita.fr>

*/
?>
  <meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1" />
  <link rel="shortcut icon" type="image/png" href="/favicon.png" />
  <meta name='keywords' content='EPITA, ACU, YAKA, assistants C unix'>
</head>

<body marginwidth=0 marginheight=0 topmargin=0 rightmargin=0 leftmargin=0 bottommargin=0
 onload='javascript:document.logform._login.focus();'>

<table cellpadding=0 cellspacing=0 width=100% height=100% border=0 bgcolor='#505060'><tr><td>
<form method='post' action='index.php' name='logform'>

<table class=login_table align=center valign=top cols=3>

  <tr height=130><td colspan=3>&nbsp</td></tr>
  <tr height=50><td width=150 ></td><td colspan=2>Les informations que vous saisissez, doivent etre correctes pour que l'association
puisse vous envoyer un lot si vous faites parti des meilleurs de ce defi.</td></tr>
  <tr height=20><td colspan=3>&nbsp</td></tr>
  <tr height=30>
     <td width=300>&nbsp</td>
     <td valign=middle align=left>
	Nom
     </td>
     <td valign=middle align=left>
       <input type='text' name='_last' maxlength=20>
     </td>
  </tr>
  <tr height=30>
	<td width=300>&nbsp</td>
     <td valign=middle align=left>
	Prenom
     </td>
     <td valign=middle align=left>
      <input type='text' name='_first' maxlength=50>
     </td>
 </tr>
  <tr height=30>
	<td width=300>&nbsp</td>
     <td valign=middle align=left>
	E-mail
     </td>
     <td valign=middle align=left>
      <input type='text' name='_mail' maxlength=50>
     </td>
 </tr>
  <tr height=30>
	<td width=300>&nbsp</td>
     <td valign=middle align=left>
	Login
     </td>
     <td valign=middle align=left>
      <input type='Login' name='_login' maxlength=50>
     </td>
 </tr>
  <tr height=30>
	<td width=300>&nbsp</td>
     <td valign=middle align=left>
	Password
     </td>
     <td valign=middle align=left>
      <input type='password' name='_pass' maxlength=50>
     </td>
 </tr>

  <tr height=20><td>&nbsp</td></tr>

  <tr height=30>
    <td >&nbsp</td>

    <td valign=middle align=right width=200>
      <input type='submit' name="register" value="Register me !">
    </td>
    </tr>

  <tr>
    <td colspan=3 align=center valign=middle>

<?php
print "<div class=login_cell0> $_login_message</div>";
?>

    </td>
  </tr>

</table>
</form>
</td></tr></table>
