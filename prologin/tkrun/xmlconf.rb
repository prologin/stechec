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

require "rexml/document"
require 'pathname'
require 'conf'
include REXML

class XMLConf

  def open(filename)
    @filename = filename
    begin
      File.open(filename, "r") do |f|
        load f
      end
    rescue
      puts "Could not load '#{filename}' ! Aborting now."
      exit 1
    end
  end

  def load(xml_doc)
    @doc = Document.new(xml_doc)

    @client_sec = {}
    @doc.elements["config"].each_element do |e|
      case e.name
      when "game"
        @game_sec = e;
      when "client"
        @client_gen_sec = e;
      when /client_(DISABLED_)?(\d+)/
        index = $2.to_i
        @client_sec[index] = e;
        # disable all section but the first.
        # special case for the second.
        if index == 2
          @use_gui = (e.name =~ /DISABLED/) == nil
        elsif index != 1
          e.name = "client_DISABLED_#{index}"
        end
      when "server"
        @server_sec = e;
      end
    end
    puts "Configuration file '#@filename' loaded." if @filename
    puts "Default configuration loaded." if not @filename
  end

  attr_reader :filename

  def save()
    return if not @filename
    File.open(@filename, "w") do |f|
      @doc.write(f, 0)
    end
    puts "Configuration file saved to #@filename."
  end

  def filename=(s)
    @filename = s
    @filename = nil if s == ""
  end

  def use_gui=(b)
    disable = "DISABLED_" if not b
    @client_sec[2].name = "client_#{disable}2"
  end

  def use_gui()
    (@client_sec[2].name =~ /DISABLED/) == nil
  end

  def game_contest()
    @game_sec.elements["rules"].text
  end

  def game_map()
    @game_sec.elements["map"].text
  end

  def game_map=(s)
    @game_sec.elements["map"].text = s
  end

  def srv_max_turn()
    @game_sec.elements["max_turn"].text
  end

  def srv_max_turn=(i)
    @game_sec.elements["max_turn"].text = i
  end

  def srv_verbose()
    @server_sec.elements["debug"].attributes["verbose"]
  end
  
  def srv_verbose=(v)
    @server_sec.elements["debug"].attributes["verbose"] = v
  end

  def cl_use_valgrind()
    @client_sec[1].elements["debug"].attributes["valgrind"]
  end

  def cl_use_valgrind=(b)
    @client_sec[1].elements["debug"].attributes["valgrind"] = b
  end

  def cl_use_gdb()
    @client_sec[1].elements["debug"].attributes["gdb"]
  end

  def cl_use_gdb=(b)
    @client_sec[1].elements["debug"].attributes["gdb"] = b
  end

  def cl_time_limit()
    @client_sec[1].elements["limit"].attributes["time"]
  end

  def cl_time_limit=(i)
    @client_sec[1].elements["limit"].attributes["time"] = i
  end

  def cl_time_reserve()
    @client_sec[1].elements["limit"].attributes["time_reserve"]
  end

  def cl_time_reserve=(i)
    @client_sec[1].elements["limit"].attributes["time_reserve"] = i
  end

  def cl_memory_limit()
    @client_sec[1].elements["limit"].attributes["memory"]
  end

  def cl_memory_limit=(i)
    @client_sec[1].elements["limit"].attributes["memory"] = i
  end

  def cl_verbose()
    @client_sec[1].elements["debug"].attributes["verbose"]
  end

  def cl_verbose=(i)
    @client_sec[1].elements["debug"].attributes["verbose"] = i
  end

  def cl_champion()
    @client_sec[1].elements["champion"].text
  end

  def cl_champion=(s)
    @client_sec[1].elements["champion"].text = s
  end

end
