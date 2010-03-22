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
class CamlCFileGenerator < CxxProto

  def initialize
    super
    @lang = "C++ with Caml extension"
  end

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_caml.rb"
    print_include "vector", true
    build_enums
    build_structs
    for_each_fun do |fn|
      @f.print cxx_proto(fn, "api_", 'extern "C"'); @f.puts ";"
    end
    for_each_user_fun do |fn|
      @f.print cxx_proto(fn, "", 'extern "C"'); @f.puts ";"
    end
    @f.close
  end

  def proto(fn)
    buf = "extern \"C\" value ml_#{fn.name}("
    args = fn.args.map do |arg|
      "value #{arg.name}"
    end
    if args == []
      args = ["value unit"]
    end
    buf + args.join(", ") + ")"
  end

  def build_common_wrappers
    @f.puts <<-EOF
template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
value cxx2lang<value, int>(int in)
{
  return Val_int(in);
}

template <>
value cxx2lang<value, bool>(bool in)
{
  return Val_int(in);
}

template <typename Cxx>
value cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  if (size == 0)
    return Atom(0);

  value v = caml_alloc(size, 0);
  for (int i = 0; i < size; ++i)
    Field(v, i) = cxx2lang<value, Cxx>(in[i]);

  return v;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<value, int>(value in)
{
  return Int_val(in);
}

template <>
bool lang2cxx<value, bool>(value in)
{
  return Int_val(in);
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(value in)
{
  std::vector<Cxx> out;
  mlsize_t size = Wosize_val(in);

  for (int i = 0; i < size; ++i)
    out.push_back(lang2cxx<value, Cxx>(Field(in, i)));

  return out;
}
EOF
  end

  def build_enum_wrappers(enum)
    name = enum['enum_name']
    @f.puts <<-EOF
template <>
value cxx2lang<value, #{name}>(#{name} in)
{
  return Val_int(in);
}

template <>
#{name} lang2cxx<value, #{name}>(value in)
{
  return (#{name})Int_val(in);
}
EOF
  end

  def build_struct_wrappers(struct)
    name = struct['str_name']
    nfields = struct['str_field'].length
    @f.puts "template <>"
    @f.puts "value cxx2lang<value, #{name}>(#{name} in)"
    @f.puts "{"
    @f.puts "  value out = caml_alloc(#{nfields}, 0);"
    i = 0
    struct['str_field'].each do |f|
      fn = f[0]
      ft = @types[f[1]]
      if ft.is_array?
        @f.puts "  Field(out, #{i}) = cxx2lang_array(in.#{fn});"
      else
        ft = ft.name
        @f.puts "  Field(out, #{i}) = cxx2lang<value, #{ft}>(in.#{fn});"
      end
      i += 1
    end
    @f.puts "  return out;"
    @f.puts "}"
    @f.puts

    # OTHER WAY
    @f.puts "template <>"
    @f.puts "#{name} lang2cxx<value, #{name}>(value in)"
    @f.puts "{"
    @f.puts "  #{name} out;"
    i = 0
    struct['str_field'].each do |f|
      fn = f[0]
      ft = @types[f[1]]
      if ft.is_array?
        @f.puts "  out.#{fn} = lang2cxx_array<#{ft.type.name}>(Field(in, #{i}));"
      else
        ft = ft.name
        @f.puts "  out.#{fn} = lang2cxx<value, #{ft}>(Field(in, #{i}));"
      end
      i += 1
    end
    @f.puts "  return out;"
    @f.puts "}"
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_caml.rb"

    # caml -> api glue
    @f.puts 'extern "C" {'
    print_include "caml/mlvalues.h", true
    print_include "caml/callback.h", true
    print_include "caml/alloc.h", true
    print_include "caml/memory.h", true
    @f.puts "}"
    print_include @header_file
    @f.puts

    build_common_wrappers
    for_each_enum { |enum| build_enum_wrappers enum }
    for_each_struct { |struct| build_struct_wrappers struct }

    for_each_fun do |fn|
      @f.print proto(fn)
      @f.puts "", "{"
      args = fn.args
      if args == [] then
        args = [FunctionArg.new(@types, ["unit", "void"])]
      end
      @f.puts "  CAMLparam0();"
      args.each do |arg|
        @f.puts "  CAMLxparam1(#{arg.name});"
      end
      @f.puts
      unless fn.ret.is_nil? then
        @f.print "  CAMLreturn(("
        if fn.ret.is_array? then
          @f.print "cxx2lang_array<#{fn.ret.type.name}>("
        else
          @f.print "cxx2lang<value, #{fn.ret.name}>("
        end
      else
        @f.print "  "
      end
      @f.print "api_", fn.name, "("
      args = fn.args.map do |arg|
        if arg.type.is_array? then
          "lang2cxx_array<#{arg.type.type.name}>(#{arg.name})"
        else
          "lang2cxx<value, #{arg.type.name}>(#{arg.name})"
        end
      end
      @f.print args.join(", ")
      @f.print ")"
      if fn.ret.is_nil? then
        @f.puts ";"
        @f.puts "  CAMLreturn(Val_unit);"
      else
        @f.puts ")));"
      end
      @f.puts "}"
    end
    
    # api -> caml glue
    for_each_user_fun do |fn|
      @f.print cxx_proto(fn)
      @f.puts "", "{"
      @f.puts "  static value *closure = NULL;"
      @f.puts "  if (closure == NULL)"
      @f.puts "    closure = caml_named_value(\"ml_#{fn.name}\");"
      @f.puts "  value _ret = callback(*closure, Val_unit);"
      if fn.ret.is_nil?
        @f.puts "  return;"
      elsif fn.ret.is_array?
        @f.puts "  return lang2cxx_array<#{fn.ret.type.name}>(_ret);"
      else
        @f.puts "  return lang2cxx<value, #{fn.ret.name}>(_ret);"
      end
      @f.puts "}", ""
    end

    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "caml"
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'

    generate_header
    generate_source
  end

end


class CamlFileGenerator < FileGenerator
  def initialize
    super
    @lang = "OCaml"
  end

  def conv_type(type)
    if type.is_nil?
      "unit"
    elsif type.is_array?
      "#{type.type.name} array"
    else
      type.name
    end
  end

  def print_comment(str)
    @f.puts '(* ' + str + ' *)' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '(*'
    str.each_line {|s| @f.print '** ', s }
    @f.puts "\n*)"
  end

  def print_constant(type, name, val)
      @f.print 'let ', name.downcase, " = ", val, "\n"
  end

  def build_enums
    for_each_enum do |e|
      @f.puts "type #{e['enum_name']} ="
      e['enum_field'].each do |f|
        name = f[0].capitalize
        @f.print "| ", name, " (* <- ", f[1], " *)\n"
      end
      @f.puts
    end
  end

  def build_structs
    for_each_struct do |s|
      @f.print "type #{s['str_name']} = "
      if s['str_tuple']
        @f.print "("
        args = s['str_field'].map { |f| f[1] }
        @f.print args.join(" * ")
        @f.puts ")"
      else
        @f.puts "{"
        s['str_field'].each do |f|
          @f.puts "  #{f[0]} : #{f[1]} ; (* <- #{f[2]} *)"
        end
        @f.puts "}"
      end
    end
  end

  def print_proto(fn, external = true)
    if external
      prefix = "external"
    else
      prefix = "val"
    end
    @f.print prefix, " ", fn.name, " : "
    if fn.args != []
      fn.args.each do |arg|
        @f.print conv_type(arg.type), " -> "
      end
    else
      @f.print "unit -> "
    end
    @f.print conv_type(fn.ret)
    @f.print ' = "ml_', fn.name, '"' unless not external
  end
  
  def generate_makefile
    target = $conf['conf']['player_lib']
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

# Tu peux rajouter des fichiers .ml ou changer les flags de ocamlc
# Les fichiers .mli correspondants seront automatiquement utilises si
# existants.
#{target}-srcs = api.ml #{@caml_file}
#{target}-camlflags = -g

# Evite de toucher a ce qui suit
#{target}-dists = interface.hh
#{target}-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
    EOF
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "caml"
    @caml_ml_file = $conf['conf']['player_filename'] + '.ml'
    @caml_mli_file = $conf['conf']['player_filename'] + '.mli'
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
    build_enums
    build_structs
    for_each_fun do |fn|
      print_proto fn
    end

    @f.close

    #
    # Generate ml source file
    #
    @f = File.open(@path + @caml_ml_file, 'w')
    print_banner "generator_caml.rb"

    @f.puts "open Api;;", ""

    # function to be completed
    @f.puts
    for_each_user_fun do |fn|
      @f.print "let ", fn.name, " () =  (* Pose ton code ici *)\n"
      @f.puts "  flush stderr; flush stdout;; (* Pour que vos sorties s'affichent *)"
    end

    # callback register
    print_comment "/!\\ Ne touche pas a ce qui suit /!\\"
    for_each_user_fun(false) do |fn| 
      @f.print 'Callback.register "ml_', fn.name, '" ', fn.name, ";;"
    end
    @f.close

    #
    # Generate mli interface file
    #
    @f = File.open(@path + @caml_mli_file, 'w')
    print_banner "generator_caml.rb"

    @f.puts "open Api", ""

    for_each_user_fun do |fn|
      print_proto(fn, false)
      @f.puts
    end
    @f.close

    generate_makefile
  end
end
