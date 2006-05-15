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

class CxxFileGenerator < CxxProto

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_cxx.rb"
    build_constants
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |x, y, z| print_proto(x, y, z, "extern"); @f.puts ";" }
    for_each_user_fun { |x, y, z| print_proto(x, y, z); @f.puts ";" }
    @f.puts "}"
    @f.close
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_cxx.rb"
    print_include @header_file
    @f.puts
    for_each_user_fun do |x, y, z| 
      print_proto x, y, z
      print_body "  // fonction a completer"
    end
    @f.close
  end

  def generate_makefile
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
#
# Makefile
#

INCL      = #{@header_file} # Ajoutez ici vos fichiers .hh eventuels
SRC       = #{@source_file} # Ajoutez ici vos fichiers .cc
NAME      = #{$conf['conf']['player_lib']}.so

MY_CXXFLAGS = -ggdb3

include ../includes/makecxx
    EOF
    @f.close
  end

  def build()
    @path = Pathname.new($install_path) + "cxx"
    @header_file = $conf['conf']['player_filename'] + '.hh'
    @source_file = $conf['conf']['player_filename'] + '.cc'

    generate_header
    generate_source
    generate_makefile
  end

end
