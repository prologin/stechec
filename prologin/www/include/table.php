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

/*
** Remplace dans la chaine $s_string toute sorte de retour a la ligne
** par une balise BR
** Retourne la chaine modifiee
*/
function nl2br2($s_string)
{
  return ereg_replace("(\r\n|\n|\r)", "<br />", $s_string);
}

/*
** Remplace dans la chaine $s_string toutes les occurences
** du caractere ' par le code HTML correspondant
** Retourne la chaine modifiee
*/
function encode_quote($s_string)
{
  return ereg_replace("'", "&#39;", $s_string);
}

/*
** Affiche l'entete du tableau HTML pour l'affichage de la liste
*/
function list_disp_head($list, &$cnt)
{
  print("<table class=table_list>\n");

  print("<tr>\n");
  $cnt = 0;
  for ($i = 0; $i < count($list); $i++)
    if (access_check($list[$i][3]))
      {
	$cnt++;
	print("<td class=table_list_title>\n");
	print($list[$i][1]);
	print("</td>\n");
      }
  print("</tr>\n");
}

/*
** Affiche une ligne de la liste sous forme de ligne de tableau
** L'affichage des elements differe en fonction du type demande
** Types:
** bool, image, photo, percent, datetime, date, text, dayperiod, log,
** login, link_info, link, extern_link
*/
function list_disp_line($data, $list, $style, $st_field, &$cache, $even)
{
  /* display a table for each row */
  $data["even"] = $even;
  print("<tr class=table_list_".$style[$data[$st_field] % count($style)].">\n");

  for ($j = $i = 0; $i < count($list); $i++)
    {
      /* skip if no read access right */
      if (access_check($list[$i][3]))
	{
	  if (is_array($list[$i][5])
	      && ($str = $list[$i][5][$data[$list[$i][0]] % count($list[$i][5])]) != "")
	    print("<td class=table_list_".$str.">\n");
	  else
	    print("<td class=table_list_".$style.$data[$st_field].">\n");

	  /* switch on field type */
	  switch ($list[$i][2])
	    {
	    case ("bool"):
	      print($list[$i][4][ $data[$list[$i][0]] ]);
	      break;

	    case ("image"):
	      print("<img class=table_info_image src='".$data[$list[$i][0]]."'>\n");
	      break;

	    case ("photo"):
	      print("<img class=table_info_photo src='".$data[$list[$i][0]]."'>\n");
	      break;

	    case ("percent"):
	      style_percent($data[$list[$i][0]], 100);
	      break;

	    case ("datetime"):
	    case ("date"):
	    case ("text"):
	      print($data[$list[$i][0]]);
	      break;

	    case ("dayperiod"):
	      print(date("z \\j\\o\\u\\r\\s H:i:s", $data[$list[$i][0]] - 3600));
	      break;

	    case ("log"):
	      print(nl2br2($data[$list[$i][0]]));
	      break;

	    case ("login"):
	      print("<a class=links href='mailto:".$data[$list[$i][0]].$list[$i][4]."'>".
		    $data[$list[$i][0]]."</a>");
	      break;

	    case ("link_info"):
	      $info_name = isset($list[$i][6]) ? $list[$i][6] : "_info_id";
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
		  $list[$i][4][$info_name] = $data["id"];
		  href($list[$i][4]);
		}
	      else
		href(array($info_name => $data["id"], "_id_page" => $list[$i][4]));
	      print("'>".$data[$list[$i][0]]."</a>");
	      break;

            // Type permettant de passer n'importe qu'elle valeur d'une base
            //    $6: array contenant les donnees a passer: ($nom_variable_passe,
            //        $nom_variable_in_bd)
            case ("link_generique_info"):
	      $tab_info = isset($list[$i][6]) ? $list[$i][6] : array();
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
                  foreach ($tab_info as $name_info => $name_field)
                  {
                    $list[$i][4][$name_info] = $data[$name_field];
                  }
		  href($list[$i][4]);
		}
	      else
                href(array());
	      print("'>".$data[$list[$i][0]]."</a>");
	      break;

	      // case added by delecr_n
	      // redirects page to the link
	    case ("link_to_follow"):

	      //	      print "<p>on va afficher le link...</p>\n";
	      $lnk = "";

	      if ($data[$list[$i][0]] == "")
		break;
	      $info_name = isset($list[$i][6]) ? $list[$i][6] : "_entry";
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
		  if (isset($list[$i][7]))
		    $list[$i][4][$list[$i][7]] = $data["id2"];

		  $list[$i][4][$info_name] = $data[$list[$i][0]];
		  href($list[$i][4]);
		  $lnk = href_var($list[$i][4]);
		}
	      else
		{
		  print($list[$i][4]);
		  $lnk = href_var($list[$i][4]);
		}
	      print("'>".$list[$i][1]."</a>\n");

	 	        print $lnk;

	      // go to the link
	 	      print " <script> window.location='" . $lnk . "';</script>";

	      break;


	    case ("link"):
	      if ($data[$list[$i][0]] == "")
		break;
	      $info_name = isset($list[$i][6]) ? $list[$i][6] : "_entry";
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
		  if (isset($list[$i][7]))
		    $list[$i][4][$list[$i][7]] = $data["id2"];

		  $list[$i][4][$info_name] = $data[$list[$i][0]];
		  href($list[$i][4]);
		}
	      else
		print($list[$i][4]);
	      print("'>".$list[$i][1]."</a>\n");
	      break;

	    case ("extern_link"):
	      if ($data[$list[$i][0]] == "")
		break;
	      print("<a class=links href='");
	      print($data[$list[$i][0]]);
	      print("'>".$list[$i][1]."</a>\n");
	      break;

	    default:
	      /* get data from libelle table */
	      if (!isset($cache[$list[$i][2]]))
		{
		  /* cache for futur use */
		  $res_b = db_query("SELECT id, libelle FROM ".$list[$i][2],
				    $db_query);
		  while ($array = db_fetch_array($res_b))
		    $cache[$list[$i][2]][$array["id"]] = $array["libelle"];
		}
	      print($cache[$list[$i][2]][$data[$list[$i][0]]]);
	    }
	  print("</td>\n");
	  $j++;
	}
    }
  print("</tr>\n");
}

/*
**
*/
function list_disp($query, $list, $style, $st_field)
{
  List_disp_head($list, $cnt);

  if (db_num_rows($query) > 0)
    {
      $cache = array();
      while ($data = db_fetch_array($query))
	list_disp_line($data, $list, $style, $st_field, $cache, $i++);
    }
  else
    {
      print("<tr><td align=center class=table_list_gray colspan=".$cnt.">");
      print("Aucune entree disponible</td></tr>\n");
    }

  print("</table>\n\n");
}


/*
** Affiche les donnees contenues dans la table en utilisant
** le template $list
*/
function list_disp_data($table, $list, $style, $st_field)
{
  list_disp_head($list, $cnt);

  if (count($table))
    {
      $cache = array();
      while ($data = array_shift($table))
	list_disp_line($data, $list, $style, $st_field, $cache, $i++);
    }
  else
    {
      print("<tr><td align=center class=table_list_gray colspan=".$cnt.">");
      print("Aucune entree disponible</td></tr>\n");
    }

  print("</table>\n\n");
}

/*
** Affiche les donnees contenues dans les result sets $querys
** en utilisant le template $list
*/
function list_disp_concat($querys, $list, $style, $st_field)
{
  list_disp_head($list[0], $cnt);

  $empty = 1;
  for ($i = 0; $i < count($querys); $i++)
    {
      $cache = array();
      if (db_num_rows($querys[$i]) > 0)
	{
	  while ($data = db_fetch_array($querys[$i]))
	    list_disp_line($data, $list[$i], $style, $st_field, $cache, $j++);
	  $empty = 0;
	}
    }

  if ($empty)
    {
      print("<tr><td align=center class=table_list_gray colspan=".$cnt.">");
      print("Aucune entree disponible</td></tr>\n");
    }

  print("</table>\n\n");
}

/*
** $list format
**
**   list (field name, field title, data_type, read access right, write access right)
**   list (link test , field title, "link"   , read access right, write access right, link postdata ())
**   list (link test , field title, "link"   , read access right, write access right, link text)
*/

function table_select($table, $list, $where)
{
  global $db_link;

  /* build field list for db query */
  for ($j = $i = 0; $i < count($list); $i++)
    {
      //      if (access_check($list[$i][3]))
	{
	  switch ($list[$i][2])
	    {
	    case ("now"):
	    case ("datetime"):
	      $fields .= (++$j > 1 ? ", " : "").
		"DATE_FORMAT(".$list[$i][0].", '%d/%m/%Y %H:%i:%s') AS ".$list[$i][0];
	      break;

	    case ("date"):
	      $fields .= (++$j > 1 ? ", " : "").
		"DATE_FORMAT(".$list[$i][0].", '%d/%m/%Y') AS ".$list[$i][0];
	      break;

	    case ("linkinfo"):
	    case ("link"):
	      break;

	    default:
	      $fields .= (++$j > 1 ? ", " : "").$list[$i][0];

	    case ("text_nodb"):
	    case ("title"):
	    }
	}
    }

  //print("SELECT id, $fields FROM $table WHERE $where");
  /* exec db query */
  return (db_query("SELECT id, $fields FROM $table WHERE $where", $db_link));
}

/*
** Affiche sous forme de tableau les donnees contenues dans $list
*/
function table_disp_one($data, $list, $action, $entry, $max_upload_size = 30000)
{
  global $db_link;
  static $uniq_id = 0;

  //print("<p>ACTION destination :" . href(array()) . "</p>");

  print("<table class=table_info cols=2>\n".
	"<form enctype='multipart/form-data' method='post' action='");
  href(array());
  print("' name='form".$uniq_id."'>\n");

  print("<input type='hidden' name='_action' value='$action'>\n".
	"<input type='hidden' name='".$entry."' value='".$data["id"]."'>\n");

  for ($i = 0; $i < count($list); $i++)
    {
      /* skip if no read access right */
      if (access_check($list[$i][3]))
	{
	  if (access_check($list[$i][4]))
	    /* write access */
	    {
	      print("<tr><td class=table_info_title>\n");
	      print($list[$i][1]);
	      print("</td><td class=table_info_line0>\n");

	      /* switch on field type */
	      switch ($list[$i][2])
		{

		case ("upload"):
		  print("<input type=\"hidden\" name=\"MAX_FILE_SIZE\" value=\"".$max_upload_size."\">");
		  print("<input class=table_info_line0 type='file' size='65'");
		  print("name='_form_".$list[$i][0]."'>\n");
		  break;

		case ("image"):
		case ("photo"):
		case ("now"):
		case ("datetime"):
		case ("date"):
		case ("text"):
		case ("url"):
		case ("login"):
		  print("<input class=table_info_line0 type='text' ");
		  // Fixme: Why stripslashes is commented on awk??
		  //if (is_dev())
		  //  print("value='".stripslashes(encode_quote($data[$list[$i][0]]))."' ");
		  //else
		    print("value='".encode_quote($data[$list[$i][0]])."' ");
		  print("name='_form_".$list[$i][0]."'>\n");
		  break;

		  // case table_margin added by delecr_n
// 		case ("table_margin"):

// 		  break;

		case ("date_today"):
		  print("<input class=table_info_line0 type='text' ");
		  print("value='".date ("d/m/Y")."' ");
		  print("name='_form_".$list[$i][0]."'>\n");
		  break ;

		case ("textarea"):
		  print("<textarea class=table_info_line0 ");
		  print("wrap=virtual rows=4 name='_form_".$list[$i][0]."'>\n");
		  print($data[$list[$i][0]]."</textarea>");
		  break;

		case ("textarea_big"):
		  print("<textarea class=table_info_line0 ");
		  print("wrap=virtual rows=12 name='_form_".$list[$i][0]."'>\n");
		  print($data[$list[$i][0]]."</textarea>");
		  break;

		case ("checkbox"):
		  print("<input type=checkbox ");
		  if (intval($data[$list[$i][0]]))
		    print("checked ");
		  print("name='_form_".$list[$i][0]."'>\n");
		  break;

		case ("linkinfo"):
		  break;

	/*
		case ("link"):
		  break;
        */

	            // Maybe a stupidity but I copy ode from list
	    case ("link"):
	      if ($data[$list[$i][0]] == "")
		break;
	      $info_name = isset($list[$i][6]) ? $list[$i][6] : "_entry";
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
		  if (isset($list[$i][7]))
		    $list[$i][4][$list[$i][7]] = $data["id2"];

		  $list[$i][4][$info_name] = $data[$list[$i][0]];
		  href($list[$i][4]);
		}
	      else
		print($list[$i][4]);
	      print("'>".$list[$i][1]."</a>\n");
	      break;



		case ("bool"):
		  print("<select class=table_info_line0 name='_form_".$list[$i][0]."'>\n");
		  for ($j = 0; $j < count($list[$i][5]); $j++)
		    {
		      if ($list[$i][5][$j] == "---")
			continue;
		      print ("<option ");
		      if ($j == $data[$list[$i][0]])
			print("selected ");
		      print ("value='".$j."'>".$list[$i][5][$j]."</option>\n");
		    }
		  print("</select>\n");
		  break;


		  /**************** shen ************/
		case ("liste"):
		  print("<select class=table_info_line0 name='_form_".$list[$i][0]."'>\n");
		  for ($j = 0; $j < count($list[$i][5]); $j++)
		    {
		      if ($list[$i][5][$j] == "---")
			continue;
		      print ("<option ");
		      if ($j == $data[$list[$i][0]])
			print("selected ");
		      print ("value='".$j."'>".$list[$i][5][$j]."</option>\n");
		    }
		  print("</select>\n");
		  break ;
		  /**********************************/

		default:
		  print("<select class=table_info_line0 name='_form_".$list[$i][0]."'>\n");
		  if (isset($list[$i][6]))
		    $res_b = db_query($list[$i][6]);
		  else
		    $res_b = db_query("SELECT id, libelle FROM ".$list[$i][2]);
		  while($row = db_fetch_row($res_b))
		    {
		      print ("<option ");
		      if ($row[0] == $data[$list[$i][0]])
			print("selected ");
		      print ("value='".$row[0]."'>".$row[1]."</option>\n");
		    }
		  print("</select>\n");
		}

	      $wr_count++;

	    }
	  else
	    /* read access */
	    {
	      if ($list[$i][2] == "title")
		{
		  print("<tr><td class=table_info_htitle colspan=2>\n");
		  print($list[$i][1]);
		  print("</td></tr>\n");
		  continue;
		}

	      print("<tr><td class=table_info_title>\n");
	      print($list[$i][1]);
	      print("</td><td class=table_info_line0>\n");

	      /* switch on field type */
	      switch ($list[$i][2])
		{
		case ("photo"):
		  print("<img align=right class=table_info_photo src='".$data[$list[$i][0]]."'>\n");
		  break;

		case ("image"):
		  print("<img class=table_info_image src='".$data[$list[$i][0]]."'>\n");
		  break;

		case ("percent"):
		  style_percent($data[$list[$i][0]], 80);
		  break;
/*
		case ("link"):
		  print("<div class=table_info_text><a class=links ");
		  if ($list[$i][6])
		    print("onclick=\"return (confirm('".$list[$i][6]."'))\" ");
		  print(" href='");
		  if (is_array($list[$i][5]))
		    {
		      $list[$i][5]["_entry"] = $data["id"];
		      href($list[$i][5]);
		    }
		  else
		    print($list[$i][5]);
		  print("'>".$list[$i][0]."</a></div>\n");
		  break;
*/
            // Maybe a stupidity but I copy ode from list
	    case ("link"):
	      if ($data[$list[$i][0]] == "")
		break;
	      $info_name = isset($list[$i][6]) ? $list[$i][6] : "_entry";
	      print("<a class=links href='");
	      if (is_array($list[$i][4]))
		{
		  if (isset($list[$i][7]))
		    $list[$i][4][$list[$i][7]] = $data["id2"];

		  $list[$i][4][$info_name] = $data[$list[$i][0]];
		  href($list[$i][4]);
		}
	      else
		print($list[$i][4]);
	      print("'>".$list[$i][1]."</a>\n");
	      break;



		case ("linkinfo"):
		  print("<div class=table_info_text><a class=links ");
		  if ($list[$i][6])
		    print("onclick=\"return (confirm('".$list[$i][6]."'))\" ");
		  print(" href='");
		  if (is_array($list[$i][5]))
		    {
		      $list[$i][5]["_info_id"] = $data["id"];
		      href($list[$i][5]);
		    }
		  else
		    print($list[$i][5]);
		  print("'>".$list[$i][0]."</a></div>\n");
		  break;

		case ("url"):
		  print("<a class=links href='".$data[$list[$i][0]].
			"'>".$data[$list[$i][0]]."</a>");
		  break;

		case ("checkbox"):
		  if (intval($data[$list[$i][0]]))
		    print("<div class=table_info_text>Oui</div>");
		  else
		    print("<div class=table_info_text>Non</div>");
		  break;

		case ("text_nodb"):
		case ("text"):
		case ("datetime"):
		case ("date"):
		case ("date_today"):
		case ("now"):
		case ("textarea_big"):
		case ("textarea"):
		  print("<div class=table_info_text>".stripslashes($data[$list[$i][0]])."</div>");
		  break;

		case ("login"):
		  print("<div class=table_info_text><a class=links ".
			"href='mailto:".$data[$list[$i][0]]."@epita.fr'>".
			$data[$list[$i][0]]."</a></div>\n");
		  break;

		case ("bool"):
		  print("<div class=table_info_text>".
			$list[$i][5][$data[$list[$i][0]]]."</div>");
		  break;

		default:
		  if (isset($list[$i][5]))
		    $res_b = db_query($list[$i][5]);
		  else
		    $res_b = db_query("SELECT libelle FROM ".$list[$i][2].
				      " WHERE id=".$data[$list[$i][0]]);
		  $row = db_fetch_row($res_b);
		  print ("<div class=table_info_text>".$row[0]."</div>");
		}
	    }

	  print("</td></tr>\n");
	}
    }

  if ($wr_count)
    print("<tr><td class=table_info_valid colspan=2>\n".
	  "<a class=links href='javascript:document.form".$uniq_id.
	  ".submit()'>Valider</a></td></tr>\n");

  $uniq_id++;
  print("</form></table>\n\n");
}

/*
** Affiche les donnees contenues dans le result set $query
** en utilisant le template $list
*/

function table_disp($query, $list, $action, $entry)
{
  if (db_num_rows($query) < 1)
    print("<div class=text>Aucune donnee disponible</div>");
  else
    /* display a table for each row */
    while ($data = db_fetch_array($query))
      table_disp_one($data, $list, $action, $entry);
}

/*
** Effectue une mise a jour dans la base de donnees de la table
** $table, sur les champs de la liste $list
** $where: contenu de la clause WHERE de la requete generee
*/
function table_update($table, $list, $where)
{
  global $db_link;

  /* build field list for db query */
  $j = 0;
  for ($i = 0; $i < count($list); $i++)
    {
      if (access_check($list[$i][4]))
	{
 	  switch ($list[$i][2])
	    {
	      /* use perl regexpr to reorder date format */
	    case ("date"):
	      if (!preg_match('/^([0-9]{2})\\/([0-9]{2})\\/([0-9]{4})$/',
			     $_POST["_form_".$list[$i][0]],
			      $dlst))
		return (1);
	      $fields .= (++$j > 1 ? ", " : "").
		$list[$i][0].
		"='".($dlst[3]."-".$dlst[2]."-".$dlst[1])."'";
	      break;

	      /* use perl regexpr to reorder date format */
	    case ("now"):
	    case ("datetime"):
	      if (!preg_match('/^([0-9]{2})\\/([0-9]{2})\\/([0-9]{4}) ([0-9]{2}:[0-9]{2}:[0-9]{2})$/',
			      $_POST["_form_".$list[$i][0]],
			      $dlst))
		return (1);
	      $fields .= (++$j > 1 ? ", " : "").
		$list[$i][0].
		"='".($dlst[3]."-".$dlst[2]."-".$dlst[1])." ".$dlst[4]."'";
	      break;

	    case ("checkbox"):
	      $fields .= (++$j > 1 ? ", " : "").
		$list[$i][0].
		"=".addslashes($_POST["_form_".$list[$i][0]] == "on" ? 1 : 0);
	      break;

	    default:
	      if (isset($_POST["_form_".$list[$i][0]]))
		$fields .= (++$j > 1 ? ", " : "").
		  $list[$i][0].
		  "='".addslashes($_POST["_form_".$list[$i][0]])."'";
	    case ("text_nodb"):
	    }
	}
    }

  if (!$fields)
    return (1);
  $res = db_query("UPDATE $table SET $fields WHERE $where");
  return ($res);
}

/*
** Effectue une recherche dans une base de donnees sur la table $table.
** La clause WHERE est generee en partie et $where est concatene.
** $limit permet de restreindre la selection a un intervalle
*/
function table_search($table, $list, $where, $limit)
{
  global $db_link;

  /* build field list for db query */
  $j = 0;
  for ($i = 0; $i < count($list); $i++)
    {
      if (access_check($list[$i][4]))
	{
	  $val = $_POST["_form_".$list[$i][0]];

 	  switch ($list[$i][2])
	    {
	    case ("bool"):
	      if (intval($val) < 1)
		continue;

	      $fields .= (++$j > 1 ? " AND " : "").
		$list[$i][0].
		"=".(intval($val) - 1);
	      break;

	    default:
	      if (strlen($val) < 1)
		continue;

	      $fields .= (++$j > 1 ? " AND " : "").
		$list[$i][0].
		" LIKE '%".addslashes($_POST["_form_".$list[$i][0]])."%'";
	    }
	}
    }

  if (!$fields)
    return (0);

  return (db_query("SELECT * FROM $table WHERE $fields $where LIMIT ".$limit, $db_link));
}

/*
**
*/
function table_insert($table, $list, $extra)
{
  global $db_link;

  /* build field list for db query */

  for ($j = $i = 0; $i < count($list); $i++)
    {
      switch ($list[$i][2])
	{
	case ("link"):
	case ("title"):
	  continue 2;

	case ("datetime"):
	  $vals .= ($j > 0 ? ", " : "");
	  if (!preg_match('/^([0-9]{2})\\/([0-9]{2})\\/([0-9]{4}) ([0-9]{2}:[0-9]{2}:[0-9]{2})$/',
			  $_POST["_form_".$list[$i][0]],
			  $dlst))
	    $vals .= "'00-00-0000 00:00:00'";
	  else
	    $vals .= "'".$dlst[3]."-".$dlst[2]."-".$dlst[1]." ".$dlst[4]."'";
	  break;

	case ("now"):
	  $vals .= ($j > 0 ? ", " : "");
	  $vals .= "NOW()";
	  break;

	case ("checkbox"):
	  $vals .= ($j > 0 ? ", " : "");
	  $vals .= "'".addslashes($_POST["_form_".$list[$i][0]] == "on" ? 1 : 0)."'";
	  break;

	default:
	  $vals .= ($j > 0 ? ", " : "");
	  $vals .= "'".addslashes($_POST["_form_".$list[$i][0]])."'";
	  break;
	}
      $fields .= ($j++ > 0 ? ", " : "");
      $fields .= $list[$i][0];
    }

  for (reset($extra);
       $key = key($extra);
       next($extra))
    {
      if ($j++ > 0)
	{
	  $fields .= ", ";
	  $vals .= ", ";
	}
      $fields .= $key;
      $vals .= pos($extra);
    }

  if (!$fields)
    return (1);

  return (db_query("INSERT INTO $table ($fields) VALUES ($vals)", $db_link));
}

?>
