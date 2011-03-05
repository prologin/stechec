//=============================================================================
//
// TextureManager class
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
// texturemanager.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		<iostream>
#include		<jpeglib.h>
#include		<jerror.h>
#include		<GL/gl.h>
#include		<GL/glu.h>
#include		"Api.hh"
#include		"texturemanager.hh"

// ================================================================ Constructor

TextureManager::TextureManager ():
  _active_texture(0),
  _texturing(true),
  _texturing_activated(false),
  _filtering(true),
  _mipmapping(true),
  _filtering_modified(true)
{
  LOG1("TextureManager::TextureManager");
}

TextureManager::~TextureManager ()
{
  LOG1("TextureManager::~TextureManager");

  for (Textures_t::const_iterator it = _textures.begin ();
       it != _textures.end ();
       ++it)
    {
      glDeleteTextures (1, &(it->id));
      delete it->data;
    }
}


// ================================================================== Accessors

bool			TextureManager::Texturing () const
{
  return _texturing;
}

bool			TextureManager::Filtering () const
{
  return _filtering;
}

bool			TextureManager::Mipmapping () const
{
  return _mipmapping;
}


// ================================================================== Modifiers

void			TextureManager::Texturing (const bool texturing)
{
  if (!texturing)
    Disactivate_texturing ();
  _texturing = texturing;
}

void			TextureManager::Filtering (const bool filtering)
{
  if (filtering == _filtering)
    return;
  _filtering = filtering;
  _filtering_modified = true;
}

void			TextureManager::Mipmapping (const bool mipmapping)
{
  if (mipmapping == _mipmapping)
    return;
  _mipmapping = mipmapping;
  _filtering_modified = true;
}

void			TextureManager::Toggle_texturing ()
{
  Texturing (!Texturing ());
}

void			TextureManager::Toggle_filtering ()
{
  Filtering (!Filtering ());
}

void			TextureManager::Toggle_mipmapping ()
{
  Mipmapping (!Mipmapping ());
}


// ====================================================================== Calls

static void		Set_texture_filtering (const bool filtering,
					       const bool mipmaping)
{
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		   (filtering ?
		    (mipmaping ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) :
		    (mipmaping ? GL_NEAREST_MIPMAP_NEAREST :
		     GL_NEAREST)));
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		   (filtering ? GL_LINEAR : GL_NEAREST));
}

void			TextureManager::Update ()
{
  LOG1("TextureManager::Update");

  if (!_texturing || !_filtering_modified)
    return;
  for (Textures_t::const_iterator it = _textures.begin ();
       it != _textures.end ();
       ++it)
    {
      Activate_texture (it->id);
      Set_texture_filtering (it->filtering && _filtering,
			     it->mipmapping && _mipmapping);
    }
  _filtering_modified = false;
}

void			TextureManager::Activate_texture (const unsigned int i)
{
  if (!_texturing)
    return;
  Bind_texture (i);
  Activate_texturing ();
}

void			TextureManager::Activate_texturing ()
{
  if (!_texturing || _texturing_activated)
    return;
  glEnable (GL_TEXTURE_2D);
  _texturing_activated = true;
}

void			TextureManager::Disactivate_texturing ()
{
  if (!_texturing || !_texturing_activated)
    return;
  glDisable (GL_TEXTURE_2D);
  _texturing_activated = false;
}

//
// Function to load a Jpeg file.
// A bit ugly (not supporting many things), but this should be enough.
//
unsigned int		TextureManager::Load_texture (const char * filename,
						      const bool filtering,
						      const bool mipmapping,
						      const bool repeat)
{
  LOG1("TextureManager::Load_texture");

  // First we check if the texture hasn't been loaded already
  for (Textures_t::const_iterator it = _textures.begin ();
       it != _textures.end ();
       ++it)
    if (0 == strcmp (filename, it->name))
      return it->id;

  // Ok, we load it
  FILE *fd;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char * line;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&cinfo);

  if (0 == (fd = fopen(filename, "rb")))
    return 0;

  jpeg_stdio_src (&cinfo, fd);
  jpeg_read_header (&cinfo, TRUE);
  if (cinfo.image_width != cinfo.image_height)
    return 0;

  Texture texture;
  texture.name = filename;
  texture.size = cinfo.image_width;

  jpeg_start_decompress (&cinfo);

  if (cinfo.out_color_space == JCS_GRAYSCALE)
    {
      texture.format = GL_ALPHA;
      texture.data = new unsigned char[texture.size * texture.size];
      while (cinfo.output_scanline < cinfo.output_height)
	{
	  line = texture.data + texture.size * cinfo.output_scanline;
	  jpeg_read_scanlines (&cinfo, &line, 1);
	}
    }
  else
    {
      texture.format = GL_RGB;
      texture.data = new unsigned char[texture.size * texture.size * 3];
      while (cinfo.output_scanline < cinfo.output_height)
	{
	  line = texture.data + 3 * texture.size * cinfo.output_scanline;
	  jpeg_read_scanlines (&cinfo, &line, 1);
	}
    }

  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);

  glGenTextures (1, &(texture.id));
  Bind_texture (texture.id);
  gluBuild2DMipmaps (GL_TEXTURE_2D,
		     texture.format,
		     texture.size,
		     texture.size,
		     texture.format,
		     GL_UNSIGNED_BYTE,
		     texture.data);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		   (repeat ? GL_REPEAT : GL_CLAMP));
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		   (repeat ? GL_REPEAT : GL_CLAMP));

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		   (filtering ?
		    (mipmapping ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) :
		    (mipmapping ? GL_NEAREST_MIPMAP_NEAREST :
		     GL_NEAREST)));
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		   (filtering ? GL_LINEAR : GL_NEAREST));

  _textures.push_back (texture);

  // From the Red Book:
  // "0 is reserved and is never returned by glGenTextures()."
  // This is why we can use it as an error code. :-)
  return texture.id;
}

void			TextureManager::Bind_texture (const unsigned int i)
{
  if (i == _active_texture)
    return;
  _active_texture = i;
  glBindTexture (GL_TEXTURE_2D, _active_texture);
}

// ============================================================================
