#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005, 2006 Prologin
#

require 'conf'

def load_image

    x = {
      :almanach => [:black],
      :building => [:blue],
      :casino => [:red],
      :delorean => [:red],
      :square => [:yellow],
      :street => [:black],
      :userplc => [:black, :full],
      :userplc_current => [:green, :full]
    }
    h={}
    x.each do |k, v|
      if v[1]
        file = v[1].to_s
      else
        file = k.to_s
      end
      h[k.to_s] = TkBitmapImage.new(
        :foreground => v[0].to_s, 
        :file => get_file_path(file + '.bitmap', PKGDATADIR + "/prolo2006")
      )
    end
    return h
end
