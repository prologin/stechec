//=============================================================================
//
// Library entry point
// Copyright (C) 2006  Julien Guertault
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//=============================================================================
//
// API for Prologin 2006
//
// main.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"Api.hh"
#include		"client_cx.hh"
#include		"datacenter.hh"
#include		"glutgui.hh"
#include		"tools.hh"

//
// Library entry point
//
extern "C" int		run (xml::XMLConfig *, Api * api, ClientCx * ccx)
{
  // Creation of the window
  GlutGUI * gui(GlutGUI::Instance (api, ccx));

  if (gui->Init_GUI (0 /* &argc */, 0 /* argv */,
		     800, 300, "Prologin 2006") != 0)
    {
      std::cerr << "GUI initialization failed T_T" << std::endl;
      return 1;
    }

  // Creation of the world
  DataCenter::Instance ()->Init_world (api);

  // Loading of the textures and files
  if (gui->Init_files () != 0)
    {
      std::cerr << "Files initialization failed T_T" << std::endl;
      DataCenter::Destroy ();
      return 1;
    }

  // Creation of the rendering
  if (gui->Init_API () != 0)
    {
      std::cerr << "API initialization failed T_T" << std::endl;
      DataCenter::Destroy ();
      return 1;
    }

  // Loop
  gui->Run (500, 33);

  // Never reached since GlutMainLoop is called
  return 0;
}

//=============================================================================
