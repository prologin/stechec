#
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2005, 2006 Prologin
#

class Interface

  def initialize
    $root = TkRoot.new { title "Interface Lancement Prologin 2006" }

    #
    # Creation du menu
    #
    bar = TkMenu.new($root)
    
    # Menu fichier
    file = TkMenu.new(bar, :tearoff => 0)

    file.add('command',
		'label' => 'Ouvrir',
                 'command' => proc { openConfFile },
                 'underline' => 0,
                 'accel' => 'Ctrl+O')
    $root.bind('Control-o', proc { openConfFile })
 
    file.add('command',
                 'label' => 'Sauvegarder',
                 'command' => proc { saveConfFile },
                 'underline' => 0,
                 'accel' => 'Ctrl+S')
    $root.bind('Control-s', proc { saveConfFile })

    file.add('command',
                 'label' => 'Sauvegarder sous',
                 'command' => proc { saveConfFileAs },
                 'underline' => 0,
		 'accel' => 'Ctrl+Shift+S')
#    $root.bind('Control-shift-s', proc { saveConfFileAs })

    file.add('command',
                 'label' => 'Quitter',
                 'command' => proc { exit },
                 'underline' => 0,
                 'accel' => 'Ctrl+Q')
    $root.bind('Control-q', proc { exit })

    # Menu about
    helpMenu = TkMenu.new(bar, :tearoff => 0)
    helpMenu.add('command',
                 'label' => 'A propos',
                 'command' => proc { showAboutBox })

    bar.add('cascade', 'menu' => file, 'label' => 'Fichier')
    bar.add('cascade', 'menu' => helpMenu, 'label' => 'A propos')
    $root.menu(bar)


    # Champion et serveur sur le cote gauche
    g = TkFrame.new($root) {
      pack('side' => 'left',
           'fill' => 'y',
           'pady' => 5,
           'padx' => 5)
      
    }

    # Creation du champion
    chp = TkFrame.new(g) { pack('side' => 'top') }
    TkLabel.new(chp) {
      text 'Configuration du champion'
      justify 'right'
      pack('side' => 'top')
    }
    @champion = Champion.new(chp)

    # Creation du serveur
    serv = TkFrame.new(g) { pack('side' => 'top') }
    TkLabel.new(serv) {
      text 'Configuration du serveur'
      justify 'right'
      pack('side' => 'top')
    }
    @server = Server.new(serv)

    @launchButton = TkButton.new(g) do
      text 'Lancer la partie'
      width 13
      pack('padx' => 5,
           'pady' => 2,
           'side' => 'bottom',
           'fill' => 'x')
    end.command { launchGame }

    # La map du cote droit
    @carte = Carte.new($root)

    # Charge les configs
    @champion.load_config
    @server.load_config
  end

  def launchGame
    saveConfFile
    return if not $conf.filename
    
    @launchButton.configure('text' => '*** PARTIE EN COURS ***',
                            'state' => 'disable')
    $root.update

    fork do
      exec "run #{$conf.filename}"
    end
    Process.wait
    @launchButton.configure('text' => 'Lancer la partie',
                            'state' => 'normal')
  end

  def saveConfFileAs
    $conf.filename = nil
    saveConfFile
  end

  def saveConfFile
    @carte.save_config
    @server.save_config
    @champion.save_config
    if not $conf.filename
      filetypes = [["Yaml Configuration File", "*.yml"], ["All Files", "*"]]
      filename = Tk.getSaveFile('filetypes' => filetypes,
                                'parent' => $root)
      $conf.filename = filename
    end
    $conf.save
  end

  def openConfFile
    filetypes = [["Yaml Configuration File", "*.yml"], ["All Files", "*"]]
    filename = Tk.getOpenFile('filetypes' => filetypes,
                              'parent' => $root)
    return if filename == ""
    $conf.open filename
    @carte.fill_list
    @server.load_config
    @champion.load_config
  end

  def showAboutBox
    Tk.messageBox('icon' => 'info', 'type' => 'ok',
                  'title' => 'Boite frisant l\'inutile',
                  'parent' => $root,
                  'message' => 'Copyright (C) 2005, 2006 Prologin.')
  end
  
end
