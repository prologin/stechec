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

#include <SDL_ttf.h>

#include "SDLWindow.hh"

SDLWindow::SDLWindow(xml::XMLConfig* xml)
  : xml_(xml),
    is_fullscreen_(false)
{
  frame_drawed_ = 0;
}

Input& SDLWindow::getInput()
{
  return input_;
}

VirtualSurface& SDLWindow::getScreen()
{
  return screen_;
}

SDLWindow::~SDLWindow()
{
  // FIXME: close SDL.
}

void SDLWindow::setFullscreen(bool enable)
{
  LOG3("Switch to fullscreen mode: " << enable);
  is_fullscreen_ = enable;
  // FIXME: todo.
}

void SDLWindow::init()
{
  // FIXME: get fullscreen from xml, ...

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    PRINT_AND_THROW(SDLError, "Error initializing SDL");

  if (TTF_Init() < 0)
    PRINT_AND_THROW(TTFError, "Error initializing TTF");

  SDL_Surface* screen;
  screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
  if (screen == NULL)
    PRINT_AND_THROW(SDLError, "Unable to activate graphic mode");
  screen_ = VirtualSurface("Screen", screen);

  SDL_WM_SetCaption("TBT Game", NULL);
  SDL_WM_GrabInput(SDL_GRAB_OFF);

  frame_last_tick_ = SDL_GetTicks();
}

bool SDLWindow::processOneFrame()
{
  // Process SDL events
  input_.reset();
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
        {
        case SDL_QUIT:
          return true;
        }
      input_.update(event);
    }
  
  for (int i = 0; i < 255; i++)
    if (input_.key_pressed_[i])
      LOG1("PRESSED " << i);

  // Update and render.
  screen_.update();
  screen_.render();

  // FIXME: stick to a certain number of fps.

  frame_drawed_++;
  if (SDL_GetTicks() >= frame_last_tick_ + 1000)
    {
      frame_last_tick_ = SDL_GetTicks();
      LOG3("FPS: " << frame_drawed_);
      frame_drawed_ = 0;
    }
  
  return false;
}
