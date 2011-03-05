//=============================================================================
//
// Composite class
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
// composite.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			COMPOSITE_HH
# define		COMPOSITE_HH

// {Composite} est un groupe d'objets 3D.

#include		"3ds.hh"
#include		"solid.hh"

class			Composite : public Solid
{
public:
  // Constructor and destructor
  Composite (Gems_t &);
  Composite (const struct t3DModel &);
  virtual ~Composite ();

  // Accessor
  const Gems_t &	Objects () const;

  // Calls
  virtual void		Render (const float) const;
  virtual void		Update (const Matrix &);

private:
  Gems_t		_objects;
};

#endif			// COMPOSITE_HH
