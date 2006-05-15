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
// texturemanager.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			TEXTUREMANAGER_HH
# define		TEXTUREMANAGER_HH

// {TextureManager} gère et fournit les textures.

// Son rôle est donc de permettre de cherger les textures de façon
// transparente (on passe un nom de fichier et on récupère un id. De
// plus les chagements d'état OpenGL (coûteux en temps) sont minimisés
// en gardant en mémoire l'état en cours.

#include		<vector>
#include		<GL/gl.h>
#include		"singleton.hh"

class			TextureManager : public Singleton<TextureManager>
{
  friend class		Singleton<TextureManager>;

  // Pour l'animation c'est parfaitement gore : toutes les textures
  // d'une animation doivent être du même format et de la même taille,
  // et leurs données sont rangées à la suite.
  // Pour le moment je commente, parce que c'est laid.
  typedef struct
  {
    const char *	name;
    unsigned int	id;		// id of the first, for OpenGL
//     unsigned int	textures;	// > 1 <=> animation
    unsigned int	size;
    GLenum		format;		// GL_ALPHA, GL_RGB, GL_RGBA
    bool		filtering;
    bool		mipmapping;
    unsigned char *	data;
  }			Texture;
  typedef
  std::vector<Texture>	Textures_t;

protected:
  // Constructor and destructor
  TextureManager ();
  ~TextureManager ();

public:
  // Accessors
  bool			Texturing () const;
  bool			Filtering () const;
  bool			Mipmapping () const;

  // Modifiers
  void			Texturing (const bool);
  void			Filtering (const bool);
  void			Mipmapping (const bool);
  void			Toggle_texturing ();
  void			Toggle_filtering ();
  void			Toggle_mipmapping ();

  // Calls
  void			Update ();
  void			Activate_texture (const unsigned int i);
  void			Disactivate_texturing ();
  unsigned int		Load_texture (const char *,
				      const bool,
				      const bool,
				      const bool);

private:
  void			Activate_texturing ();
  void			Bind_texture (const unsigned int);

  unsigned int		_active_texture;
  // The _texturing member tells if the texturing is allowed or not,
  // while the _texturing_activated member is only for objects that
  // have or doesn't have texture.
  bool			_texturing;
  bool			_texturing_activated;
  bool			_filtering;
  bool			_mipmapping;
  bool			_filtering_modified;
  Textures_t		_textures;
};

#endif			// TEXTUREMANAGER_HH
