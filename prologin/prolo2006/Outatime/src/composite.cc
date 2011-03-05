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
// composite.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"GL/gl.h"
#include		<vector>
#include		"composite.hh"
#include		"mesh.hh"
#include		"texturemanager.hh"

// ================================================= Constructor and destructor

Composite::Composite (Gems_t & objects):
  Solid (),
  _objects(objects)
{}

Composite::Composite (const struct t3DModel & object):
  Solid ()
{
  for (std::vector<t3DObject>::const_iterator it = object.pObject.begin ();
       it != object.pObject.end ();
       ++it)
    {
      Mesh * new_mesh = new Mesh (*it);
      TextureManager * texturemanager = TextureManager::Instance ();
      new_mesh->Texture (texturemanager->
			 Load_texture (object.pMaterials[it->
							 materialID].strName,
				       true,
				       true,
				       false));
      _objects.push_back (new_mesh);
    }
}

Composite::~Composite ()
{
  for (Gems_t::iterator it = _objects.begin (); it != _objects.end (); ++it)
    delete (*it);
}


// =================================================================== Accessor

const Gems_t &		Composite::Objects () const
{
  return _objects;
}


// ======================================================================= Call

void			Composite::Render (const float state) const
{
  glPushMatrix ();
  Load_position ();
  Load_orientation ();
  for (Gems_t::const_iterator it = _objects.begin ();
       it != _objects.end ();
       ++it)
    (*it)->Render (state);
  glPopMatrix ();
}

void			Composite::Update (const Matrix & camera)
{
  for (Gems_t::iterator it = _objects.begin (); it != _objects.end (); ++it)
    (*it)->Update (camera);
}

// ============================================================================
