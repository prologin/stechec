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
    super
    @lang = "C++ (for pascal interface)"
  end

  def conv_type(type)
    if type.is_simple? and type.name = "bool"
      "boolean"
    else
      type.name
    end
  end

  def build_enums
    for_each_enum do |x|
      @f.puts "type #{x['enum_name']} =\n  (\n"
      x['enum_field'].each do |f|
        name = f[0].downcase
        @f.print "    ", name, ", " 
        print_comment (" <- "+ f[1])
      end
      @f.print "  );\n\n"
    end
  end
  def build_structs
    for_each_struct do |x|
      @f.puts "type #{x['str_name']} =\n  record\n"
      x['str_field'].each do |f|
        @f.print "    #{f[0]} : #{f[1]}; "
        print_comment (" <- " + f[2])
      end
      @f.print "  end;\n\n"
    end
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

  def print_proto(fn)
    if fn.ret.is_nil?
      @f.print "procedure"
    else
      @f.print "function"
    end
    @f.print " ", fn.name
    args = fn.args.map do |arg|
      "#{arg.name} : #{ conv_type(arg.type) }"
    end
    @f.print "(#{args.join("; ")})"
    if not fn.ret.is_nil?
      @f.print " : ", conv_type(fn.ret)
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
    build_enums
    build_structs
    for_each_fun do |f|
      print_proto(f);
      @f.puts " cdecl; external;"
    end
    @f.puts "implementation", "", "begin", "end."
    @f.close

    #
    # Generate user file
    #
    @f = File.open(@path + (@source_file + '.pas'), 'w')
    @f.puts "library champion;","", "uses prolo_interface;", ""
    for_each_user_fun do |f|
      print_proto(f)
      @f.puts " cdecl; export;"
      @f.puts "begin", "\t(* fonction a completer *)", "end;"
    end
    @f.puts "", "exports"
    v = false
    for_each_user_fun false do |f|
      if v then @f.print "," end
      v = true
      @f.print "  #{ f.name }"
    end
    @f.print ";\n"
    @f.puts "", "begin", "end."
    @f.close

    #
    # Generate Makefile
    #
    generate_makefile
  end
end
