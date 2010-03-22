
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2006 Prologin
#

require "gen/file_generator"

def camel_case(str)
  strs = str.split("_")
  strs.each { |s| s.capitalize! }
  strs.join
end

def cs_proto(fn)
  # Returns the prototype of a C function
  # WARNING: arrays are hard to handle in C...
  buf = ""
  if fn.ret.is_array?
    rettype = "void"
  else
    rettype = fn.ret.name
  end

  if not fn.ret.is_simple?
    rettype = camel_case(rettype)
  end

  buf += "\t\tpublic " + rettype + " " + camel_case(fn.name) + "("

  # Handle arguments
  args = []
  fn.args.each do |arg|
    if not arg.type.is_array?
      args = args << "#{arg.type} #{arg.name}"
    else
      args = args << "#{arg.type.type.name}[] #{arg.name}_arr"
    end
  end
  if fn.ret.is_array?
    args = args << "#{fn.ret.type.name}[] ret_arr"
  end
  if args.empty?
    args = args << ""
  end
  buf += args.join(", ")
  buf + ")\n"
end

# C++ generator, for C# interface
class CSharpCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for C# interface)"
  end

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_cs.rb"

    @f.puts <<-EOF
#ifndef INTERFACE_HH_
# define INTERFACE_HH_

#include <map>
#include <vector>
#include <string>
#include <glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

class CSharpInterface
{
public:
  CSharpInterface();
  ~CSharpInterface();
  MonoObject*   callCSharpMethod(const char* name);
  MonoImage*    getImage();
  MonoDomain*   getDomain();

private:

  MonoDomain*		_domain;
  MonoAssembly*		_assembly;
  MonoImage*		_image;
  MonoClass*		_class;
  MonoObject*       _object;
};

    EOF

    build_enums
    build_structs

    @f.puts "", 'extern "C" {', ""
    for_each_fun { |fn| @f.print cxx_proto(fn, "api_"); @f.puts ";" }
    for_each_user_fun { |fn| @f.print cxx_proto(fn); @f.puts ";" }
    @f.puts "}", "", "#endif // !INTERFACE_HH_"
    @f.close
  end

  def build_common_wrappers
    @f.puts <<-EOF
template <class Out, class Cxx>
Out cxx2lang(Cxx in)
{
  return (Out)in;
}

template <>
gint32 cxx2lang<gint32, int>(int in)
{
  return (gint32)in;
}

template <>
gint32 cxx2lang<gint32, bool>(bool in)
{
  return (gint32)in;
}

template <class Out, class Cxx>
Cxx lang2cxx(Out in)
{
  return (Cxx)in;
}

template <>
int lang2cxx<gint32, int>(gint32 in)
{
  return (int)in;
}

template <>
bool lang2cxx<gint32, bool>(gint32 in)
{
  return (bool)in;
}
/*
template <typename Cxx>
std::vector<Cxx> lang2cxx_array(MonoArray* in)
{
  std::vector<Cxx> out;
  mlsize_t size = mono_array_length(in);

  for (int i = 0; i < size; ++i)
    out.push_back(lang2cxx<value, Cxx>(Field(in, i)));

  return out;
}*/
    EOF
  end

#TODO Finish it
  def build_array()
    @f.puts <<-EOF
template <class Cxx>
MonoArray* cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  if (size == 0)
    return mono_array_new(_domain, "Prologin::");

  value v = caml_alloc(size, 0);
  for (int i = 0; i < size; ++i)
    Field(v, i) = cxx2lang<value, Cxx>(in[i]);

  return v;
}
    EOF
  end

  def build_struct_wrappers(str)
    name = str['str_name']
    @f.puts "template <>"
    @f.puts "MonoObject* cxx2lang<MonoObject*, #{name}>(#{name} in)"
    @f.puts "{"
    @f.puts "  void* arg;"
    @f.puts "  MonoClass*  mcKlass  = mono_class_from_name(gl_csharp.getImage(), \"Prologin\", \"#{camel_case(name)}\");"
    @f.puts "  MonoObject* moObj    = mono_object_new(gl_csharp.getDomain(), mcKlass);"
    @f.puts "  mono_runtime_object_init(moObj);"
    str['str_field'].each do |f|
      fn = f[0]
      ft = @types[f[1]]
      @f.print "  arg = reinterpret_cast<void*>("
      if ft.is_array? then
        @f.print "cxx2lang_array"
      else
        if ft.is_struct? then
          object_type = "MonoObject*"
        else
          object_type = "gint32"
        end
        @f.print "cxx2lang<#{object_type}, #{ft.name}>"
      end
      @f.puts "(in.#{fn}));"
      @f.puts "  mono_field_set_value(moObj, mono_class_get_field_from_name(mcKlass, \"#{camel_case(fn)}\"), &arg);"
    end
    @f.puts "  return moObj;"
    @f.puts "}"
    @f.puts

    @f.puts "template <>"
    @f.puts "#{name} lang2cxx<MonoObject*, #{name}>(MonoObject* in)"
    @f.puts "{"
    @f.puts "  #{name} out;"
    @f.puts "  void*      field_out;"
    @f.puts "  MonoClass* mcKlass = mono_class_from_name(gl_csharp.getImage(), \"Prologin\", \"#{camel_case(name)}\");"
    @f.puts "  (void)field_out;\n"
    str['str_field'].each do |f|
      fn = f[0]
      ft = @types[f[1]]
      if ft.is_array?
        @f.puts  "  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, \"#{camel_case(fn)}\"), &field_out);"
        @f.puts  "  out.#{fn} = lang2cxx_array<#{ft.type.name}>reinterpret_cast<MonoObject*>(field_out));"
      else
        if ft.is_struct? then
          @f.puts  "  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, \"#{camel_case(fn)}\"), &field_out);"
          @f.puts  "  out.#{fn} = lang2cxx<MonoObject*, #{ft.name}>(reinterpret_cast<MonoObject*>(field_out));"
        else
          @f.puts  "  mono_field_get_value(in, mono_class_get_field_from_name(mcKlass, \"#{camel_case(fn)}\"), &out.#{fn});"
        end
      end
    end
    @f.puts "  return out;"
    @f.puts "}"
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_cs.rb"

    @f.puts <<-EOF
#include "interface.hh"

#include <iostream>
#include <assert.h>
#include <cstdlib>

CSharpInterface gl_csharp;

    EOF
    build_common_wrappers
    for_each_struct { |s| build_struct_wrappers s }
    @f.puts <<-EOF

/*
** Inititialize Mono and load the DLL file.
*/
CSharpInterface::CSharpInterface()
{
  const char*		champion_path = getenv("CHAMPION_PATH");
  std::string		champion;

  if (!champion_path)
    champion = "./prologin.dll";
  else
  {
    champion = champion_path;
    champion += "/prologin.dll";
  }

  _domain = mono_jit_init(champion.c_str());
  assert(_domain != NULL);

  _assembly = mono_domain_assembly_open(_domain, champion.c_str());
  assert(_assembly != NULL);

  _image = mono_assembly_get_image(_assembly);
  assert(_image != NULL);

  _class = mono_class_from_name(_image, "Prologin", "Prologin");
  assert(_class != NULL);

  _object = mono_object_new(_domain, _class);
  assert(_object);

  mono_runtime_object_init(_object);

  // Register API functions as internal Mono functions
     EOF

     for_each_fun(false) do |fn|
       @f.print "//  mono_add_internal_call(\"Prologin.API::" + camel_case(fn.name) + "\", (const void*)" + fn.name + ");"
     end

     @f.puts <<-EOF
}

MonoImage* CSharpInterface::getImage()
{
    return _image;
}

MonoDomain* CSharpInterface::getDomain()
{
    return _domain;
}

CSharpInterface::~CSharpInterface()
{
  mono_image_close(_image);
  mono_assembly_close(_assembly);
  // XXX -- mono segfaults when calling this. Seems to be a known bug
  //        appearing when mono_jit_clean() is called from a dtor. ???
  //mono_jit_cleanup(_domain);
}

/*
** Calls C# functions from C++
*/
MonoObject* CSharpInterface::callCSharpMethod(const char* name)
{
  MonoMethod*	method;

  method = mono_class_get_method_from_name(_class, name, 0);
  return mono_runtime_invoke(method, _object, NULL, NULL);
}

/*
** Functions called from stechec to C.
*/
    EOF

    for_each_user_fun(false) do |fn|
      @f.print cxx_proto(fn, '')
      if cxx_type(fn.ret) != "void"
        print_body "  return lang2cxx<MonoObject*, " + cxx_type(fn.ret) + ">(gl_csharp.callCSharpMethod(\"" + camel_case(fn.name) + "\"));"
      else
        print_body "  gl_csharp.callCSharpMethod(\"" + camel_case(fn.name) + "\");"
      end
     end

   @f.close
  end

  def build
    @path = Pathname.new($install_path) + "cs"
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'

    generate_header
    generate_source
  end

end

class CSharpFileGenerator < CSharpProto
  def initialize
    super
    @lang = "cs"
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each_line {|s| @f.print '// ', s }
    @f.puts ""
  end

  def generate_api()
    @f = File.open(@path + @api_file, 'w')
    print_banner "generator_cs.rb"

    @f.puts "using System.Runtime.InteropServices;", "using System.Runtime.CompilerServices;", "using System.Collections.Generic;",""

    @f.puts "namespace Prologin {"

    build_enums
    build_structs

    @f.puts "\tclass API {"
    build_constants
    for_each_fun do |fn|
      @f.puts "\t\t[MethodImplAttribute(MethodImplOptions.InternalCall)]"
      print_proto(fn, "\t\tpublic static extern");
      @f.puts ";"
    end
    @f.puts "\t}", "}"

    @f.close
  end

  def generate_makefile
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
# -*- Makefile -*-
#

SRC       = #{@source_file} # Ajoutez ici vos fichiers .cs
NAME      = #{$conf['conf']['player_lib']}.so

include ../includes/makecs
    EOF
    @f.close
  end


  def build()
    @path = Pathname.new($install_path) + "cs"
    @source_file = $conf['conf']['player_filename'] + '.cs'
    @api_file = 'api.cs'

    ######################################
    ##  interface.hh file generating    ##
    ######################################
    CSharpCxxFileGenerator.new.build

    generate_api

    ######################################
    ##  prologin.cs file generating     ##
    ######################################
    @f = File.new(@path + @source_file, 'w')
    print_banner "generator_cs.rb"
    # Required stuff to call C from C#
    @f.puts  "using System;", "using System.Collections.Generic;", "using System.Runtime.InteropServices;", ""

    @f.puts "namespace Prologin {", "", "\tclass Prologin {", ""

    for_each_user_fun(false) do |fn|
      @f.print cs_proto(fn)
      @f.puts "\t\t{", "\t\t\t// Place ton code ici", "\t\t}"
    end

    @f.puts "\t}", "}"
    @f.close

    generate_makefile
  end
end
