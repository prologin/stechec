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
#include <SDL_image.h>

#include "SDLWindow.hh"

SDLWindow::SDLWindow(xml::XMLConfig* xml)
  : xml_(xml),
    is_fullscreen_(false),
    frame_drawed_(0),
    fps_(0)
{
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
  TTF_Quit();
  SDL_Quit();
}

bool SDLWindow::isFullScreen() const
{
  return is_fullscreen_;
}

void SDLWindow::setFullScreen(bool enable)
{
  if (is_fullscreen_ != enable)
    {
      LOG3("Switch to fullscreen mode: " << enable);
      is_fullscreen_ = enable;
      SDL_WM_ToggleFullScreen(screen_.getSDLSurface());
    }
}

int SDLWindow::getFps() const
{
  return fps_;
}


void SDLWindow::init()
{
  // FIXME: get window size, fullscreen mode, ... from xml.

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    PRINT_AND_THROW(SDLError, "Error initializing SDL");

  if (TTF_Init() < 0)
    PRINT_AND_THROW(TTFError, "Error initializing TTF");

  SDL_Surface* icon = IMG_Load(PKGDATADIR "/image/general/tbt.ico");
  if (icon != NULL)
    {
      SDL_WM_SetIcon(icon, NULL);
      SDL_FreeSurface(icon);
    }
  else
    WARN("Can't set window's icon: " << SDL_GetError());
  
  SDL_Surface* screen;
  screen = SDL_SetVideoMode(800, 600, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
  if (screen == NULL)
    PRINT_AND_THROW(SDLError, "Unable to activate graphic mode");
  screen_ = VirtualSurface("Screen", screen);

  SDL_WM_SetCaption("TBT Game", NULL);
  SDL_WM_GrabInput(SDL_GRAB_OFF);

  // Enables Unicode keyboard translation.
  SDL_EnableUNICODE(1);

  frame_previous_tick_ = frame_tick_fps_ = SDL_GetTicks();
}

void SDLWindow::clearScreen()
{
  SDL_FillRect(screen_.getSDLSurface(), NULL, 0);
  screen_.invalidate(screen_.getRect());
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

  // Update and render.
  screen_.update();
  screen_.render();

  // Count FPS.
  frame_drawed_++;
  const unsigned now = SDL_GetTicks();
  if (now >= frame_tick_fps_ + 1000)
    {
      frame_tick_fps_ = now;
      fps_ = frame_drawed_;
      LOG5("FPS: " << frame_drawed_);
      frame_drawed_ = 0;
    }

  // Wait a little if we have rendered too fast - 50 FPS max.
  if (now < frame_previous_tick_ + 20)
    SDL_Delay(20 - (now - frame_previous_tick_));
  frame_previous_tick_ = now;

  return false;
}
