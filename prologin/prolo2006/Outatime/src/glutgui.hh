//=============================================================================
//
// GlutGUI class
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
// glutgui.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			GLUTGUI_HH
# define		GLUTGUI_HH

// {GlutGUI} est une implémentationde {GUI} pour le cas de
// l'utilisation avec glut.

// C'est un singleton, mais pour éviter l'héritage multiple, c'est
// recodé.

// Pour pouvoir passer des fonctions membres commes callbacks, il faut
// qu'elles soient satiques. Et pour quelles puissent faire leur
// boulot, il faut un singleton.

#include		"gui.hh"

class			GlutGUI : public GUI
{
protected:
  // Constructor and destructor
  GlutGUI (Api *, ClientCx *);
  ~GlutGUI ();

public:
  // Instance
  static GlutGUI *	Instance (Api *, ClientCx *);
  static GlutGUI *	Instance ();
  static void		Destroy ();

  // Initialization
  int			Init_GUI (int *,
				  char **,
				  const unsigned int,
				  const unsigned int,
				  const char *);

  void			Run (const int, const int);
  void			Redraw ();
  int			Elapsed_time ();

protected:
  // Glut callbacks
  static void		GlutRenderTimerFunc (int);
  static void		GlutFrameTimerFunc (int);
  static void		GlutIdleFunc ();
  static void		GlutDisplayFunc ();
  static void		GlutReshapeFunc (int, int);
  static void		GlutKeyboardFunc (unsigned char, int, int);
  static void		GlutSpecialFunc (int, int, int);
  static void		GlutMouseFunc (int, int, int, int);
  static void		GlutPassiveMotionFunc (int, int);
  static void		GlutMotionFunc (int, int);

  static GlutGUI *	_glut_instance;
};

#endif			// GLUTGUI_HH
