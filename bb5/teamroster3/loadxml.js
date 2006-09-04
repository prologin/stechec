/********************************************************
 * Copyright (C) 2002-2003, CodeHouse.com. All rights reserved.
 * CodeHouse(TM) is a registered trademark.
 *
 * THIS SOURCE CODE MAY BE USED FREELY PROVIDED THAT
 * IT IS NOT MODIFIED OR DISTRIBUTED, AND IT IS USED
 * ON A PUBLICLY ACCESSIBLE INTERNET WEB SITE.
 * 
 * CodeHouse.com JavaScript Library Module: Load XML Document Method
 *
 * You can obtain this script at http://www.codehouse.com
 ********************************************************/
function CJL_loadXmlDocument(xmlFile, cbFun)
{
   var xdoc;

   if( window.ActiveXObject && /Win/.test(navigator.userAgent) )
   {
      xdoc = new ActiveXObject("Microsoft.XMLDOM");

      xdoc.async = false;
      xdoc.load(xmlFile);

      cbFun(xdoc);

      return true;
   }
   else if( document.implementation && document.implementation.createDocument )
   {
      xdoc = document.implementation.createDocument("", "", null);
      xdoc.load(xmlFile);

      xdoc.onload = function()
      {
         cbFun(xdoc);
      }

      return true;
   }
   else
   {
      return false;
   }
}
