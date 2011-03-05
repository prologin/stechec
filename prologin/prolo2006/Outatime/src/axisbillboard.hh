//=============================================================================
//
// AxisBillboard class
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
// axisbillboard.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			AXISBILLBOARD_HH
# define		AXISBILLBOARD_HH

// {AxisBillboard} inclue les billboards alignés avec un axe.

#include		"billboard.hh"
#include		"vector3d.hh"

class			AxisBillboard : public Billboard
{
public:
  // Constructor
  AxisBillboard ();
  AxisBillboard (const float);

  // Accessors
  float			Length () const;
  const Vector3D &	Axis () const;

  // Modifier
  void			Length (const float);
  void			Axis (const Vector3D &);

protected:
  // Modifier
  void			ReAlign (const Matrix &) = 0;

private:
  float			_length;
  Vector3D		_axis;
};

#endif			// AXISBILLBOARD_HH
