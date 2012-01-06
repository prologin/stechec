/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#include "VirchorGui.hh"

class ConfFile;

VirchorGui::VirchorGui(Api* api, ClientCx* ccx)
  : api_(api),
    ccx_(ccx)
{
}

VirchorGui::~VirchorGui()
{
}

int VirchorGui::jump()
{
  std::cout << "entree dans jump" << std::endl;
  std::ostringstream stream;
  stream << "prolo /player_jump";

  int team_id = 0;
  for (team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
        unit_[team_id][unit_id].x = api_->position_joueur_x(unit_id + team_id * 3);
        unit_[team_id][unit_id].y = api_->taille_ville_y() - api_->position_joueur_y(unit_id + team_id * 3) - 1;
	stream << " player" << team_id << unit_id
	       << " " << unit_[team_id][unit_id].x
	       << " " << unit_[team_id][unit_id].y;
                       
      }
  for (;team_id < 6; team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
	stream << " player" << team_id << unit_id
	       << " 0 -3";
      }

  // Debug
  std::cout << stream.rdbuf()->str() << std::endl;
   
  Point pos_delorean(api_->position_delorean_x(), api_->taille_ville_y() - api_->position_delorean_y() - 1);
  Point pos_almanach(api_->position_almanach_x(), api_->taille_ville_y() - api_->position_almanach_y() - 1);
  delorean_ = pos_delorean;
  almanach_ = pos_almanach;
  
  stream << " laureane"
	 << " " << delorean_.x
	 << " " << delorean_.y;

  stream << " book" 
	 << " " << almanach_.x
	 << " " << almanach_.y;
  v.send(stream);
  std::cout << "sortie dans jump" << std::endl;
}

int VirchorGui::translate()
{
  LOG3("entree dans translate");
  std::ostringstream stream;
  stream << "prolo /player_translate";

  int team_id = 0;
  for (team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
        Point new_pos;
        new_pos.x = api_->position_joueur_x(unit_id + team_id * 3);
        new_pos.y = api_->taille_ville_y() - api_->position_joueur_y(unit_id + team_id * 3) - 1;

	stream << " player" << team_id << unit_id
	       << " " << unit_[team_id][unit_id].x 
	       << " " << unit_[team_id][unit_id].y
	       << " " << new_pos.x
	       << " " << new_pos.y;
        unit_[team_id][unit_id] = new_pos;
      }

  for (;team_id < 6; team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
      {
	stream << " player" << team_id << unit_id
	       << " 0 -3 0 -3";
      }

  // debug
  std::cout << stream.rdbuf()->str() << std::endl;

  stream << " laureane"
	 << " " << delorean_.x
	 << " " << delorean_.y
	 << " " << api_->position_delorean_x()
	 << " " << api_->taille_ville_y() - api_->position_delorean_y() - 1;

  stream << " book" 
	 << " " << almanach_.x
	 << " " << almanach_.y
	 << " " << api_->position_almanach_x()
	 << " " << api_->taille_ville_y() - api_->position_almanach_y() - 1;

  Point pos_delorean(api_->position_delorean_x(), api_->taille_ville_y() - api_->position_delorean_y() - 1);
  Point pos_almanach(api_->position_almanach_x(), api_->taille_ville_y() - api_->position_almanach_y() - 1);
  delorean_ = pos_delorean;
  almanach_ = pos_almanach;

  v.send(stream);
  LOG3("sortie de translate");
}



int VirchorGui::init()
{
  std::cout << "entree dans init" << std::endl;

  // Initialize udp connection to virchor.
  v.connect("localhost", 1978, 1979);
  // Get the current date.
  date_ = api_->current_date();

  std::ostringstream stream;
  stream << "score ";
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    stream << "j" << team_id + 1<< ": 0 ";

  stream << "date: " << date_ << " delaureane autonomie: " << api_->autonomie_delorean();
  v.send(stream);
  this->jump();

  std::cout << "sortie dans init" << std::endl;
  return 0;
}

// This function is called at the end of any turn.
void VirchorGui::endOfTurn()
{
  LOG4("Enter endOfTurn...");

  const int current_date = api_->current_date();

  std::ostringstream stream;
  stream << "score ";
  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    stream << " j" << team_id + 1 << ": " << api_->score(team_id);
  
  stream << " date: " << current_date
         << " delaureane autonomie: " << api_->autonomie_delorean();
   
  v.send(stream);
  if (current_date != date_ + 1)
    jump();
  else
    translate();

  date_ = current_date;


  // Re-initialize player positions.
  /*  for (int team_id = 0; team_id < api_->equipes(); team_id++)
    for (int unit_id = 0; unit_id < MAX_GOODMEN; unit_id++)
    unit_[team_id][unit_id].newTurn();
    delorean_.newTurn();
    almanach_.newTurn();
  */
  LOG4("date %1", date_);
  LOG4("endOfTurn finished...");
}


int VirchorGui::run()
{
  api_->setEventHandler(this);
  LOG4("Do init...");
  if (init())
    return 1;
  LOG4("Init done.");

  while (api_->getState() != GS_END)
    {
      // Check if virchor send us something.
      if (v.poll(0))
	ccx_->setReady();

      // Process any incoming rules messages. Block at most 50 ms.
      while (ccx_->process(true))
        ;
    }
  LOG4("Exiting from main loop. Game finished.");
  return 0;
}

// Library entry point.
extern "C" int run(ConfFile*, Api* api, ClientCx* ccx)
{
  VirchorGui visu(api, ccx);

  try {
    return visu.run();
  } catch (const NetUDPError& e) {
    ERR("UDP: %1", e.what());
    return 1;
  }
}
