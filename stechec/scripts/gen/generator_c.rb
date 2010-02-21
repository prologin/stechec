# -*- ruby -*-
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

require "pathname"
require "gen/file_generator"

def c_type(type)
  if type.is_simple? and type.name == "bool"
    "int"
  else
    type.name
  end
end

def c_proto(fn)
  # Returns the prototype of a C function
  # WARNING: arrays are hard to handle in C...
  buf = ""
  if fn.ret.is_array?
    rettype = "void"
  else
    rettype = c_type(fn.ret)
  end

  buf += rettype + " " + fn.name + "("

  # Handle arguments
  args = []
  fn.args.each do |arg|
    if not arg.type.is_array?
      type = c_type(arg.type)
      args = args << "#{type} #{arg.name}"
    else
      args = args << "#{arg.type.type.name}* #{arg.name}_arr"
      args = args << "size_t #{arg.name}_len"
    end
  end
  if fn.ret.is_array?
    args = args << "#{fn.ret.type.name}** ret_arr"
    args = args << "size_t* ret_len"
  end
  if args.empty?
    args = args << "void"
  end
  buf += args.join(", ")
  buf + ")"
end

class CCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for C interface)"
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_c.rb"
    @f.puts <<-EOF
#include "interface.hh"

#include <cstring>

// Utils
template<typename T>
void c_array_to_vector(T* tab, size_t len, std::vector<T>& vect)
{
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back(tab[i]);
}

extern "C" {
EOF
    for_each_fun do |fn|
      @f.puts c_proto(fn)
      @f.puts "{"
      @f.print "  ", cxx_type(fn.ret), " ", "_retval;\n"

      # Check if some of the arguments need conversion
      fn.args.each do |arg|
        if arg.type.is_array?
          # Make a vector from the arr + len
          arrname = arg.name + "_arr"
          lenname = arg.name + "_len"
          name = arg.name
          @f.print "  ", cxx_type(arg.type), " ", name, ";\n"
          @f.puts "c_array_to_vector(#{arrname}, #{lenname}, #{name});"
        end
      end

      @f.print "  _retval = "
      @f.print "api_", fn.name, "("
      argnames = fn.args.map { |arg| arg.name }
      @f.print argnames.join(", ")
      @f.puts ");"

      # Return if it is not an array, else convert
      if not fn.ret.is_array?
        @f.puts "  return _retval;"
      else
        # Return is in the two last args, "ret" and "ret_len"
        # Put the size in ret_len
        @f.puts "  *ret_len = _retval.size();"
        # Allocate ret
        ty = fn.ret.type.name
        @f.puts "  *ret_arr = (#{ty}*)malloc((*ret_len) * sizeof(#{ty}));"
        # Copy from the _retval vector
        @f.puts "  memcpy(*ret_arr, &_retval[0], *ret_len);"
        # Done !
      end

      @f.puts "}"
    end
    @f.puts "}"
    @f.close
  end

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_c.rb"
    @f.puts "#ifndef INTERFACE_HH_", "# define INTERFACE_HH_"
    @f.puts "", "# include <vector>", "# include <string>", ""
    @f.puts 'extern "C" {'
    @f.puts "# include \"#{$conf['conf']['player_filename']}.h\""
    @f.puts "}", ""
    for_each_fun do |fn|
      @f.print cxx_proto(fn), ";\n"
    end
    @f.puts "#endif"
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "c"
    @source_file = 'interface.cc'
    @header_file = 'interface.hh'

    generate_source
    generate_header
  end
end

class CFileGenerator < CProto
  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_c.rb"
    @f.puts "#include <stdlib.h>"
    build_constants
    build_enums
    build_structs
    for_each_fun do |f|
      @f.print c_proto(f), ";\n"
    end
    for_each_user_fun do |f|
      @f.print c_proto(f), ";\n"
    end
    @f.close
  end

  def generate_source()
    @f = File.open(@path + @user_file, 'w')
    print_banner "generator_c.rb"
    print_include @header_file
    @f.puts
    for_each_user_fun do |f|
      @f.print c_proto(f)
      print_body "  /* fonction a completer */"
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
#{target}-srcs = #{@user_file}
#{target}-dists =
#{target}-cflags = -ggdb3

# Evite de toucher a ce qui suit
#{target}-dists += #{@header_file}
#{target}-srcs += #{@source_file} ../includes/main.c
include ../includes/rules.mk
    EOF
    @f.close
  end


  def build()
    CCxxFileGenerator.new.build

    @path = Pathname.new($install_path) + "c"
    @header_file = $conf['conf']['player_filename'] + '.h'
    @source_file = 'interface.cc'
    @user_file = $conf['conf']['player_filename'] + '.c'

    generate_header
    generate_source
    generate_makefile
  end
end

