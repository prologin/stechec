/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/
#include "RaceHandler.hh"
#include "TeamWriter.hh"
#include "Player.hh"
#include <vector>
#include <fstream>
  
  
TeamWriter::TeamWriter()
{
}

TeamWriter::~TeamWriter()
{
}

void TeamWriter::writeTeam(const char* filename, Team* team)
{
  using namespace std;
  
  ofstream fout(filename, ios::trunc);
  fout << "<?xml version=\"1.0\"?>" << endl;
  fout << "<team race=\"" << team->getRace()->getName() << "\" BBversion=\"" << RaceHandler::BBversion_ << "\" emblem=\"" << team->getEmblem() << "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"team.xsd\">" << endl;
  fout << "  <name>" << team->getName() << "</name>" << endl;
  fout << "  <coach>" << team->getHeadCoach() << "</coach>" << endl;
  fout << "  <bank>" << team->getBank() << "</bank>" << endl;
  fout << "  <reroll>" << team->getReroll() << "</reroll>" << endl;
  fout << "  <rerollcost>" << team->getRerollUnitaryCost() << "</rerollcost>" << endl;
  fout << "  <fanfactor>" << team->getFanFactor() << "</fanfactor>" << endl;
  fout << "  <assistant>" << team->getAssistantCoach() << "</assistant>" << endl;
  fout << "  <cheerleader>" << team->getCheerleader() << "</cheerleader>" << endl;
  fout << "  <apothecary>" << team->getApothecary() << "</apothecary>" << endl;
  fout << "  <players>" << endl;
  for (int i=1; i<=TEAM_SIZE; i++)
  {
      Player* player = team->getPlayer(i);
      if (strcmp("", player->getPositionTitle()) != 0)
      {
          fout << "     <player name=\"" << player->getName() << "\" number=\"" << i << "\" position=\"" << player->getPositionTitle() << "\" display=\"" << "test.png" << "\">" << endl;
          fout << "       <ma>" << player->getMovementAllowance() << "</ma>" << endl;
          fout << "       <st>" << player->getStrength() << "</st>" << endl;
          fout << "       <ag>" << player->getAgility() << "</ag>" << endl;
          fout << "       <av>" << player->getArmourValue() << "</av>" << endl;
          fout << "       <cost>" << player->getValue() << "</cost>" << endl;
          
          std::vector<const char*> skills = player->getSkills();
          if (skills.size() == 0)
          {
              fout << "       <skills />" << endl;                  
          }
          else
          {
              fout << "       <skills>" << endl;
              for (unsigned int j=0; j<skills.size(); j++)
              { 
                fout << "         <skill>" << skills[j] << "</skill>" << endl;
              }
              fout << "       </skills>" << endl;
          }
          fout << "       <inj>" << player->getInjuries() << "</inj>" << endl;
          fout << "       <com>" << player->getCompletions() << "</com>" << endl;
          fout << "       <td>" << player->getTouchDowns() << "</td>" << endl;
          fout << "       <int>" << player->getInterceptions() << "</int>" << endl;
          fout << "       <cas>" << player->getCasualties() << "</cas>" << endl;
          fout << "       <mpv>" << player->getMostValuablePlayer() << "</mpv>" << endl;
          fout << "       <spp>" << player->getStarPlayerPoints() << "</spp>" << endl;
          fout << "    </player>" << endl;
      }
  }
  fout << "  </players>" << endl;
  fout << "</team>" << endl;
  
  fout.close();     
}
