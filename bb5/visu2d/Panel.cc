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

#include "Api.hh"
#include "Game.hh"
#include "Colors.hh"
#include "Panel.hh"

BEGIN_NS(sdlvisu);

Panel::Panel(Game& g)
  : VirtualSurface("VPanel", 300, 600),
    g_(g),
    bg_("image/panel/panel.jpg"),
    wheel_("image/panel/wheels.png"),
    name_("Bobtag.ttf", 24, 200, 40),
    position_("Bobtag.ttf", 16, 150, 40),
    ma_("Bobtag.ttf", 32, 50, 45),
    st_("Bobtag.ttf", 32, 60, 45),
    ag_("Bobtag.ttf", 32, 80, 45),
    av_("Bobtag.ttf", 32, 80, 50),
    misc_("Bobtag.ttf", 32, 80, 50),
    skills_("Bobtag.ttf", 16, 100, 80)
{
  setPos(Point(500, 0));

  bg_.setZ(-1);
  addChild(&bg_);

  wheel_.setPos(113, 506);
  wheel_.splitNbFrame(13, 1);
  addChild(&wheel_);

  player_picture_.setPos(45, 150);
  player_picture_.hide();
  addChild(&player_picture_);

  name_.setPos(50, 95);
  name_.setTextColor(whitesmoke_color);
  name_.hide();
  addChild(&name_);

  position_.setPos(50, 120);
  position_.setTextColor(whitesmoke_color);
  position_.hide();
  addChild(&position_);

  ma_.setPos(215, 145);
  ma_.setTextColor(whitesmoke_color);
  ma_.hide();
  addChild(&ma_);

  st_.setPos(215, 180);
  st_.setTextColor(whitesmoke_color);
  st_.hide();
  addChild(&st_);

  ag_.setPos(215, 220);
  ag_.setTextColor(whitesmoke_color);
  ag_.hide();
  addChild(&ag_);

  av_.setPos(215, 260);
  av_.setTextColor(whitesmoke_color);
  av_.hide();
  addChild(&av_);

  misc_.setPos(210, 310);
  misc_.setTextColor(whitesmoke_color);
  misc_.hide();
  addChild(&misc_);

  skills_.setPos(50, 310);
  skills_.setTextColor(whitesmoke_color);
  skills_.hide();
  addChild(&skills_);

  for (int i = 0; i < 2; i++)
    {
      turn_[i].load("image/panel/turn.png");
      turn_[i].hide();
      addChild(&turn_[i]);

      digit_score_[i].load("image/panel/digits_score.png");
      digit_score_[i].splitNbFrame(10, 1);
      addChild(&digit_score_[i]);
      digit_reroll_[i].load("image/panel/digits_reroll.png");
      digit_reroll_[i].splitNbFrame(10, 1);
      addChild(&digit_reroll_[i]);
    }
  digit_score_[0].setPos(157, 510);
  digit_score_[1].setPos(197, 510);
  digit_reroll_[0].setPos(68, 505);
  digit_reroll_[1].setPos(68, 541);

  for (int i = 0; i < 3; i++)
    {
      digit_time_[i].load("image/panel/digits_time.png");
      digit_time_[i].splitNbFrame(10, 1);
      addChild(&digit_time_[i]);
    }
  digit_time_[0].setPos(161, 546);
  digit_time_[1].setPos(188, 546);
  digit_time_[2].setPos(203, 546);
}

Panel::~Panel()
{
}

void Panel::displayPlayerInfo(int team_id, int player_id)
{
  std::ostringstream os;
  const CPlayer* p;

  g_.getApi()->selectTeam(team_id);
  g_.getApi()->selectPlayer(player_id);
  p = g_.getApi()->getPlayer();
  assert(p != NULL);

  player_picture_.load(std::string("image/figs/") + p->getPlayerPicture());
  player_picture_.show();

  name_.setText(p->getName());
  name_.show();

  position_.setText(p->getPositionName());
  position_.show();

  os << p->getMa();
  ma_.setText(os.str());
  ma_.show();
  os.str("");

  os << p->getSt();
  st_.setText(os.str());
  st_.show();
  os.str("");

  os << p->getAg();
  ag_.setText(os.str());
  ag_.show();
  os.str("");

  os << p->getAv();
  av_.setText(os.str());
  av_.show();
  os.str("");

  os << "#" << (p->getId() + 1);
  misc_.setText(os.str());
  misc_.show();
  os.str("");

  skills_.clearText();
  Player::SkillList l = p->getSkillList();
  Player::SkillList::const_iterator it;
  for (it = l.begin(); it != l.end(); ++it)
    skills_.addText(Player::stringify(*it));
  skills_.show();
}

void Panel::hidePlayerInfo()
{
  player_picture_.hide();
  name_.hide();
  position_.hide();
  ma_.hide();
  st_.hide();
  ag_.hide();
  av_.hide();
  skills_.hide();
}

void Panel::setTurn(int player_id, int cur_turn)
{
  if (cur_turn == 0)
    turn_[player_id].hide();
  else
    {
      turn_[player_id].show();
      turn_[player_id].setPos(4 + (cur_turn * 33), 428 + player_id * 36);
    }
}

void Panel::update()
{
  Api* api = g_.getApi();

  for (int i = 0; i < 2; i++)
    {
      api->selectTeam(i);
      const CTeam* t = api->getTeam();
      int r = 1;
      int s = 0;
      if (t != NULL)
        {
          r = api->getTeam()->getRemainingReroll() + 1;
          s = api->getTeam()->getScore() + 1;
        }
      if (r < 1 || r >= 10)
        r = 1;
      if (s < 1 || s >= 10)
        s = 1;
      digit_reroll_[i].setFrame(r);
      digit_score_[i].setFrame(s);
    }

  // Update time remaining and wheel
  int tr = api->remainingTime();
  if (tr >= 0) {
    digit_time_[0].setFrame(tr / 60 % 10 + 1);
    digit_time_[1].setFrame(tr / 10 % 6 + 1);
    digit_time_[2].setFrame(tr % 10 + 1);
    wheel_.anim(100);
  } else {
    wheel_.stopAnim();
  }


  VirtualSurface::update();
}

END_NS(sdlvisu);
