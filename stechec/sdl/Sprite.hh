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

#ifndef SPRITE_HH_
# define SPRITE_HH_

# include "Surface.hh"

/*!
** @brief Enhancement of Surface, give life to object.
** @ingroup sdl_base
**
** Have the same behavior of Surface, but can be animated, move, ...
*/
class Sprite : public Surface
{
public:
  Sprite();
  Sprite(SDL_Surface* surf, double zoom = 1., double angle = 0.);
  Sprite(const std::string filename, double zoom = 1., double angle = 0.);
  virtual ~Sprite();

  void splitNbFrame(int nb_frame_width, int nb_frame_height);
  void splitSizeFrame(int size_frame_width, int size_frame_height);

  void move(const Point& to, double velocity);
  void move(int to_x, int to_y, double velocity);
  void anim(int delay, bool loop_forever = true);
  void stopAnim();
  void setFrame(int frame);
  void setTransparency(int level);

  virtual void setZoom(double zoom);

  virtual void update();

  virtual void blit(Surface& to);
  virtual void blit(Surface& to, const Rect& to_rect, const Rect& from_rect);

private:
  // Animation
  int nb_anim_width_;
  int nb_anim_height_;
  int current_anim_;
  unsigned last_anim_updated_;
  int anim_delay_;
  bool anim_loop_;
  bool is_animing_;

  // Moving
  Point move_from_;
  Point move_to_;
  DPoint current_pos_;    ///< Position in 'double' precision.
  unsigned last_updated_;
  bool is_moving_;
  double velocity_;
};

#endif /* !SPRITE_HH_ */
