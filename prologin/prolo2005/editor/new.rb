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
#  Copyright (C) 2005 Prologin
#


class NewMap
  def initialize(root, map, opt)

    #
    # Creation de carte
    #
    w = TkToplevel.new(:title => 'Nouvelle carte')

    var_map_x = TkVariable.new(20)
    var_map_y = TkVariable.new(10)
    var_nb_player = TkVariable.new(2)

    tab = [
      { 'label' => 'taille carte x :', 'var' => var_map_x },
      { 'label' => 'taille carte y :', 'var' => var_map_y },
      { 'label' => 'nombre joueurs :', 'var' => var_nb_player}
    ]

    tab.each { |x|
      label = TkLabel.new(w, :text => x['label'])
      input = TkEntry.new(w,
                          :textvariable => x['var'],
                          :width => 7,
                          :justify => 'right')
      TkGrid.grid(label, input)
    }

    b = TkButton.new(w, :text => 'Créer').grid(:columnspan => 2)
    b.command proc {

      # cree les options et l'editeur de carte sur la fenetre principale
      $w = var_map_x.value.to_i
      $h = var_map_y.value.to_i
      $nb_player = var_nb_player.value.to_i

      map.cleanMap
      map.loadConfig
      opt.loadConfig

      w.destroy
    }

  end
end
