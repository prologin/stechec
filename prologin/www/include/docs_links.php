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


// Documents list is here

// Format:

//  name (string) :
//   - name of the document

//  path (string) :
//   - path to the document from DOCS_PATH

//  type (int) : (see defines below)
//   - 0: Custom
//   - 1: HTML
//   - 2: Tarball
//   ...

//  allow_disp (bool): 
//   - whether user can show document in the current web page (html or txt only)



define (DOCS_PATH, "docs/");

define (SECTION_DOCS, 0);
define (SECTION_LINKS, 1);



// declaration des documents

define (DOCTYPE_CUSTOM, 0);
define (DOCTYPE_HTML, 1);
define (DOCTYPE_TARBALL, 2);
define (DOCTYPE_PDF, 3);
define (DOCTYPE_TXT, 4);


$icons_list = array(DOCTYPE_CUSTOM => "/icons/unknown.png",
		    DOCTYPE_HTML => "/icons/layout.png",
		    DOCTYPE_TARBALL => "/icons/tar.png",
		    DOCTYPE_PDF => "/icons/pdf.png",
		    DOCTYPE_TXT => "/icons/text.png");
		    


$ing1_docs = array(
		   array("name" => "Consignes a lire ABSOLUMENT",
			 "path" => "consignes.html",
			 "type" => DOCTYPE_HTML,
			 "allow_disp" => 1));




$important_docs = array(
		   array("name" => "La netiquette",
			 "path" => "netiquette.html",
			 "type" => DOCTYPE_HTML,
			 "allow_disp" => 1),
		   array("name" => "La charte du PIE",
			 "path" => "charte.html",
			 "type" => DOCTYPE_HTML,
			 "allow_disp" => 1),
		   array("name" => "Reglement des salles machines",
			 "path" => "reglement-sm.txt",
			 "type" => DOCTYPE_TXT,
			 "allow_disp" => 1));



$acus_doc = array(
		   array("name" => "Mails ACUs, mode d'emploi",
			 "path" => "mails.html",
			 "type" => DOCTYPE_HTML,
			 "allow_disp" => 1),
		   array("name" => "Soutenances ACUs, mode d'emploi",
			 "path" => "soutenances.html",
			 "type" => DOCTYPE_HTML,
			 "allow_disp" => 1),
		   array("name" => "FGA des ACUs",
			 "path" => "reponses.txt",
			 "type" => DOCTYPE_TXT,
			 "allow_disp" => 1));


$normes_docs = array(
		     array("name" => "... en txt",
			   "path" => "norme.txt",
			   "type" => DOCTYPE_TXT,
			   "allow_disp" => 1),

		     array("name" => "... en pdf",
			   "path" => "norme.pdf",
			   "type" => DOCTYPE_PDF,
			   "allow_disp" => 0));



$section_docs_list = array(
			   array("secname" => "ING 1 - Mode d'emploi",
				 "docs" => $ing1_docs),

			   array("secname" => "Les documents de reference",
				 "docs" => $important_docs),

			   array("secname" => "Les documents ACUs",
				 "docs" => $acus_doc),

			   array("secname" => "La norme",
				 "docs" => $normes_docs));
		      

// declaration des liens


$indispensable_links = array(
			     array("url" => "http://www.epita.fr:8000/faq/",
				   "name" => "La FAQ Epita",
				   "type" => DOCTYPE_HTML),

/*
			     array("url" => "http://epita.idealbug.com",
				   "name" => "Le site des delegues 2006",
				   "type" => DOCTYPE_HTML),
*/
			     array("url" => "http://www.lrde.epita.fr/~akim/compil/",
				   "name" => "Tiger et compilation",
				   "type" => DOCTYPE_HTML)

);

$sutiles_links = array(
 		       array("url" => "http://www.linux-france.org/article/debutant/ini-unix/",
			     "name" => "Unix Mode d'emploi",
			     "type" => DOCTYPE_HTML),

 		       array("url" => "http://spenfen.free.fr/unix/vi.htm",
			     "name" => "Manuel Vi",
			     "type" => DOCTYPE_HTML),

 		       array("url" => "http://www.earthside.org/nerd/emacs-refcard.html",
			     "name" => "Manuel Emacs",
			     "type" => DOCTYPE_HTML),

		       array("url" => "http://www.sgi.com/tech/stl/",
			     "name" => "Documentation STL",
			     "type" => DOCTYPE_HTML)

);

$utiles_links = array(
 		       array("url" => "http://slrn.sourceforge.net/manual/slrn-manual.html",
			     "name" => "Manuel Slrn",
			     "type" => DOCTYPE_HTML),

 		       array("url" => "http://www.gnus.org/manual/gnus.pdf",
			     "name" => "Manuel Gnus",
			     "type" => DOCTYPE_PDF),

		       array("url" => "http://www.cppreference.com/",
			     "name" => "Reference C/C++",
			     "type" => DOCTYPE_PDF),

		       array("url" => "http://java.sun.com/j2se/1.4.2/docs/api/",
			     "name" => "Documentation Java 1.4.2",
			     "type" => DOCTYPE_PDF),

		       array("url" => "http://caml.inria.fr/ocaml/htmlman/index.html",
			     "name" => "Documentation Objective Caml",
			     "type" => DOCTYPE_PDF)
);

$interesting_links = array(
		       array("url" => "http://www.gnurou.org/documents/smart-questions-fr.html",
		       	     "name" => "Poser les questions intelligemment",
			     "type" => DOCTYPE_HTML),

		       array("url" => "http://www.eskimo.com/~scs/C-faq/top.html",
		       	     "name" => "FAQ comp.lang.c",
			     "type" => DOCTYPE_HTML)
);

$section_links_list = array(
			    array("secname" => "Les Indispensables",
				  "links" => $indispensable_links),
			    array("secname" => "Les Super-Utiles",
				  "links" => $sutiles_links),
			    array("secname" => "Les Utiles",
				  "links" => $utiles_links),
                            array("secname" => "Les Interessants",
                                  "links" => $interesting_links)
);

function doc_link_begincell($class_style)
{
  if ($class_style == "")
    {
      print "<td>\n";
    }
  else
    {
      print "<td class=$class_style>\n";
    }
}


function doc_link_endcell()
{
  print "</td>\n";
}



function docs_links_begintable()
{
  print "<table class=table_docs_links>\n";
}


function docs_links_endtable()
{
  print "</table>\n";
}



function docs_links_beginrow()
{
  print "<tr>\n";
}


function docs_links_endrow()
{
  print "</tr>\n";
}


function docs_links_show_icon($icon)
{
  global $icons_list;

  $path = $icons_list[$icon];
  print "<IMG SRC=$path>\n";
}


function docs_links_show_print($sec_id, $doc_id, $enable)
{
  if ($enable)
    {
      print "<A class=links HREF=\"";
      
      href(array("_id_page" => 11,
		 "_action" => "show", 
		 "_sec_id" => $sec_id,
		 "_doc_id" => $doc_id));
      
      print "\"> Afficher </A>\n";
    }
  else
    {
      print "Afficher";
    }
}



function docs_links_show_download($path)
{
  $realpath = DOCS_PATH . $path;
  print "<A class=links HREF=\"$realpath\"> Telecharger </A>\n";
}


function doc_disp($section_id, $doc_id)
{
  global $section_docs_list;
  
  $doc_list = $section_docs_list[$section_id]["docs"];
  $this_doc = $doc_list[$doc_id];

  doc_link_begincell("table_docs_links_icons");
  docs_links_show_icon($this_doc[type]);
  doc_link_endcell();

  doc_link_begincell("table_docs_links_docname");
  print "$this_doc[name]";
  doc_link_endcell();

  doc_link_begincell("table_docs_links_path");
  docs_links_show_print($section_id, $doc_id, $this_doc["allow_disp"]);
  doc_link_endcell();

  doc_link_begincell("table_docs_links_path");
  docs_links_show_download($this_doc[path]);
  doc_link_endcell();
}
		  


function docs_disp($section_id)
{
  global $section_docs_list;

  $doc_list = $section_docs_list[$section_id]["docs"];

  docs_links_begintable();
  for ($id = 0; $doc_list[$id]; $id++)
    {
      $this_doc = $doc_list[$id];
      docs_links_beginrow();
      doc_disp($section_id, $id);
      docs_links_endrow();
    }
  docs_links_endtable();
}





function docs_links_show_link($path)
{
  print "<A class=links HREF=\"$path\"> $path </A>\n";  
}



function link_disp($section_id, $link_id)
{
  global $section_links_list;
  
  $link_list = $section_links_list[$section_id]["links"];
  $this_link = $link_list[$link_id];

  doc_link_begincell("table_docs_links_icons");
  docs_links_show_icon($this_link[type]);
  doc_link_endcell();

  doc_link_begincell("table_docs_links_docname");
  docs_links_show_link($this_link["url"]);
  doc_link_endcell();

  doc_link_begincell("table_docs_links_path");
  print "$this_link[name]";
  doc_link_endcell();
}
		  


function links_disp($section_id)
{
  global $section_links_list;

  $link_list = $section_links_list[$section_id]["links"];

  docs_links_begintable();
  for ($id = 0; $link_list[$id]; $id++)
    {
      $this_link = $link_list[$id];
      docs_links_beginrow();
      link_disp($section_id, $id);
      docs_links_endrow();
    }
  docs_links_endtable();
}





function sections_disp($section)
{
  global $section_docs_list;
  global $section_links_list;

  switch ($section)
    {

    case SECTION_DOCS:
      for ($id = 0; $section_docs_list[$id]; $id++)
	{
	  $this_section = $section_docs_list[$id];
	  style_title($this_section[secname]);
	  docs_disp($id);
	  print "<br>";
	}
      break;



    case SECTION_LINKS:
      for ($id = 0; $section_docs_list[$id]; $id++)
	{
	  $this_section = $section_links_list[$id];
	  style_title($this_section[secname]);
	  links_disp($id);
	  print "<br>";
	}
      break;

    }
}


function doc_show($sec_id, $doc_id)
{
  global $section_docs_list;
  
  $this_doc = $section_docs_list[$sec_id]["docs"][$doc_id];
  
  if ($this_doc["allow_disp"] == 1)
    {
      $realpath = DOCS_PATH . $this_doc["path"];
      
      if ($this_doc["type"] == DOCTYPE_TXT)
	print "<PRE>\n";

      print("<div class='text'>");
      include ($realpath);
      print("</div>");

      if ($this_doc["type"] == DOCTYPE_TXT)
	print "</PRE>\n";
      
    }
}

?>
