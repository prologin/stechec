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

#ifndef GAMEFIELD_HH_
# define GAMEFIELD_HH_

# include "Api.hh"
# include "Input.hh"
# include "VirtualScrollableSurface.hh"
# include "Sprite.hh"

/*
** Game field (on the left).
*/
class VisuField : public VirtualScrollableSurface
{
public:
  VisuField(Api* api, Input& input);
  virtual ~VisuField();

  void setBallPos(const Point& pos);
  
  virtual void update();

private:
  Api*          api_;
  Input&        inp_;

  Surface       bg_;
  Sprite        player_[2][16];
  Sprite        ball_;
};

#endif /* !GAMEFIELD_HH_ */
