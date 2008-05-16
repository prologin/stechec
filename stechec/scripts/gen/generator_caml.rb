# -*- ruby -*-
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

require "pathname"

# C generator with some Caml specificity
class CamlCFileGenerator < CProto

  def initialize
    @lang = "C with Caml extension"
    @types = {
      'void' => 'void',
      'int' => 'value',
      'bool' => 'value'
    }
  end

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_caml.rb"
    for_each_fun { |x, y, z| print_proto(x, y, z, "extern"); @f.puts ";" }
    for_each_user_fun { |x, y, z| print_proto(x, y, z); @f.puts ";" }
    @f.close
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_caml.rb"

    # caml -> api glue
    print_include "caml/mlvalues.h", true
    print_include "caml/callback.h", true
    print_include "caml/alloc.h", true
    print_include @header_file
    @f.puts

    for_each_fun do |name, ret_type, args| 
      print_proto("ml_" + name, ret_type, args)
      body = "  "
      body = "return Val_int(" if ret_type and ret_type != "void"
      body += name + "(" 
      if args != nil and args != []
        print_args = args.collect {|x| "Int_val(" + x[0] + ")" }
        body += print_args.join ", "
      end
      body += ")" if ret_type && ret_type != "void"
      body += ");"

      print_body body
    end
    
    # api -> caml glue
    for_each_user_fun do |name, ret, args| 
      print_proto(name, ret, args)
      print_body "
  static value *fib_closure = NULL;

  if (fib_closure == NULL)
    fib_closure = caml_named_value(\"ml_#{name}\");
   callback(*fib_closure, Val_int(0));"
    
    end
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "caml"
    @header_file = 'interface.h'
    @source_file = 'interface.c'

    generate_header
    generate_source
  end

end


class CamlFileGenerator < FileGenerator
  def initialize
    @lang = "caml"
    @types = {
      'void' => 'unit',
      'int' => 'int',
      'bool' => 'bool'
    }
  end

  def print_comment(str)
    @f.puts '(* ' + str + ' *)' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '(*'
    str.each {|s| @f.print '** ', s }
    @f.puts "\n*)"
  end

  def print_constant(type, name, val)
      @f.print 'let _', name, " = ", val, ";;\n"
  end

  def print_proto(name, ret_type, args)
    @f.print "external ", name, " : "
    if args != nil and args != []
      args.each do |arg|
        @f.print conv_type(arg[1]), " -> "
      end
    else
      @f.print "unit -> "
    end
    @f.print conv_type(ret_type), ' = "ml_', name, '";;'
  end
  
  def generate_makefile
    target = $conf['conf']['player_lib']
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

# Tu peux rajouter des fichiers .ml ou changer les flags de ocamlc
#{target}-srcs = api.ml #{@caml_file}
#{target}-camlflags = -g

# Evite de toucher a ce qui suit
#{target}-dists = interface.h
#{target}-srcs += interface.c ../includes/main.c
include ../includes/rules.mk
    EOF
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "caml"
    @caml_file = $conf['conf']['player_filename'] + '.ml'
    @caml_api_file = "api.ml"

    # Build C interface files
    CamlCFileGenerator.new.build

    #
    # Generate the api ml file.
    #
    @f = File.open(@path + @caml_api_file, 'w')
    print_banner "generator_caml.rb"

    # protos
    build_constants
    for_each_fun { |x, y, z| print_proto(x, y, z); @f.puts }

    @f.close

    #
    # Generate ml source file
    #
    @f = File.open(@path + @caml_file, 'w')
    print_banner "generator_caml.rb"

    @f.puts "open Api;;", ""

    # function to be completed
    @f.puts
    for_each_user_fun do |name, ret, args| 
      @f.print "let ", name, " () =  (* Pose ton code ici *)\n"
      @f.puts "  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)"
    end

    # callback register
    print_comment "/!\\ Ne touche pas a ce qui suit /!\\"
    for_each_user_fun(false) do |name, ret, args| 
      @f.print 'Callback.register "ml_', name, '" ', name, ";;"
    end
    @f.close

    generate_makefile
  end
end
