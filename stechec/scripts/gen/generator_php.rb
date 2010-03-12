#
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2010 Prologin
#

# C++ generator, for PHP interface
class PhpCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for PHP interface)"
  end

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_php.rb"

    @f.puts <<-EOF
#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <sapi/embed/php_embed.h>
# include <vector>

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

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_php.rb"

    @f.puts <<-EOF
#include "interface.hh"

static void _init_php();
EOF
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + 'php'
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'

    generate_header
    generate_source
  end
end

class PhpFileGenerator < FileGenerator
  def initialize
    super
    @lang = "php"
  end

  def print_constant(type, name, val)
    @f.puts "define('#{name}', #{val});"
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '/*'
    str.each_line { |s| @f.print ' * ', s }
    @f.puts "", " */"
  end

  def build_user_fun_stubs
    for_each_user_fun do |fn|
      @f.print "function #{fn.name}("
      args = fn.args.map { |a| a.name }
      @f.print args.join(", ")
      @f.puts ")", "{"
      @f.puts "  /* Place ton code ici */"
      @f.puts "}"
    end
  end

  def build_enums
    for_each_enum do |enum|
      i = 0
      enum['enum_field'].each do |f|
        name = f[0].upcase
        @f.puts "define('#{name}', #{i}); // #{f[1]}"
        i += 1
      end
    end
  end

  def build
    @path = Pathname.new($install_path) + "php"
    @source_file = $conf['conf']['player_filename'] + '.php'

    PhpCxxFileGenerator.new.build

    @f = File.new(@path + @source_file, 'w')
    @f.puts "<?php"
    print_banner "generator_php.rb"
    @f.puts "require('api.php');"
    @f.puts ""
    build_user_fun_stubs
    @f.puts "?>"
    @f.close

    @f = File.new(@path + 'api.php', 'w')
    @f.puts "<?php"
    build_constants
    build_enums
    @f.puts "?>"
    @f.close
  end
end
