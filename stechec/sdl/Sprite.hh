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

/*!
** @file sdl/Sprite.hh
** @ingroup sdl_base
*/

# include "Surface.hh"

/*!
** @ingroup sdl_base
** @brief Enhancement of Surface, give life to object.
**
** Have the same behavior of Surface, but have some more features:
**   * can be animated
**   * can be moved from one position to another
**   * can show a part of a larger picture (useful when a picture consists of
**      several 'state')
**
** Here a simple example:
** @code
** Sprite s("general/foo.png");
** s.setPos(50, 100);      // same thing as Surface.
** s.setZ(3);              // idem.
** s.splitNbFrame(4, 1);   // 4 'frames' in the row.
** s.anim(250);            // show one of these frames every 250ms.
** s.move(800, 100, 30.);  // move the sprite
** win.getScreen().addChild(&s); // don't forget that!
** @endcode
*/
class Sprite : public Surface
{
public:
  //! @brief Constructor for a empty sprite.
  Sprite();
  //! @brief Construct a Sprite from an existing SDL surface.
  //! @param surf The surface to 'swallow'.
  //! @param zoom Zoom value to apply.
  //! @param angle Rotate picture, range: [0 - 360].
  Sprite(SDL_Surface* surf, double zoom = 1., double angle = 0.);
  //! @brief Construct a Sprite from a picture file.
  //! @param filename The filename to take the picture from.
  //! @param zoom Zoom value to apply.
  //! @param angle Rotate picture, range: [0 - 360].
  Sprite(const std::string filename, double zoom = 1., double angle = 0.);
  virtual ~Sprite();

  //! @brief Split the current surface into 'multiple' surfaces. To be used when
  //!  the input image have itself several images, each one having the same size, and
  //!  you want to display only one at a time.
  //! @param nb_frame_width The number of images in a row.
  //! @param nb_frame_height The number of images in a column.
  //! @note A picture must be loaded before.
  void splitNbFrame(int nb_frame_width, int nb_frame_height);
  //! @brief Split the current surface into 'multiple' surfaces. To be used when
  //!  the input image have itself several images, each one having the same size, and
  //!  you want to display only one at a time.
  //! @param size_frame_width The size of one 'sub' image in pixel on a row.
  //! @param size_frame_height The size of one 'sub' image in pixel on a column.
  //! @note A picture must be loaded before.
  void splitSizeFrame(int size_frame_width, int size_frame_height);

  //! @brief Move the sprite from the current position to the specified position,
  //!   at a specified speed. This create an animation.
  //! @param to Position to move to.
  //! @param velocity Speed at which this sprite will move (try value between 10. and 100.).
  void move(const Point& to, double velocity);
  //! @brief Move the sprite from the current position to the specified position,
  //!   at a specified speed. This create an animation.
  //! @param to_x X coordinate to move to.
  //! @param to_y T coordinate to move to.
  //! @param velocity Speed at which this sprite will move (try value between 10. and 100.).
  void move(int to_x, int to_y, double velocity);

  //! @brief Show all the frames successively, from the first to the last.
  //! @param delay Wait this time before going to next frame (in ms).
  //! @param loop_forever When arrived at the last frame, go to the first and continue.
  //! @note Before using it, you have to split the Sprite into frames, with splitNbFrame
  //!   or splitSizeFrame.
  void anim(int delay, bool loop_forever = true);
  //! @brief Stop animation, created previously by anim.
  void stopAnim();

  //! @brief Show the specified frame.
  //! @param frame The frame to show.
  //! @note Before using it, you have to split the Sprite into frames, with splitNbFrame
  //!   or splitSizeFrame.
  //! @note Not compatible with @c anim method.
  void setFrame(int frame);
  //! @brief Modify sprite transparency. It modify the surface alpha
  //!    channel with the specified level.
  //! @param level Transparency level, between 0 (opaque) to 255 (completely transparent).
  //! @note This function is in beta-test! Do not expect too much from it.
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
