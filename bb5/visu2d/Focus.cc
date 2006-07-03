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

 #include "Focus.hh"
 #include <iostream>

 Focus::Focus(Widget* widget)
 {
     if(focus == NULL)
     {
         focus = widget;
     }
 }

 Widget* Focus::get_focus()
 {
     return focus;
 }

 void Focus::set_focus(Widget* widget)
 {
     if(focus != NULL)
     {
         focus->losefocus();
     }
     focus = widget;

     if(focus != NULL)
     {
         focus->gainfocus();
     }
 }
