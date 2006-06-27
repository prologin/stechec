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
  LOG1("GUI runned.");

  // Creation of the window
  GlutGUI * gui(GlutGUI::Instance (api, ccx));

  LOG1("GlutGUI instanciated.");

  if (gui->Init_GUI (0 /* &argc */, 0 /* argv */,
		     100, 100, "Prologin 2006") != 0)
    {
      LOG1("GUI initialization failed T_T");
//       std::cerr << "GUI initialization failed T_T" << std::endl;
      return 1;
    }

  LOG1("GUI initialized.");

  // Creation of the world
  DataCenter::Instance ()->Init_world (api);

  // Loading of the textures and files
  if (gui->Init_files () != 0)
    {
      LOG1("Files initialization failed T_T");
//       std::cerr << "Files initialization failed T_T" << std::endl;
      DataCenter::Destroy ();
      return 1;
    }

  LOG1("Files initialized.");

  // Creation of the rendering
  if (gui->Init_API () != 0)
    {
      LOG1("API initialization failed T_T");
//       std::cerr << "API initialization failed T_T" << std::endl;
      DataCenter::Destroy ();
      return 1;
    }

  LOG1("API initialized.");

  // Loop
  gui->Run (100, 500);

  LOG1("GUI finished.");

  // Never reached since GlutMainLoop is called
  return 0;
}

//=============================================================================
