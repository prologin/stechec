//=============================================================================
//
// Billboard class
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
// billboard.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			BILLBOARD_HH
# define		BILLBOARD_HH

// {Billboard} inclue les éléments affichés sous forme de billboard.

#include		"gem.hh"

class			Billboard : public Gem
{
public:
  // Constructor and destructor
  Billboard ();
  virtual ~Billboard ();

  // Calls
  void			Render (const float) const;
  void			Update (const Matrix &);

protected:
  // Modifier
  virtual void		ReAlign (const Matrix &) = 0;

  float			_texture_uv[4];
  float			_vertices[12];
};

#endif			// BILLBOARD_HH
