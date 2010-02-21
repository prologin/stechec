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


module PascalUtils
  def cxx_type(type)
    if type.is_array? then
      "std::vector<#{cxx_type(type.type) }>"
    else
      type.name
    end
  end

  def conv_type(type)
    if type.is_simple? then
      {
        "bool" => "boolean",
        "int"  => "integer",
      "void" => "void" # useless case
      }[type.name]
    elsif type.is_array? then
      "array of #{ conv_type type.type }"
    else
      type.name
    end
  end

  def print_comment(str)
    @f.puts '{ ' + str + ' }' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '{'
    str.each_line {|s| @f.print "  ", s }
    @f.puts "\n}"
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

end

class PascalCxxFileGenerator < CxxFileGenerator

  def lang_type(type)
    if type.is_array? then "#{lang_type(type.type)}*"
    else type.name
    end
  end

  def initialize
    super
    @lang = "C++ (for C interface)"
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_pascal.rb"
    @f.puts <<-EOF
#include "interface.hh"

template<typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.error;
}

template<>
int lang2cxx<int, int>(int in)
{
  return in;
}
template<>
bool lang2cxx<bool, bool>(bool in)
{
  return in;
}

template<typename Lang, typename Cxx>
std::vector<Cxx> vect lang2cxx_array(Lang *l)
{
  int len = *( ((int *)l) -1 );
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back( lang2cxx<Lang, Cxx>(l[i]) );
return vect;
}

template<typename Lang, typename Cxx>
Cxx cxx2lang(Lang in)
{
  return in.err;
}

template<>
int cxx2lang<int, int>(int in)
{
  return in;
}
template<>
bool cxx2lang<bool, bool>(bool in)
{
  return in;
}

template<typename Lang, typename Cxx>
Lang * cxx2lang_array(const std::vector<Cxx>& vect)
{
  len = vect.size(); 
  int * tab = malloc(sizeof(int) + sizeof(Lang) * len);
  tab[0] = len;
  Lang * tabl = (Lang *)(tab + 1);
  for (int i = 0; i < len; ++i)
    tabl[i] = cxx2lang<Lang, Cxx>(vect[i]);
  return tabl;
}

extern "C" {
EOF
    for_each_enum do |x|
      name = x['enum_name']
      @f.puts "template<>"
      @f.puts "#{name} lang2cxx<#{name}, #{name}>(#{name} in) {"
      @f.puts " return in;"
      @f.puts "}"
      @f.puts "template<>"
      @f.puts "#{name} cxx2lang<#{name}, #{name}>(#{name} in) {"
      @f.puts " return in;"
      @f.puts "}"
    end
    for_each_struct do |x|
      def aux(fields, name_fun, lang, cxx)
        fields['str_field'].each do |f|
          name =f[0]
          type = @types[f[1]]
          if type.is_array? then type_ = type.type
          else type_ = type
          end
          lang_type = c_type(type_)
          if type_.is_struct? then cxx_type = "__internal__cxx__" + cxx_type(type_) else cxx_type = cxx_type(type_) end
          if type.is_array? then
            name_fun = "#{name_fun}_array"
          end
          @f.puts "out.#{name} = #{name_fun}<#{lang_type}, #{cxx_type}>(in.#{name});"
        end
      end
      c_name = x['str_name']
      cxx_name = "__internal__cxx__#{c_name}"
      @f.puts "template<>"
      @f.puts "#{cxx_name} lang2cxx<#{c_name}, #{cxx_name}>(#{c_name} in) {"
      @f.puts "#{cxx_name} out;"
      aux(x, "lang2cxx", "in", "out")
      @f.puts " return out;"
      @f.puts "}"
      @f.puts "template<>"
      @f.puts "#{c_name} cxx2lang<#{c_name}, #{cxx_name}>(#{cxx_name} in) {"
      @f.puts "#{c_name} out;"
      aux(x, "cxx2lang", "out", "in")
      @f.puts " return out;"
      @f.puts "}"
    end
    for_each_fun do |fn|
      @f.puts c_proto(fn) # todo ...
      @f.puts "{"
      @f.print "  ", cxx_type(fn.ret), " ", "_retval;\n"

      # Check if some of the arguments need conversion
      fn.args.each do |arg|
        type = arg.type
        if type.is_array?
          cxx_type = cxx_type(type.type)
          lang_type = lang_type(type.type)
          @f.puts "  #{cxx_type(arg.type)} arg_#{arg.name} = lang2cxx_array<#{lang_type}, #{cxx_type}>(#{arg.name});"
        else
          cxx_type = cxx_type(type)
          lang_type = lang_type(type)
          @f.puts "  #{cxx_type(arg.type)} arg_#{arg.name} = lang2cxx<#{lang_type}, #{cxx_type}>(#{arg.name});"
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
      proto = CxxProto.new
      @f.print proto.cxx_proto(fn), ";\n"
    end
    @f.puts "#endif"
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "pascal"
    @source_file = 'interface.cc'
    @header_file = 'interface.hh'

    generate_source
    generate_header
  end
end

class PascalFileGenerator < FileGenerator
  include PascalUtils

  def initialize
    super
    @lang = "C++ (for pascal interface)"
  end

  def build_enums
    for_each_enum do |x|
      @f.puts "type #{x['enum_name']} =\n  (\n"
      fields = x['enum_field'].map do |f|
        name = f[0].downcase
        "    #{ name } { <- #{ f[1] } }"
      end
      @f.print "#{ fields.join(",\n") }\n  );\n\n"
    end
  end

  def build_structs
    for_each_struct do |x|
      @f.puts "type #{x['str_name']} =\n  record\n"
      x['str_field'].each do |f|
        @f.print "    #{f[0]} : #{ conv_type(@types[f[1]]) }; "
        print_comment (" <- " + f[2])
      end
      @f.print "  end;\n\n"
    end
  end

  def print_constant(type, name, val)
    @f.print "\t", name
    (25 - name.to_s.length).times { @f.print " " }
    @f.print " =  ", val, ";"
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
    PascalCxxFileGenerator.new.build

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
