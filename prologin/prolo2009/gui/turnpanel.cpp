//
// turnpanel.cpp for  in /home/user/prolo_stechec/prologin/prolo2009/gui
// 
// Made by user
// Login   <user@epitech.net>
// 
// Started on  Thu Apr 30 13:14:44 2009 user
// Last update Thu Apr 30 16:03:38 2009 user
//

#include "display.h"
#include "turnpanel.h"
#include "font.h"

TurnPanel::TurnPanel(Surface *src, Surface *dst)
{
  src->SetAlphaFlags();
  this->_sfc = src;
  this->_dst = dst;
  this->_turn = 0;
}

TurnPanel::~TurnPanel(void)
{
}

void		TurnPanel::UpdateTurn(int turn)
{
  this->_turn = turn;
}

void		TurnPanel::Refresh(void)
{
  int		x, y;
  Font		*ft;
  SfcField	pos;
  Display	*dsp;


  this->_sfc->Blit(*(this->_dst), this->_pos);

  dsp = Display::GetInstance();
  ft = dsp->GetFont(FT_INFOS2);

  ft->setColor(0xFFFFFFFF);
  ft->Text.str("");
  ft->Text << "Turn " << this->_turn;

  x = this->_pos.getPosX();
  y = this->_pos.getPosY();
  pos.setPos(x + (this->_sfc->getWidth() >> 1) - 20, y + 12);
  ft->Blit(*(this->_dst), pos);
}

void		TurnPanel::setDst(Surface *dst)
{
  this->_dst = dst;
}

void		TurnPanel::setPos(unsigned int x, unsigned int y)
{
  this->_pos.setPos(x, y);
}

int		TurnPanel::getWidth(void) const
{
  return (this->_sfc->getWidth());
}

int		TurnPanel::getHeight(void) const
{
  return (this->_sfc->getHeight());
}
