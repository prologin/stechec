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

require 'ftools'

class Carte
  def initialize(parent)
    frame = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
      pack('side' => 'right',
           'padx' => 5,
           'pady' => 5,
           'fill' => 'both',
           'expand' => 'yes')
    }
    TkLabel.new(frame) do
      text 'Choisissez votre carte: '
      pack('side' => 'top',
           'fill' => 'x')
    end

    x = TkFrame.new(frame) {
      pack('side' => 'top',
           'fill' => 'both',
           'expand' => 'true')
    }

    scrollbar = TkScrollbar.new(x) {
      command proc { |*args| listview.yview *args }
      pack('side' => 'left',
           'fill' => 'y',
	   :padx => 3,
	   :pady => 3)
    }

    @listview = TkListbox.new(x) {
      selectmode 'single'
      pack('side' => 'right',
           'fill' => 'both',
           'expand' => 'true',
	   :padx => 3,
	   :pady => 3)
    }
    @listview.yscrollbar(scrollbar)


    # add the refresh and upload button
    f = TkFrame.new(frame) {
      pack('side' => 'bottom',
           'fill' => 'x',
	   :padx => 3,
	   :pady => 3)
    }

    TkButton.new(f) {
      text 'Rafraichir'
      pack('side' => 'left',
           'fill' => 'x',
           'expand' => 'true')
    }.command proc { fill_list }

    TkButton.new(f) {
      text 'Ajouter'
      pack('side' => 'right',
           'fill' => 'x',
           'expand' => 'true' )
    }.command proc { upload_map }

    fill_list
  end

  def get_map_dir
    map_shared_dir = ENV[$conf.game_contest.upcase + '_SHARED_MAP']
    if not map_shared_dir or not File.stat(map_shared_dir).directory?
      map_shared_dir = PKGDATADIR + "/" + "prolo2005" # FIXME: !!!
    end
    return map_shared_dir
  end

  def upload_map
    filetypes = [["Fichier carte", "*.map"]]
    localfile = Tk.getOpenFile('filetypes' => filetypes,
                              'parent' => $root)
    return if localfile == ""
    basefile = File.basename(localfile)
    distfile = get_map_dir() + "/" + basefile

    if File.exist?(distfile) and not File.stat(distfile).owned?
      Tk.messageBox(
        'icon' => 'error', 
        'type' => 'ok',
        'title' => 'Permission denied',
        'parent' => $root,
        'message' => 'Desole, le fichier \'' + basefile + '\' ne vous appartient pas, ' \
                    "vous ne pouvez pas le supprimer.\n\nRenommez votre fichier avant de l\'ajouter.")
    else
      File.copy(localfile, distfile)
    end
    fill_list
  end

  def fill_list
    # Read directory, retrieve map files
    # and try to select the current one.
    @listview.delete(0, @listview.size)
    oldpwd = Dir.pwd
    Dir.chdir(get_map_dir())
    Dir['*.map'].each { |x|
      @listview.insert('end', x)
      if x == $conf.game_map
        @listview.selection_set(@listview.size - 1)
      end
    }
    Dir.chdir(oldpwd)

  end

  def save_config
    i = @listview.curselection()
    if i != []
      $conf.game_map = "#{@listview.get(i)}"
    end
  end

end
