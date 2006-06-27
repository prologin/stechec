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
// mesh.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"GL/gl.h"
#include		"mesh.hh"
#include		"texturemanager.hh"

// ================================================= Constructor and destructor

Mesh::Mesh ():
  Solid (),
  _loaded(false),
  _number_vertices(0),
//   _number_faces(0),
//   _number_texcoord(0),
  _texture(0),
  _name(0),
  _vertices(0),
  _normals(0),
  _texture_uv(0)// ,
//   _faces(0)
{}

Mesh::Mesh (const struct t3DObject & object):
  Solid (),
  _loaded (true),
  _number_vertices(3 * object.numOfFaces),
//   _number_faces(object.numOfFaces),
//   _number_texcoord(object.numTexVertex),
  _texture(object.materialID), // FIXME
  _name(object.strName)
{
  _vertices = new float(3 * _number_vertices);
  _normals = new float(3 * _number_vertices);
//   _faces = new float(_number_faces);
  _texture_uv = new float(2 * _number_vertices);

  for (int face = 0; face < object.numOfFaces; ++face)
    for (unsigned int vertex = 0; vertex < 3; ++vertex)
      {
	const unsigned int indice(3 * face + vertex);


	const unsigned int vertex_index =
	  object.pFaces[face].vertIndex[vertex];

	const Vector3D & p(object.pVerts[vertex_index]);
	_vertices[3 * indice] = p.X ();
	_vertices[3 * indice + 1] = p.Y ();
	_vertices[3 * indice + 2] = p.Z ();

	const Vector3D & n(object.pNormals[vertex_index]);
	_normals[3 * indice] = n.X ();
	_normals[3 * indice + 1] = n.Y ();
	_normals[3 * indice + 2] = n.Z ();


	const unsigned int texcoord_index =
	  object.pFaces[face].coordIndex[vertex];

	const float u(object.pTexVerts[texcoord_index].x);
	const float v(object.pTexVerts[texcoord_index].y);
	_texture_uv[2 * indice] = u;
	_texture_uv[2 * indice + 1] = v;
      }
}


Mesh::~Mesh ()
{
  if (!_loaded)
    return;
  delete _vertices;
  delete _normals;
  delete _texture_uv;
//   delete _faces;
}

// ======================================================================= Call

void			Mesh::Render (const float) const
{
  if (!_loaded)
    return;

  TextureManager::Instance ()->Activate_texture (_texture);

  glPushMatrix ();
  Load_position ();
  Load_orientation ();

  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);

  glTexCoordPointer (2, GL_FLOAT, 0, _texture_uv);
  glNormalPointer (GL_FLOAT, 0, _normals);
  glVertexPointer (3, GL_FLOAT, 0, _vertices);

  glDrawArrays (GL_TRIANGLES, 0, _number_vertices);

  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState (GL_NORMAL_ARRAY);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);

  glPopMatrix ();
}


// =================================================================== Modifier

void			Mesh::Texture (const unsigned int texture)
{
  _texture = texture;
}

// ============================================================================
