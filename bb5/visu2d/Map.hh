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

# include "Constants.hh"
# include "GameButton.hh"
# include "Sprite.hh"
# include "VirtualScrollableSurface.hh"

BEGIN_NS(sdlvisu);

class Game;
class VisuPlayer;

  //! @brief Map zones identifiers.
  enum eMapZone {
    MZ_EJECTED = 0,
    MZ_OUT,
    MZ_KO,
    MZ_RESERVE,
    MZ_FIELD
  };

/*!
** @brief Game map (on the left).
** @ingroup tbt_2d_gui
**
** It includes field and dugout.
*/
class Map : public VirtualScrollableSurface
{
public:
  Map(Game& g);
  virtual ~Map();

  //! @brief Check if the mouse is inside the visible game stadium.
  //! @return @c true if the mouse is inside game stadium.
  bool mouseInsideMap() const;
  //! @brief Check if the mouse is inside the visible game field.
  //! @return @c true if the mouse is inside one game square.
  bool mouseInsideField() const;
  //! @brief Convert mouse coordinate to square coordinate.
  //! @return Square coordinate (between 0 and ROWS/COLS).
  Point mouseToSquare() const;
  //! @brief Convert a square position to a map position, where to place Surface.
  //! @param pt A square coordinate (between 0 and ROWS/COLS).
  //! @param adjust Map position adjustement, when you want to center object on a square.
  Point squareToMap(const Point& pt, const Point& adjust = Point(0, 0)) const;

  void setMarker(const Point& square, int type);
  void removeMarker();
  
  void setBallPos(const Point& pos);
  void removeBall();

  /*!
  ** @brief Add a VisuPlayer to the map.
  **
  ** VisuPlayer will be added to the map and placed in the reserve.
  ** Use this instead of addChild, because map handles player position in dugouts.
  **
  ** @param vp VisuPlayer to add to the map.
  */
  void addPlayer(VisuPlayer* vp);
  /*!
  ** @brief Move a VisuPlayer from a zone to another, if needed.
  **
  ** @param old_status Old player status. It determines the zone he comes from.
  ** @param new_status New player status. It determines the zone he goes to.
  ** @param team_id Team id of the player.
  ** @param player_id Player id.
  */
  void movePlayer(enum eStatus old_status, enum eStatus new_status, int team_id, int player_id);

  bool getDrawTicks() const;
  void setDrawTicks(bool enable);
  
  virtual void update();

private:
  //! @brief Determine the map zone according to a player status.
  enum eMapZone statusToZone(enum eStatus status);
  //! @brief Move a player from a zone to another.
  void movePlayer(enum eMapZone from, enum eMapZone to, int team_id, int player_id);
  //! @brief Scroll the dugout to the given page index.
  void scrollDugout(enum eMapZone dugout, int team_id, unsigned int page_index);
  //! @brief Update a dugout after a player has been added or removed.
  void updateDugout(enum eMapZone dugout, int team_id, int player_id);

  //! @brief Scroll all dugouts forth or back.
  void scrollDugouts(bool forth);

  //! @brief Draw markers around squares.
  void drawTicks();

  Game&         g_;
  
  Surface       bg_;
  Surface       reserve_bg_;
  Sprite        ball_;
  Sprite        cross_black_;
  Sprite        cross_red_;

  bool          draw_ticks_;

  Surface       red_highlight_;
  Surface       blue_highlight_;

  unsigned int  dugouts_page_index_[4][2];
  VisuPlayer*   players_by_zone_[5][2][MAX_PLAYER];

  class DugoutsCallback : public GameButtonCallback
  {
    public:
    DugoutsCallback(Map* map, bool forth)
      : map_(map), forth_(forth) {}
    private:
      virtual void clicked();
      Map* map_;
      bool forth_;
  };
  GameButton    dugouts_back_, dugouts_forth_;
};

END_NS(sdlvisu);

#endif /* !GAMEFIELD_HH_ */
