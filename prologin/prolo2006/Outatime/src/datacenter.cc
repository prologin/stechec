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
// datacenter.cc : made by Zavie (Julien Guertault)
//
//=============================================================================

#include		"datacenter.hh"
#include		"texturemanager.hh"
#include		"vector3d.hh"

// ================================================= Constructor and destructor

DataCenter::DataCenter ():
  _current_frame(0)
//   _world_width(0),
//   _world_height(0)
{
  LOG1("DataCenter::DataCenter");
}

DataCenter::~DataCenter ()
{
  LOG1("DataCenter::~DataCenter");

  for (std::vector <Almanach *>::const_iterator it = _almanach.begin ();
       it != _almanach.end ();
       ++it)
    delete (*it);
  for (std::vector <DeLorean *>::const_iterator it = _delorean.begin ();
       it != _delorean.end ();
       ++it)
    delete (*it);
  for (std::vector <Players_t>::const_iterator it = _players.begin ();
       it != _players.end ();
       ++it)
      for (Players_t::const_iterator iitt = it->begin ();
	   iitt != it->end ();
	   ++iitt)
	delete (*iitt);

  for (GroundParts_t::const_iterator it = _ground_parts.begin ();
       it != _ground_parts.end ();
       ++it)
    delete (*it);
  for (Buildings_t::const_iterator it = _buildings.begin ();
       it != _buildings.end ();
       ++it)
    delete (*it);
  for (Casinos_t::const_iterator it = _casinos_base.begin ();
       it != _casinos_base.end ();
       ++it)
    delete (*it);
//   for (Frames_t::const_iterator it = _frames.begin ();
//        it != _frames.end ();
//        ++it)
//     delete (*it);
}


//============================================================== Initialization

//
// Appelé une seule fois, pour tout ce qui ne changera pas
//
void			DataCenter::Init_world (Api * api)
{
  LOG1("DataCenter::Init_world");

//   _world_width = api->taille_ville_x ();
//   _world_height = api->taille_ville_y ();

//   for (int y = 0; y < _world_width; ++y)
//     for (int x = 0; x < _world_width; ++x)

  const int width = api->taille_ville_x ();
  const int height = api->taille_ville_y ();
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      {
	const int kind(api->terrain_graphique (x, y));
	switch (kind)
	  {
	  case GRASSY_AREA:
	    _ground_parts.push_back (new GroundPart (x, y,
						     GroundPart::full,
						     GroundPart::up));
	    break;
	  case STREET:
	    {
	      int streets = 0; // Nombre de rue adjacente
	      GroundPart::direction_t street = GroundPart::none;
	      GroundPart::direction_t grass = GroundPart::none;
	      if (api->terrain_graphique (x, y - 1) == STREET)
		{
		  ++streets;
		  street = GroundPart::up;
		}
	      else
		grass = GroundPart::up;

	      if (api->terrain_graphique (x - 1, y) == STREET)
		{
		  ++streets;
		  street = GroundPart::left;
		}
	      else
		grass = GroundPart::left;

	      if (api->terrain_graphique (x, y + 1) == STREET)
		{
		  ++streets;
		  street = GroundPart::down;
		}
	      else
		grass = GroundPart::down;

	      if (api->terrain_graphique (x + 1, y) == STREET)
		{
		  ++streets;
		  street = GroundPart::right;
		}
	      else
		grass = GroundPart::right;

	      GroundPart * new_ground_part = 0;
	      switch (street)
		{
		case 0:
		  new_ground_part = new GroundPart (x, y,
						    GroundPart::empty,
						    GroundPart::none);
		  break;
		case 1:
		  new_ground_part = new GroundPart (x, y,
						    GroundPart::dead_end,
						    street);
		  break;
		case 2:
		  new_ground_part = new GroundPart (x, y,
						    GroundPart::street,
						    street);
		  break;
		case 3:
		  new_ground_part = new GroundPart (x, y,
						    GroundPart::tee,
						    grass);
		  break;
		case 4:
		  new_ground_part = new GroundPart (x, y,
						    GroundPart::cross,
						    GroundPart::none);
		  break;
		}
	      _ground_parts.push_back (new_ground_part);
	      break;
	    }
	  case BUILDING:
	    _buildings.push_back (new Building (x, y));
	    break;
	  case BET_PLACE:
	    _casinos_base.push_back (new Casino (x, y,
						 api->casino (x, y),
						 false));
	    break;
	  }
      }

  // Chargement du premier tour
  New_frame (api);
}

//
// Appelé à chaque tour
//
void			DataCenter::New_frame (Api * api)
{
  LOG1("DataCenter::New_frame --------------------------------------------");

  // Point de vue omniscient
  api->switchTeam (-1);

  // La date
  const int date = api->current_date ();

  const int almanach_owner_id (api->joueur_possede_almanach ());
  const int delorean_owner_id (api->joueur_possede_delorean ());
  const Player * almanach_owner = 0;
  const Player * delorean_owner = 0;

  // Les joueurs
  Players_t players;
  const int num_players = api->equipes () * api->taille_equipe ();
  for (int player = 0; player < num_players; ++player)
    {
      // Si le joueur est dans le temps, on passe
      if (api->etat_joueur (player) == STATE_TIME_WARP)
	continue;

      const bool possede_almanach(almanach_owner_id == player);
      const bool possede_delorean(delorean_owner_id == player);
      Player::state_t etat = Player::idle;
      if (api->etat_joueur (player) == STATE_KO)
	etat = Player::ko;
#warning "FIXME: obtenir (ou calculer selon) l'état du joueur"
      Player * new_player = new Player (api->position_joueur_x (player),
					api->position_joueur_y (player),
					true, // FIXME
					player,
					api->equipe (player),
					possede_almanach,
					possede_delorean,
					etat);
      if (possede_almanach)
	almanach_owner = new_player;
      if (possede_delorean)
	delorean_owner = new_player;
      players.push_back (new_player);
    }

#warning "FIXME: gérer le cas où l'almanach ou la De Lorean ne sont pas là"
  // L'almanach
  Almanach * new_almanach = new Almanach (api->position_almanach_x (),
					  api->position_almanach_y (),
					  true, // FIXME
					  almanach_owner);

  // La de Lorean
  DeLorean * new_delorean = new DeLorean (api->position_delorean_x (),
					  api->position_delorean_y (),
					  true, // FIXME
					  delorean_owner,
					  api->autonomie_delorean ());

  // Les casinos
  // On a déjà la liste, on se contente de regarder s'ils sont ouverts.
  Casinos_t casinos;
  for (Casinos_t::const_iterator it = _casinos_base.begin ();
       it != _casinos_base.end ();
       ++it)
    {
      Casino * new_casino =
	new Casino ((*it)->X (),
		    (*it)->Y (),
		    (*it)->Id (),
		    (api->casino_ouvert ((*it)->Id ()) == 1));
      casinos.push_back (new_casino);
    }

  // A ce stade on connaît la position de chaque élément mobile. Il ne
  // reste qu'à déterminer leur visibilité
  for (int team = 0; team < api->equipes (); ++team)
    {
      api->switchTeam (team);
      new_almanach->Visible_by_team (team, (api->almanach_visible () == 1));
      new_delorean->Visible_by_team (team, (api->delorean_visible () == 1));
      for (Players_t::iterator it = players.begin ();
	   it != players.end ();
	   ++it)
	(*it)->Visible_by_team (team,
				(api->joueur_visible ((*it)->Id ()) == 1));
    }

//   // Création de la frame grâce à toutes ces informations
//   Frame * new_frame = new Frame (date,
// 				 Ground_parts (),
// 				 almanach,
// 				 delorean,
// 				 Buildings (),
// 				 casinos,
// 				 players);

  // Ajout de la frame dans la pile
//   _frames.push_back (new_frame);

//   if (_current_frame == Frames () - 1)
//     _current_frame++;

  _date.push_back(date);
  _almanach.push_back(new_almanach);
  _delorean.push_back(new_delorean);
  _casinos.push_back(casinos);
  _players.push_back(players);
}


// ================================================================== Accessors

unsigned int		DataCenter::Frames () const
{
  return _date.size ();
}

unsigned int		DataCenter::Current_frame () const
{
  return _current_frame;
}

// Elements pour le reste du monde :

unsigned int		DataCenter::Date () const
{
  return _date[_current_frame];
}

const GroundParts_t &	DataCenter::Ground_parts () const
{
  return _ground_parts;
}

const Almanach *	DataCenter::The_almanach () const
{
  return _almanach[_current_frame];
}

const DeLorean *	DataCenter::The_de_Lorean () const
{
  return _delorean[_current_frame];
}

const Buildings_t &	DataCenter::Buildings () const
{
  return _buildings;
}

const Casinos_t &	DataCenter::Casinos () const
{
  return _casinos[_current_frame];
}

const Players_t &	DataCenter::Players () const
{
  return _players[_current_frame];
}

// const Frames_t &	DataCenter::Frames () const
// {
//   return _frames;
// }

// #warning "FIXME: exception et gestion de la navigation"

// const Frame &		DataCenter::Current_frame () const
// {
//   Frames_t::const_reverse_iterator frame = _frames.rbegin ();
// //   assert (frame != _frames.rend());
//   return **frame;
// }

// const Frame &		DataCenter::Previous_frame () const
// {
//   Frames_t::const_reverse_iterator last = _frames.rbegin ();
// //   assert (last != _frames.rend());

//   Frames_t::const_reverse_iterator frame = last;
//   frame++;
//   if (frame == _frames.rend())
//     return **last;

//   return **frame;
// }

// const Buildings_t &	DataCenter::Buildings () const
// {
//   return _buildings;
// }

// const Casinos_t &	DataCenter::Casinos () const
// {
//   return _casinos;
// }


// ================================================================== Modifiers

void			DataCenter::First_frame ()
{
  _current_frame = 0;
}

void			DataCenter::Previous_frame ()
{
  if (_current_frame > 0)
    --_current_frame;
}

void			DataCenter::Next_frame ()
{
  if (_current_frame < Frames () - 1)
    ++_current_frame;
}

void			DataCenter::Last_frame ()
{
  _current_frame = Frames ();
  if (_current_frame > 0)
    --_current_frame;
}

// ============================================================================
