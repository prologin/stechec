//=============================================================================
//
// Gem class
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
// gem.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			GEM_HH
# define		GEM_HH

// {Gem} inclue tout ce qui peut être affiché.

#include		<vector>
#include		"matrix.hh"
#include		"vector3d.hh"

class			Gem
{
public:
  // Cette énumération a pour but de permettre de gérer l'ordre
  // d'affichage des éléments (les éléments transparents doivent être
  // affichés après les éléments opaques par exemples)
  typedef enum
    {
      background,
      object,
      transparent,
      light
    }			kind;

  // Constructor and destructor
  Gem ();
  virtual ~Gem ();

  // Calls
  virtual void		Render (const float) const = 0;
  virtual void		Update (const Matrix &);

  // Accessors
  kind			Kind () const;
  const Vector3D &	Position () const;

protected:
  // Call
  void			Load_position () const;

  // Modifier
  void			Kind (const kind);
  void			Position (const Vector3D &);

private:
  kind			_kind;
  Vector3D		_position;
};

typedef
std::vector<Gem *>	Gems_t;

#endif			// GEM_HH
