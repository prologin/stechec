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

class Option
  def map=(m)
    @map = m
  end

  def initialize

    parent = TkFrame.new($root) {
      pack(:side => 'left')
    }

    require 'image_path'
    @bitmap = load_image

    #
    # Info carte
    #
    @fInfo = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
    }

    TkLabel.new(@fInfo, :text => 'Info Carte').grid

    @lbl_file = TkLabel.new(@fInfo, :text => 'fichier : <new>')
    @lbl_map_x = TkLabel.new(@fInfo, :text => 'taille x : ')
    @lbl_map_y = TkLabel.new(@fInfo, :text => 'taille y : ')
    @lbl_nb_player = TkLabel.new(@fInfo, :text => 'max joueurs : ')
    @inp_comment = TkVariable.new('commentaires...')
    input = TkEntry.new(@fInfo, :textvariable => @inp_comment)

    [@lbl_file, @lbl_map_x, @lbl_map_y, @lbl_nb_player, input].each { |l|
      l.grid(:sticky => 'w')
    }

    #
    # Placement terrain
    #
    @fMap = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
    }

    TkLabel.new(@fMap,
                :text => 'Terrain') {
      grid
    }

    @pencil = TkVariable.new('square')
    TkRadioButton.new(@fMap,
                      :value => 'clear',
                      :text => 'nettoyer',
                      :variable => @pencil) {
      grid(:sticky => 'w')
    }

    but = TkRadioButton.new(@fMap,
                            :value => 'square',
                            :text => 'terrain vague',
                            :variable => @pencil)
    canv = TkCanvas.new(@fMap, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['square'])
    TkGrid.grid(but, canv, :sticky => 'w')


    but = TkRadioButton.new(@fMap,
                            :value => 'street',
                            :text => 'rue',
                            :variable => @pencil)
    canv = TkCanvas.new(@fMap, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['street'])
    TkGrid.grid(but, canv, :sticky => 'w')
    
    but = TkRadioButton.new(@fMap,
                            :value => 'building',
                            :text => 'batiment',
                            :variable => @pencil)
    canv = TkCanvas.new(@fMap, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['building'])
    TkGrid.grid(but, canv, :sticky => 'w')

    but = TkRadioButton.new(@fMap,
                            :value => 'casino',
                            :text => 'casino',
                            :variable => @pencil)
    canv = TkCanvas.new(@fMap, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['casino'])
    TkGrid.grid(but, canv, :sticky => 'w')


    #
    # Placement des objets
    #
    @fObject = TkFrame.new(parent) {
      borderwidth 4
      relief 'groove'
    }

    TkLabel.new(@fObject,
                :text => 'Objets') {
      grid(:columnspan => 2)
    }

    but = TkRadioButton.new(@fObject,
                            :value => 'delorean',
                            :text => 'delorean',
                            :variable => @pencil)
    canv = TkCanvas.new(@fObject, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['delorean'])
    TkGrid.grid(but, canv, :sticky => 'w')

    but = TkRadioButton.new(@fObject,
                            :value => 'almanach',
                            :text => 'almanach',
                            :variable => @pencil)
    canv = TkCanvas.new(@fObject, :borderwidth => 0, :width => 16, :height => 16)
    TkcImage.new(canv, 8, 8, :image => @bitmap['almanach'])
    TkGrid.grid(but, canv, :sticky => 'w')


    #
    # Placement joueurs
    #
    @fPlayer = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
    }

    TkLabel.new(@fPlayer,
                :text => 'Joueurs') {
      grid(:columnspan => 2)
    }

    @player_num = TkVariable.new(1)
    mb = TkMenubutton.new(@fPlayer, 
                          :textvariable => @player_num,
                          :indicatoron => 1)
    @vmenu = TkMenu.new(mb, :tearoff => 0)
    mb.menu(@vmenu)

    TkGrid.grid(TkLabel.new(@fPlayer,
                            :text => 'numéro: '),
                mb,
                :sticky => 'w')

    TkRadioButton.new(@fPlayer,
                      :value => 'userplc',
                      :text => 'départ joueur',
                      :variable => @pencil) {
      grid(:sticky => 'w', :columnspan => 2)
    }

    #
    # Special options
    #
    @fExtra = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
    }

    TkLabel.new(@fExtra,
                :text => 'Options séciales') {
      grid
    }

    $extra_opt = TkVariable.new(0)
    TkRadioButton.new(@fExtra,
                      :value => 0,
                      :text => 'aucune',
                      :variable => $extra_opt) {
      grid(:sticky => 'w')
    }

    TkRadioButton.new(@fExtra,
                      :value => 1,
                      :text => 'mirroir vertical',
                      :variable => $extra_opt) {
      grid(:sticky => 'w')
    }

    TkRadioButton.new(@fExtra,
                      :value => 2,
                      :text => 'mirroir horizontal',
                      :variable => $extra_opt) {
      grid(:sticky => 'w')
    }


    TkRadioButton.new(@fExtra,
                      :value => 3,
                      :text => 'double mirroir',
                      :variable => $extra_opt) {
      grid(:sticky => 'w')
    }

  end

  def get_bitmap(player_id = -1)
    return @bitmap[@pencil.value] unless @pencil.value =~ /^userplc/
    return @bitmap['userplc_current'] if player_id == @player_num.value.to_i
    return @bitmap['userplc']
  end

  def get_pencil
    return @pencil.value unless @pencil.value == "userplc"
    return 'userplc_' + @player_num.value.to_s
  end

  def set_pencil(str)
    @pencil.value = str
  end

  def get_comment
    return @inp_comment.value.to_s
  end
  
  def set_map_file
    @lbl_file.configure(:text => 'fichier : ' + File.basename($map_file))
  end

  def set_comment(s)
    @inp_comment.value = s
  end

  def loadConfig
    @lbl_file.configure(:text => 'fichier : ' + File.basename($map_file))
    @lbl_map_x.configure(:text => 'taille x : ' + $w.to_s)
    @lbl_map_y.configure(:text => 'taille y : ' + $h.to_s)
    @lbl_nb_player.configure(:text => 'max joueurs : ' + $nb_player.to_s)

    @vmenu.delete(0, 'end')
    for i in 1 .. $nb_player
      @vmenu.add(:radiobutton, 
                 :label => i.to_s, 
                 :variable => @player_num , 
                 :command => proc { @map.redrawUserPlc }
)
    end

    # montre toujours les infos
    @fInfo.pack('side' => 'top',
                :padx => 5,
                :pady => 5)

    # mode lecture seule
    if $restricted
      [@fMap, @fObject, @fPlayer, @fExtra].each { |x| x.unpack }
    else
      [@fMap, @fObject, @fPlayer, @fExtra].each { |x|
        x.pack('side' => 'top',
               :padx => 5,
               :pady => 5,
               :fill => 'x')
      }
    end
  end

end
