# -*- ruby -*-
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2011 Prologin

class JavaCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for Java interface)"
  end

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner 'generator_java.rb'

    @f.puts <<-EOF
#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <jni.h>
# include <vector>
# include <string>

    EOF

    build_enums
    build_structs

    @f.puts "", 'extern "C" {', ""
    for_each_fun do |fn|
      @f.print cxx_proto(fn, "api_")
      @f.puts ";"
    end
    @f.puts "}", "", "#endif // !INTERFACE_HH_"
    @f.close
  end

  def build_common_wrappers
    @f.puts "// TODO: common wrappers", ""
  end

  def build_enum_wrappers(enum)
    name = enum['enum_name']
    @f.puts "// TODO: wrapper for enum #{name}"
  end

  def build_struct_wrappers(str)
    name = str['str_name']
    @f.puts "// TODO: wrapper for struct #{name}"
  end

  def build_function(fn)
    @f.puts "// TODO: wrapper for API function #{fn.name}"
  end

  def build_user_function(fn)
    @f.puts "// TODO: wrapper for user function #{fn.name}"
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_java.rb"

    @f.puts <<-EOF
#include "interface.hh"

static void _init_vm();

    EOF

    build_common_wrappers
    for_each_enum { |e| build_enum_wrappers e }
    for_each_struct { |s| build_struct_wrappers s }
    for_each_fun { |fn| build_function fn }

    @f.puts <<-EOF

// TODO: Java/C++ interface functions

    EOF

    for_each_user_fun { |fn| build_user_function fn }

    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "java"
    @header_file = "interface.hh"
    @source_file = "interface.cc"

    generate_header
    generate_source
  end
end

class JavaFileGenerator < JavaProto
  def initialize
    super
    @lang = "java"
    @java_types = {
      'void' => 'void',
      'int' => 'int',
      'bool' => 'boolean'
    }
    @java_obj_types = {
      'int' => 'Integer',
      'bool' => 'Boolean'
    }
  end

  def name_to_type(str)
    $conf['enum'].each do |x|
       if str == x['enum_name']
         return x
       end
    end
    return nil #error
  end

  def build
    @path = Pathname.new($install_path) + "java"
    @java_interface = 'Interface'
    @java_file = $conf['conf']['player_filename'].capitalize

    JavaCxxFileGenerator.new.build

    ######################################
    ##  Interface.java file generating  ##
    ######################################
    @f = File.new(@path + (@java_interface + '.java'), 'w')
    print_banner "generator_java.rb"

    for_each_enum do |x|
      name = conv_java_type(x['enum_name'])
      @f.puts "enum #{name} {"
      x['enum_field'].each do |f|
        @f.puts "  #{f[0].upcase}, // <- #{f[1]}"
      end
      @f.puts "}"
    end

    for_each_struct do |x|
      name = conv_java_type(x['str_name'])
      @f.puts "class #{name} {"
      x["str_field"].each do |f|
        @f.print "  public #{conv_java_type(f[1])} #{f[0]}"
        if @types[f[1]].is_struct? then
          # Initialize structures so that the candidate can use the object
          # more easily
          @f.print " = new #{conv_java_type(f[1])}()";
        end
        @f.puts "; // #{f[2]}"
      end
      @f.puts "}"
    end

    @f.puts "public class #{@java_interface}", "{"
    build_constants '  '
    for_each_fun(true, 'function', '  ') do |f|
      print_proto("  public static native", f)
      @f.puts ";"
    end
    @f.puts "}"
    @f.close

    #####################################
    ##  Prologin.java file generating  ##
    #####################################
    @f = File.new(@path + (@java_file + '.java'), 'w')
    print_banner "generator_java.rb"
    # generate functions bodies
    @f.puts "public class #{@java_file} extends #{@java_interface}", "{"
    for_each_user_fun(true, '  ') do |f|
      @f.print "  "
      print_proto("public", f)
      @f.puts "",
      "  {",
      "    // Place ton code ici",
      "  }",
      ""
    end
    @f.puts "}"
    @f.close

    #
    # Makefile generation
    #
    target = $conf['conf']['player_lib']
    @f = File.new(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

#{target}-srcs = Interface.java Prologin.java
#{target}-cxxflags = -I. -ggdb3

# Evite de toucher a ce qui suit
EOF
    @f.print "#{target}-jclassopt ="
    $conf['struct'].each do |x|
        @f.print " ", x['str_name'].capitalize, ".class"
    end
    $conf['enum'].each do |x|
       @f.print " ", x['enum_name'].capitalize, ".class"
    end
    @f.puts
    @f.print <<-EOF
#{target}-dists = interface.hh
#{target}-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
    EOF
    @f.close
  end

end
