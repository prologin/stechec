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

class Champion
  def initialize(parent)
    frame = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
      pack('side' => 'top',
      	   :padx => 5,
	   :pady => 5)
    }

    chpframe = TkFrame.new(frame).grid(:columnspan => 2, :sticky => "ew")

    TkLabel.new(chpframe) {
      text 'Champion : '
      pack('side' => 'left', :expand => 'true', :fill => 'x', :anchor => 'w')
    }

    @champion_name = TkLabel.new(chpframe) {
      text ''
      pack('side' => 'left', :expand => 'true', :fill => 'x', :anchor => 'w')
    }

    TkButton.new(chpframe) {
      text '...'
      relief 'ridge'
      width 1
      height 1
      pack('side' => 'right')
    }.command { selectChampion }

    @timelimit_var = TkVariable.new
    @timeextra_var = TkVariable.new
    @memlimit_var = TkVariable.new
    createOption(frame, 'Limite de temps (ms) : ', @timelimit_var)
    createOption(frame, 'Temps additionnel (ms) : ', @timeextra_var)
    createOption(frame, 'Limite de memoire (ko) : ', @memlimit_var)

    @verbose_var = TkVariable.new
    mb = TkMenuButton.new(frame, :textvariable => @verbose_var,
       	 			 :indicatoron => 1)
    vmenu = TkMenu.new(mb, :tearoff => 0)
    vmenu.add(:radiobutton, :label => '0', :variable => @verbose_var)
    vmenu.add(:radiobutton, :label => '1', :variable => @verbose_var)
    vmenu.add(:radiobutton, :label => '2', :variable => @verbose_var)
    mb.menu(vmenu)
    l = TkLabel.new(frame, :text => 'Bavardage :')
    TkGrid.grid(l, mb, :sticky => 'w')

    # checkboxes for gdb and valgrind
    @gdb = TkVariable.new
    @valgrind = TkVariable.new

    # add checkbox for gdb
    gdbchk = TkCheckButton.new(frame) {
      text 'Utiliser GDB'
    }.grid(:sticky => 'w')
    gdbchk.variable @gdb
    gdbchk.command {
      @valgrind.value = "0"
    }

    # add checkbox for valgrind
    valchk = TkCheckButton.new(frame) {
      text 'Utiliser Valgrind'
    }.grid(:sticky => 'w')
    valchk.variable @valgrind
    valchk.command {
      @gdb.value = "0"
     }

  end

  def selectChampion
    filetypes = [["Bibliotheque champion", "*.so"]]
    localfile = Tk.getOpenFile('filetypes' => filetypes,
                              'parent' => $root)
    return if localfile == ""
    $conf.cl_champion = "#{localfile}"
    @champion_name.text = File.basename($conf.cl_champion)
  end

  def save_config
    $conf.cl_time_limit = @timelimit_var
    $conf.cl_time_reserve = @timeextra_var
    $conf.cl_memory_limit = @memlimit_var
    $conf.cl_verbose = @verbose_var
    $conf.cl_use_gdb = @gdb.value == "1"
    $conf.cl_use_valgrind = @valgrind.value == "1"
  end

  def load_config
    if $conf.cl_use_valgrind
      @valgrind.value = "1"
    end
    if $conf.cl_use_gdb
      @gdb.value = "1"
    end
    @timelimit_var.value = $conf.cl_time_limit
    @timeextra_var.value = $conf.cl_time_reserve
    @memlimit_var.value = $conf.cl_memory_limit
    @verbose_var.value = $conf.cl_verbose
    begin
      @champion_name.configure('text' => File.basename($conf.cl_champion))
    rescue
      @champion_name.configure('text' => '')
    end
  end
end
