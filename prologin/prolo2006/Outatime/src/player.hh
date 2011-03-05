//=============================================================================
//
// Player class
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
// player.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			PLAYER_HH
# define		PLAYER_HH

// {Player} représente les joueurs (pas les candidats, mais leurs
// personnages).

#include		<list>
#include		"moving.hh"

class			Player : public Moving
{
public:
  typedef		enum
  {
    idle,
    bored,
    ko,
    moving,
    attacking,
    attacked
  }			state_t;

  // Constructor
  Player (const unsigned int,
	  const unsigned int,
	  const bool,
	  const unsigned int,
	  const unsigned int,
	  const bool,
	  const bool,
	  const state_t);

  // Accessors
  unsigned int		Id () const;
  unsigned int		Team () const;
  bool			OwnAlmanach () const;
  bool			OwnDeLorean () const;
  state_t		State () const;

private:
  const unsigned int	_id;
  const unsigned int	_team;
  const bool		_own_almanach;
  const bool		_own_de_lorean;
  const state_t		_state;
};

typedef
std::list<Player *>	Players_t;

#endif			// PLAYER_HH
