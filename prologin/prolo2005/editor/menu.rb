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

class Menu
  def initialize(m, o)
    @carte = m
    @option = o

    #
    # Creation du menu
    #
    bar = TkMenu.new($root)
    
    # Menu fichier
    file = TkMenu.new(bar, :tearoff => 0)
    
    if not $restricted
      file.add('command',
               'label' => 'Ouvrir',
               'command' => proc { openMapFile },
               'underline' => 0,
               'accel' => 'Ctrl+O')
      $root.bind('Control-o', proc { openMapFile })
      
      file.add('command',
               'label' => 'Sauvegarder',
               'command' => proc { saveMapFile },
               'underline' => 0,
               'accel' => 'Ctrl+S')
      $root.bind('Control-s', proc { saveMapFile })
      
      file.add('command',
               'label' => 'Sauvegarder sous',
               'command' => proc { saveMapFileAs },
               'underline' => 0,
               'accel' => 'Ctrl+Shift+S')
      $root.bind('Shift-Control-s', proc { saveMapFileAs })
    end

    file.add('command',
             'label' => 'Quitter',
             'command' => proc { exit },
             'underline' => 0,
             'accel' => 'Ctrl+Q')
    $root.bind('Control-q', proc { exit })
    
    # Menu carte
    mapMenu = TkMenu.new(bar, :tearoff => 0)
    mapMenu.add('command',
                'label' => 'Nouvelle carte',
                'accel' => 'Ctrl+N',
                'command' => proc { newMap })
    $root.bind('Control-n', proc { newMap })

    mapMenu.add('command',
                'label' => 'Tout effacer',
                'accel' => 'Ctrl+C',
                'command' => proc { @carte.clearAll })
    $root.bind('Control-c', proc { @carte.clearAll })

    # Menu about
    helpMenu = TkMenu.new(bar, :tearoff => 0)
    helpMenu.add('command',
                 'label' => 'A propos',
                 'command' => proc { showAboutBox })

    bar.add('cascade', 'menu' => file, 'label' => 'Fichier')
    bar.add('cascade', 'menu' => mapMenu, 'label' => 'Carte')
    bar.add('cascade', 'menu' => helpMenu, 'label' => 'A propos')
    $root.menu(bar)

  end

  def openMapFile
    filetypes = [["Fichier carte", "*.map"], ["All Files", "*"]]
    filename = Tk.getOpenFile('filetypes' => filetypes,
                              'parent' => $root)
    return if filename == ""
    $map_file = filename
    @carte.cleanMap
    @carte.loadMap
    @carte.loadConfig
    @option.loadConfig
    puts "map file " + filename + " loaded"      
  end

  def saveMapFileAs
    $map_file = "<default>"
    saveMapFile
  end

  def saveMapFile
    if $map_file == "<default>"
      filetypes = [["Map file", "*.map"], ["All Files", "*"]]
      filename = Tk.getSaveFile('filetypes' => filetypes,
                                'parent' => $root)
      return if filename == ""
      $map_file = filename
      @option.set_map_file
    end
    puts "map saved in " + $map_file if @carte.saveMap($map_file)
  end

  def newMap
    NewMap.new($root, @carte, @option)
  end

  def showAboutBox
    Tk.messageBox('icon' => 'info', 'type' => 'ok',
                  'title' => 'Boite inutile',
                  'parent' => $root,
                  'message' => 'Merci de ne pas nourrir le chimpanzé qui a codé cette application.')
  end

end
