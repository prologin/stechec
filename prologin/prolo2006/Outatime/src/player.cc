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
// player.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"player.hh"

// ================================================================ Constructor

Player::Player (const unsigned int x,
		const unsigned int y,
		const unsigned int id,
		const unsigned int team,
		const bool own_almanach,
		const bool own_delorean,
		const state_t state):
  Moving(x, y),
  _id(id),
  _team(team),
  _own_almanach(own_almanach),
  _own_de_lorean(own_delorean),
  _state(state)
{}


// ================================================================== Accessors

unsigned int		Player::Id () const		{ return _id; }
unsigned int		Player::Team () const		{ return _team; }

bool			Player::OwnAlmanach () const
{
  return _own_almanach;
}

bool			Player::OwnDeLorean () const
{
  return _own_de_lorean;
}

Player::state_t		Player::State () const		{ return _state; }

// ============================================================================
