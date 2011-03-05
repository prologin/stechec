//
// infopanel2.cpp for  in /home/user/prolo_stechec/prologin/prolo2009/gui
// 
// Made by user
// Login   <user@epitech.net>
// 
// Started on  Thu Apr 30 13:14:44 2009 user
// Last update Fri May  1 02:48:04 2009 user
//

#include "display.h"
#include "infopanel2.h"
#include "font.h"

InfoPanel2::InfoPanel2(Surface *src, Surface *dst)
{
  src->SetAlphaFlags();
  this->_sfc = src;
  this->_dst = dst;
  this->_player_id = -1;
  this->_visible = false;
}

InfoPanel2::~InfoPanel2(void)
{
}

void		InfoPanel2::UpdatePlayer(int id, int score, int money, int bid)
{
  this->_player_id = id;
  this->_player.score = score;
  this->_player.money = money;
  this->_player.money = bid;
}

void		InfoPanel2::Refresh(void)
{
  int		x , y;
  Font		*ft;
  SfcField	pos;
  Display	*dsp;

  if (!this->_visible)
    return ;
  this->_sfc->Blit(*(this->_dst), this->_pos);

  dsp = Display::GetInstance();
  
  ft = dsp->GetFont(FT_INFOS2);
  ft->setColor(0xFFFFFFFF);
  
  x = this->_pos.getPosX() + (this->_sfc->getWidth() >> 1);
  y = this->_pos.getPosY();

  pos.setPos(x, y + 50);
  ft->Text.str("");
  ft->Text << "GAME ENDED !";
  ft->Blit(*(this->_dst), pos);

  pos.setPos(x, y + 70);
  ft->Text.str("");
  if (this->_player_id == -1)
    ft->Text << "Nobody won the game :(";
  else
    ft->Text << "Player " << (this->_player_id + 1) << " won the game";
  x = this->_pos.getPosX() + (this->_sfc->getWidth() >> 1);
  y = this->_pos.getPosY() + 40;
  ft->Blit(*(this->_dst), pos);
}

void		InfoPanel2::setDst(Surface *dst)
{
  this->_dst = dst;
}

void		InfoPanel2::setPos(unsigned int x, unsigned int y)
{
  this->_pos.setPos(x, y);
}

void		InfoPanel2::setVisible(bool v)
{
  this->_visible = v;
}

int		InfoPanel2::getWidth(void) const
{
  return (this->_sfc->getWidth());
}

int		InfoPanel2::getHeight(void) const
{
  return (this->_sfc->getHeight());
}
