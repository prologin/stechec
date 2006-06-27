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


define(BANNER_PATH, "/home/local/intradmin/www/banner/");

function get_random_banner()
{
  global $db_link;

  $res = db_query("SELECT name,link,path,(RAND()*weight) AS bobby ".
		  "FROM banniere ORDER BY bobby DESC LIMIT 1",
		  $db_link);
  
  return (db_fetch_array($res));
}




function show_banner()
{
  $banner = get_random_banner();

  $realpath = $banner[path];
  $url = $banner["link"];
  $name = $banner["name"];

  print "<A HREF=$url><IMG class=banner border=\"0\" TITLE=\"$name\" width=\"468\" height=\"60\" SRC=$realpath></A>";
}




function banner_template()
{
  $list = array(
		array("", "Banniere", "title", ACL_ALL, 0),
		array("path",	"Image", "image", ACL_ALL, 0),
		array("name",	"Nom",	 "text", ACL_ALL, ACL_SITEADM),
		array("link",	"URL",	 "text", ACL_ALL, ACL_SITEADM),
		array("weight",	"Poids", "text", ACL_ALL, ACL_SITEADM)
		);

  return ($list);
}


function banner_disp_all()
{
  $list = banner_template();
  
  $query = table_select("banniere", $list, "1");
  array_push($list, array("path", "Uploader", "upload", ACL_ALL, ACL_SITEADM));
  array_push($list, array("Supprimer la banniere", "Suppression", "link",
			  ACL_ALL, 0, array("_action" => "del_banner",
					    "_id_page" => 7),
			  "Voulez vous supprimer cette banniere ?")
	     );

  table_disp($query, $list, "edit_banner", "_entry");
}



function banner_action()
{
  global $db_link;
  global $_action;
  global $_entry;


  switch ($_action)
    {
    case ("edit_banner"):
      $res = db_query("SELECT id FROM banniere ".
		      "WHERE id=".$_entry,
		      $db_link);
      
      if (db_num_rows($res) != 1)
	die("Error: update_banner()");

      $list = banner_template();
 
      if ($_FILES['_form_path']['name'])
	{
	  print "<pre>";
	  
	  $uploaddir = BANNER_PATH;
	  $uploadfile = $uploaddir. $_FILES['_form_path']['name'];
	  
	  if (move_uploaded_file($_FILES['_form_path']['tmp_name'], $uploadfile))
	    {
	      print "File is valid, and was successfully uploaded. ";
	    }
	  else 
	    {
	      print "Upload failed:\n";
	      print_r($_FILES);
	      return (1);
	    }
	  print "</pre>";
	  array_push($list, array("path", "path", "text", ACL_ALL, ACL_SITEADM));
	  $_POST[_form_.path] = "banner/" . $_FILES['_form_path']['name'];
	}

      if (!table_update("banniere", $list, "id=$_entry"))
	print ("Erreur de mise a jour des donnees");

      $_action = "";
      break;


    case ("add_banner"):
      table_insert("banniere", array(), array("name" => "'nouvelle banniere'",
					      "path" => "''",
					      "link" => "'http://'",
					      "weight" => "0"));
      break;

    case ("del_banner"):
      db_query("DELETE FROM banniere WHERE id=".$_entry, $db_link);
      break;


    }



  return (0);
}


function banner_display_add()
{
  style_links(array(
		    array("Ajouter une banniere", ACL_ALL,
			  array("_id_page" => 7, "_action" => "add_banner"))
		    ));

  print "<br>";
  
}


?>




