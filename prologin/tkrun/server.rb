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

class Server
  def initialize(parent)
    frame = TkFrame.new(parent) {
      borderwidth  4
      relief 'groove'
      pack('side' => 'top',
           'padx' => 5,
           'pady' => 5)
    }

    @nbturn_var = TkVariable.new
    createOption(frame, 'Nombre de tour : ', @nbturn_var)
    
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

    # add checkbox for visu
    @visu_var = TkVariable.new
    TkCheckButton.new(frame, :text => 'Lancer l\'interface graphique',
    			     :variable => @visu_var).grid(:sticky => 'w',
                                                          :columnspan => 2)

  end

  def save_config
    $conf.srv_verbose = @verbose_var.value
    $conf.srv_max_turn = @nbturn_var.value
    $conf.use_gui = @visu_var.value == "1"
  end

  def load_config
    @verbose_var.value = $conf.srv_verbose
    @nbturn_var.value = $conf.srv_max_turn
    @visu_var.value = $conf.use_gui
  end
end
