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

class Map
  def initialize(opt)
    @opt = opt

    parent = TkFrame.new($root) {
      borderwidth  4
      relief 'groove'
      pack(:fill => 'both',
           :expand => 'true',
           :padx => 5,
           :pady => 5)
    }

    @canvas = TkCanvas.new(parent, 
                          :xscrollincrement => 16,
                          :yscrollincrement => 16,
                          :width => 400, 
                          :height => 400)
    
    # Set scrollbars
    hbar = TkScrollbar.new(parent, :orient => 'hor').command proc { |*arg|
      @canvas.xview *arg
    }
    vbar = TkScrollbar.new(parent, :orient => 'ver').command proc { |*arg|
      @canvas.yview *arg
    }
    @canvas.configure(:xscrollcommand => proc { |*arg| hbar.set *arg},
                      :yscrollcommand => proc { |*arg| vbar.set *arg})
    
    TkGrid.grid(@canvas, vbar, 'sticky'=>'nsew')
    TkGrid.grid(hbar,         'sticky'=>'ew')
    
    TkGrid.columnconfigure(parent, 0, 'weight'=>1)
    TkGrid.rowconfigure(parent, 0, 'weight'=>1)
  end

  def new_plot(x, y, str)
    if @opt.get_pencil != "clear"
      i = TkcImage.new(@canvas, x, y, :image => @opt.get_bitmap)
      print "add plot ", @opt.get_pencil, " ", str, "\n"
      i.addtag(@opt.get_pencil)
      i.addtag(str)
      i.addtag('image')
    end
  end

  def delete_plot(str)
    # vire les elements deja presents sur cette case
    puts 'delete plot ' + str
    items = @canvas.find_withtag(str)
    items.each do |z|
      @canvas.delete z
    end
  end

  def add_plot(ex, ey)
    x = ex.to_i
    y = ey.to_i
    c = x.to_s + " " + y.to_s

    if @opt.get_pencil =~ /^userplc_/
      items = @canvas.find_withtag(@opt.get_pencil)
      items.each do |z|
        @canvas.delete z
      end
    end

    delete_plot(c)
    new_plot(x, y, c)
      
    # option mirroir vertical
    if @opt.get_pencil !~ /^userplc_/ and ($extra_opt.value == "1" or $extra_opt.value == "3")
      mx = (($h + 1) * 16) - x
      c = mx.to_s + " " + y.to_s
      delete_plot(c)
      new_plot(mx, y, c)
    end

    # option mirroir horizontal
    if @opt.get_pencil !~ /^userplc_/ and ($extra_opt.value == "2" or $extra_opt.value == "3")
      my = (($w + 1) * 16) - y
      c = x.to_s + " " + my.to_s
      delete_plot(c)
      new_plot(x, my, c)
    end
    
    # option mirroir double
    if @opt.get_pencil !~ /^userplc_/ and ($extra_opt.value == "3")
      c = mx.to_s + " " + my.to_s
      delete_plot(c)
      new_plot(mx, my, c)
    end
  end

  def redrawUserPlc
    # don't work
    items = @canvas.find_withtag('userplc_*')
    items.each do |x|
      p x
      @canvas.delete x
    end
  end
  
  def clearAll
    items = @canvas.find_withtag('image')
    items.each do |x|
      @canvas.delete x
    end
  end


  def saveMap(out_file)
    items = @canvas.find_withtag('image')
    map = Array.new($w)
    player = Array.new($nb_player, -1)
    for i in 0 .. $w - 1
      map[i] = Array.new($h, '.')
    end

    # recupere ce qui est place sur la carte
    items.each do |x|
      info = @canvas.itemconfiginfo(x)
      info_hash = {}
      info.each{|key| info_hash[ key[0] ] = key[4] if key[4] != "" }
      y = (info_hash['coords'][0] - 1).to_i / 16
      x = (info_hash['coords'][1] - 1).to_i / 16
      type = info_hash['tags'][0]
      if x >=0 and x < $w and y >= 0 and y < $h
        case type
        when 'plasma'
          map[x][y] = 'p'
        when 'water'
          map[x][y] = 'w'
        when 'ore'
          map[x][y] = 'o'
        when /^userplc_/
          index = type[8, 3].to_i - 1
          player[index] = [x + 1, y + 1]
        else
          puts type + ' unknown !!!!!'
        end
      end
    end

    for i in 0..$nb_player -1
      if player[i] == -1
        Tk.messageBox('icon' => 'error', 'type' => 'ok',
                      'title' => 'Erreur',
                      'parent' => $root,
                      'message' => "start position not specified for player : " + (i + 1).to_s)
        return false
      end
    end
    
    # la sauvegarde en elle meme...
    f = File.new(out_file, 'w')

    f.puts @opt.get_comment
    f << $nb_player << " " << $w << " " << $h << "\n"
    player.each { |a|
      f << a[0] << " " << a[1] << "\n"
    }
    
    for i in 0 .. $w - 1
      for j in 0 .. $h - 1
        f.putc map[i][j]
      end
      f.puts
    end
    f.close
    return true
  end

  def cleanMap
    items = @canvas.find_withtag('all')
    items.each do |x|
      @canvas.delete x
    end
  end

  def loadConfig
    @canvas.configure(:scrollregion => '0 0 ' + (($h + 1) * 16).to_s + ' ' + (($w + 1) * 16).to_s)

    # grille
    for i in 0 .. $h - 1
      for j in 0 .. $w - 1
        TkcRectangle.new(@canvas, i * 16 + 8, j * 16 + 8, (i+1) * 16 + 8, (j+1) * 16 + 8)
      end
    end
    @press = false
    @canvas.bind('Button-1') { |event|
      @press = true
      add_plot @canvas.canvasx(event.x, 16), @canvas.canvasy(event.y, 16)
    }
    @canvas.bind('B1-Motion') { |event| 
      add_plot @canvas.canvasx(event.x, 16), @canvas.canvasy(event.y, 16)
    }
    @canvas.bind('ButtonRelease') { 
      @press = false; 
    }

  end

  def loadMap
    f = File.new($map_file, 'r')

    $extra_opt.value = "0"

    @opt.set_comment f.gets.strip
    a = f.gets.scan(/\d+/)
    $nb_player = a[0].to_i
    $w = a[1].to_i
    $h = a[2].to_i

    for i in 1..$nb_player
      a = f.gets.scan(/\d+/)
      print "get plc ", a[0], " ", a[1], "\n"
      @opt.set_pencil 'userplc_' + i.to_s
      add_plot a[1].to_i * 16, a[0].to_i * 16
    end

    for i in 1 .. $w
      line = f.gets
      for j in 1 .. $h
        case line[j - 1, 1]
        when 'p'
          @opt.set_pencil 'plasma'
          add_plot j * 16, i * 16
        when 'w'
          @opt.set_pencil 'water'
          add_plot j * 16, i * 16
        when 'o'
          @opt.set_pencil 'ore'
          add_plot j * 16, i * 16
        end
      end
    end
    f.close
  end

end
