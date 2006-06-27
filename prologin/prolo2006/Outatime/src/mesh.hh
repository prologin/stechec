//=============================================================================
//
// Mesh class
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
// mesh.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			MESH_HH
# define		MESH_HH

// {Mesh} est un objet 3D solide composé d'un seul bloc.

#include		"3ds.hh"
#include		"solid.hh"

class			Mesh : public Solid
{
public:
  // Constructor and destructor
  Mesh ();
  Mesh (const struct t3DObject &);

  virtual ~Mesh ();

  // Calls
  virtual void		Render (const float) const;

  // Modifier
  void			Texture (const unsigned int);

private:
  bool			_loaded;
  unsigned int		_number_vertices;
//   unsigned int		_number_faces;
//   unsigned int		_number_texcoord;
  unsigned int		_texture;
  const char *		_name;
  float *		_vertices;
  float *		_normals;
  float *		_texture_uv;
//   unsigned int *	_faces; // man glDrawElements
};

#endif			// MESH_HH
