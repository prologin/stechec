//
// infopanel.cpp for  in /home/user/prolo_stechec/prologin/prolo2009/gui
// 
// Made by user
// Login   <user@epitech.net>
// 
// Started on  Thu Apr 30 13:14:44 2009 user
// Last update Fri May  1 02:55:09 2009 user
//

#include "display.h"
#include "infopanel.h"
#include "font.h"

InfoPanel::InfoPanel(Surface *src, Surface *dst)
{
  src->SetAlphaFlags();
  this->_sfc = src;
  this->_dst = dst;
}

InfoPanel::~InfoPanel(void)
{
}

void		InfoPanel::UpdatePlayer(int id, int score, int money, int bid)
{
  this->_player_info[id].score = score;
  this->_player_info[id].money = money;
  this->_player_info[id].bid = bid;
}

void		InfoPanel::Refresh(void)
{
  int		i, x , y;
  Font		*ft;
  Font		*ft2;
  Surface	*infos;
  SfcField	pos;
  Display	*dsp;


  this->_sfc->Blit(*(this->_dst), this->_pos);

  dsp = Display::GetInstance();
  
  ft = dsp->GetFont(FT_INFOS);
  ft->setColor(0xFF000000);

  ft2 = dsp->GetFont(FT_INFOS2);
  ft2->setColor(0xFFFFFFFF);

  infos = dsp->GetSurface(SFC_PANEL_INFO);
  for (i = 0; i < (int)this->_player_info.size(); i++)
    {
      x = this->_pos.getPosX();
      y = this->_pos.getPosY() + 6 + i * infos->getHeight();
      pos.setPos(x, y);
      infos->Blit(*(this->_dst), pos);
      
      ft2->Text.str("");
      ft2->Text << "PLAYER " << (i + 1);
      pos.setPos(x + (infos->getWidth() >> 1), y);
      ft2->Blit(*(this->_dst), pos);
      
      ft->Text.str("");
      ft->Text << this->_player_info[i].score;
      pos.setPos(x + (infos->getWidth() >> 1) + 9, y + 18);
      ft->Blit(*(this->_dst), pos);

      ft->Text.str("");
      ft->Text << this->_player_info[i].money;
      pos.setPos(x + (infos->getWidth() >> 1) + 9, y + 38);
      ft->Blit(*(this->_dst), pos);

      ft->Text.str("");
      ft->Text << this->_player_info[i].bid;
      pos.setPos(x + (infos->getWidth() >> 1) + 9, y + 58);
      ft->Blit(*(this->_dst), pos);
    }
}

void		InfoPanel::setDst(Surface *dst)
{
  this->_dst = dst;
}

void		InfoPanel::setPos(unsigned int x, unsigned int y)
{
  this->_pos.setPos(x, y);
}

int		InfoPanel::getWidth(void) const
{
  return (this->_sfc->getWidth());
}

int		InfoPanel::getHeight(void) const
{
  return (this->_sfc->getHeight());
}
