#  -*- ruby -*-
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2005, 2006, 2007 Prologin
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
    target = $conf['conf']['player_lib']
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
#{target}-srcs = #{@source_file}
#{target}-dists =
#{target}-cflags = -ggdb3

# Evite de toucher a ce qui suit
#{target}-dists += #{@header_file}
#{target}-srcs += ../includes/main.c
include ../includes/rules.mk
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
