//=============================================================================
//
// DataCenter class
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
// datacenter.hh : made by Zavie (Julien Guertault)
//
//=============================================================================

#ifndef			DATACENTER_HH
# define		DATACENTER_HH

// {DataCenter} contient les différentes {Frame} d'un match : c'est
// l'historique du jeu.

#include		"Api.hh"
#include		"building.hh"
// #include		"frame.hh"
#include		"singleton.hh"

#include		"almanach.hh"
#include		"casino.hh"
#include		"delorean.hh"
#include		"groundpart.hh"
#include		"player.hh"

// #include		"worldbillboard.hh"

class			DataCenter : public Singleton<DataCenter>
{
  friend class		Singleton<DataCenter>;

protected:
  // Constructor and destructor
  DataCenter ();
  ~DataCenter ();

public:
  void			Init_world (Api *);
  void			New_frame (Api *);

  // Accessors
  unsigned int		Frames () const;
  unsigned int		Current_frame () const;

  unsigned int		Date () const;
  const GroundParts_t &	Ground_parts () const;
  const Almanach *	The_almanach () const;
  const DeLorean *	The_de_Lorean () const;
  const Buildings_t &	Buildings () const;
  const Casinos_t &	Casinos () const;
  const Players_t &	Players () const;

//   const Frames_t &	Frames () const;
//   const Frame &		Current_frame () const;
//   const Frame &		Previous_frame () const;

  // Modifiers
  void			First_frame ();
  void			Previous_frame ();
  void			Next_frame ();
  void			Last_frame ();

private:
//   const GroundParts_t &	Ground_parts () const;
//   const Buildings_t &	Buildings () const;
//   const Casinos_t &	Casinos () const;

  unsigned int		_current_frame;

//   int			_world_width;
//   int			_world_height;
  GroundParts_t		_ground_parts;
  Buildings_t		_buildings;
  Casinos_t		_casinos_base;
//   Frames_t		_frames;

  std::vector <unsigned int>	_date;
  std::vector <Almanach *>	_almanach;
  std::vector <DeLorean *>	_delorean;
  std::vector <Casinos_t>	_casinos;
  std::vector <Players_t>	_players;

//   std::vector <Mesh>		_ground_representation;
//   Mesh				_delorean_representation;
//   ScreenBillboard		_almanach_representation;
//   std::vector <Mesh>		_buildings_representation;
//   std::vector <Mesh>		_casinos_representation;
//   std::vector <WorldBillboard>	_players_representation;
};

#endif			// DATACENTER_HH
