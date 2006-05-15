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

class PascalFileGenerator < FileGenerator
  def initialize
    @lang = "pascal"
    @types = {
      'void' => '',
      'int' => 'LongInt',
      'bool' => 'LongBool'
    }
  end

  def print_comment(str)
    @f.puts '{ ' + str + ' }' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '{'
    str.each {|s| @f.print "  ", s }
    @f.puts "\n}"
  end

  def print_constant(type, name, val)
    @f.print "\t", name
    (25 - name.to_s.length).times { @f.print " " }
    @f.print " =  ", val, ";"
  end

  def print_proto(name, ret_type, args)
    if ret_type == nil
      @f.print "procedure"
    else
      @f.print "function"
    end
    @f.print " ", name, "("
    if args != nil and args != []
      lastelt = args.pop
      args.each do |arg|
        @f.print arg[0], " : ", conv_type(arg[1]), "; "
      end
      @f.print lastelt[0], " : ", conv_type(lastelt[1])
      args.push(lastelt)
    end
    @f.print ")"
    if ret_type != nil
      @f.print " : ", conv_type(ret_type)
    end
    @f.print ";"
  end

  def generate_makefile
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
#
# Makefile
#

SRC       = #{@source_file}.pas # Ajoutez ici vos fichiers .pas
NAME      = #{$conf['conf']['player_lib']}.so
MY_CFLAGS =

include ../includes/makepascal
    EOF
    @f.close
  end

  def build()
    @path = Pathname.new($install_path) + "pascal"
    @source_file = $conf['conf']['player_filename']

    #
    # Generate c-interface file
    #
    @f = File.open(@path + ('prolo_interface.pas'), 'w')
    @f.puts "unit prolo_interface;", "", "interface", ""
    print_banner "generator_pascal.rb"
    @f.puts "const\n"
    build_constants
    for_each_fun do |x, y, z|
      print_proto(x, y, z);
      @f.puts " cdecl; external;"
    end
    @f.puts "implementation", "", "begin", "end."
    @f.close

    #
    # Generate user file
    #
    @f = File.open(@path + (@source_file + '.pas'), 'w')
    @f.puts "library champion;","", "uses prolo_interface;", ""
    for_each_user_fun do |x, y, z|
      print_proto(x, y, z)
      @f.puts " cdecl; export;"
      @f.puts "begin", "\t(* fonction a completer *)", "end;"
    end
    @f.puts "", "exports"
    @f.print ["init_game", "play_turn"].join(",\n")
    @f.print ";\n"
    @f.puts "", "begin", "end."
    @f.close

    #
    # Generate Makefile
    #
    generate_makefile
  end
end
