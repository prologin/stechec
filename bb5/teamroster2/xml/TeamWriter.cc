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
#include "common/Player.hh"

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
          fout << "     <player name=\"" << player->getName() << "\" number=\"" << i << "\" position=\"" << player->getPositionTitle() << "\" display=\"" << player->getDisplay() << "\">" << endl;
          fout << "       <ma>" << player->getMovementAllowance() << "</ma>" << endl;
          fout << "       <st>" << player->getStrength() << "</st>" << endl;
          fout << "       <ag>" << player->getAgility() << "</ag>" << endl;
          fout << "       <av>" << player->getArmourValue() << "</av>" << endl;
          fout << "       <cost>" << player->getValue() << "</cost>" << endl;
          
          std::vector<const char*> skills = player->getSkillsNormal();
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
          std::vector<const char*> skillsDouble = player->getSkillsDouble();
          if (skillsDouble.size() == 0)
          {
              fout << "       <skills_double />" << endl;                  
          }
          else
          {
              fout << "       <skills_double>" << endl;
              for (unsigned int j=0; j<skillsDouble.size(); j++)
              { 
                fout << "         <skill_double>" << skillsDouble[j] << "</skill_double>" << endl;
              }
              fout << "       </skills_double>" << endl;
          }
          
          // add information for injuries 
          fout << "       <inj ";
                  
          if (player->getMissNextMatch())
          {
             fout << " missNextMatch=\"true\"";
          }
          if (player->getNigglingInjuries() > 0)
          {
             fout << " nigglingInjuries=\"" << player->getNigglingInjuries() << "\"";
          }
          if (player->getMaReducted() > 0)
          {
             fout << " MaReduction=\"" << player->getMaReducted() << "\"";
          }
          if (player->getStReducted() > 0)
          {
             fout << " StReduction=\"" << player->getStReducted() << "\"";
          }
          if (player->getAgReducted() > 0)
          {
             fout << " AgReduction=\"" << player->getAgReducted() << "\"";
          }
          if (player->getAvReducted() > 0)
          {
             fout << " AvReduction=\"" << player->getAvReducted() << "\"";
          }
          fout << " />" << endl; 

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
  
  if (strcmp("", team->getBackground()) != 0)
  {
    fout << "    <background>" << team->getBackground() << "</background>" << endl;
  }
  
  fout << "</team>" << endl;
  
  fout.close();     
}
