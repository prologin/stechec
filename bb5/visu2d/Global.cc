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

#include "Global.hh"

#include "tools.hh"

void PrintStrings(SDL_Surface *dest, TTF_Font *font, string str,
                  SDL_Rect &rc, SDL_Color textColor)
{

  int lineSkip = TTF_FontLineSkip(font);  // Get the line-skip value.
  int width = 0, height = 10;

  vector<string> vLines; // these are the individual lines.
  uint line_length = str.length();

      // Break the string into its lines:
  int n = 0;
  while (n != -1)
  {
        // Get until either '\n':
    string strSub; 
    n = str.find('\n', 0);
    strSub = str.substr(0, n);
    if (n != -1)
    {
      str = str.substr(n+1, line_length);
    }
    vLines.push_back(strSub);
    n = str.find('\n', 0); // Find the next '\n'
  }
      // Actually render the text:
  SDL_Surface *temp = NULL;

  for (ushort i = 0; i < vLines.size(); i++)
  {
        // The rendered text:
    string tmpstr(vLines[i]);
    TTF_SizeText(font, tmpstr.c_str(), &width, &height);

        // Get the height : (vLines.size() == Number of Lines)
    height = (vLines.size()-1) * lineSkip + height; 
    temp = TTF_RenderText_Solid(font, tmpstr.c_str(), textColor);
    if (temp == NULL)
      WARN("FAILED to render text : %1", TTF_GetError());
    
    SDL_Rect r = {rc.x + (rc.w - width) / 2, rc.y + (rc.h-height) / 2 + i * lineSkip, rc.w, rc.h};

    SDL_BlitSurface(temp, NULL, dest, &r);
        // Clean up:
    SDL_FreeSurface(temp);
  }
}
