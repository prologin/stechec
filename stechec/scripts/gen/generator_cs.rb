#
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

# C++ generator, for C# interface
class CSharpCxxFileGenerator < CSharpProto
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
  void callCSharpMethod(const char* name);

private:
  typedef std::map<std::string, MonoMethod*> UserMethodsMap;

  MonoDomain*		_domain;
  MonoAssembly*		_assembly;
  MonoImage*		_image;
  MonoClass*		_class;
  UserMethodsMap	_user_methods;
};

    EOF

    build_constants
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |x, y, z| print_proto(x, y, z, "extern"); @f.puts ";" }
    for_each_user_fun { |x, y, z| print_proto(x, y, z); @f.puts ";" }
    @f.puts "}", "", "#endif // !INTERFACE_HH_"
    @f.close
  end

  def generate_api()
    @f = File.open(@path + @api_file, 'w')
    print_banner "generator_cs.rb"

    @f.puts "using System.Runtime.InteropServices;", "using System.Runtime.CompilerServices;", ""
    @f.puts "namespace Prologin {"
    @f.puts "\tclass API {"

    for_each_fun(false) do |name, type_ret, args|
      @f.puts "\t\t[MethodImplAttribute(MethodImplOptions.InternalCall)]"
      print_proto(name, type_ret, args, "\t\tpublic static extern");
      @f.puts ";"
    end

    @f.puts "\t}", "}"
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_cs.rb"
    
    @f.puts <<-EOF
#include "interface.hh"

#include <iostream>
#include <assert.h>
#include <cstdlib>
#define LOG std::cerr

CSharpInterface gl_csharp;

/*
** Inititialize Mono and load the DLL file.
*/
CSharpInterface::CSharpInterface()
{
  const char*		champion_path = getenv("CHAMPION_PATH");
  std::string		champion;
  MonoMethodDesc*	method_desc;

  if (!champion_path)
    champion = "./prologin.dll";
  else
  {
    champion = champion_path;
    champion += "/prologin.dll";
  }

  LOG << "[C# Wrapper] Initializing" << std::endl;
  _domain = mono_jit_init(champion.c_str());
  assert(_domain != NULL);

  _assembly = mono_domain_assembly_open(_domain, champion.c_str());
  assert(_assembly != NULL);

  _image = mono_assembly_get_image(_assembly);
  assert(_image != NULL);

  _class = mono_class_from_name(_image, "Prologin", "Prologin");
  assert(_class != NULL);

  const char* method_names[] = {
    EOF

    for_each_user_fun(false) do |name, ret, args|
      @f.print "  \"Prologin:", name, "\","
    end
    @f.puts <<-EOF
    NULL
  };

  MonoMethod* method = NULL;

  // Get MonoMethod* hanldes to user functions
  for (int i = 0; method_names[i] != NULL; ++i)
  {
    method_desc = mono_method_desc_new(method_names[i], false);
    method = mono_method_desc_search_in_class(method_desc, _class);
    assert(method != NULL);
    _user_methods[method_names[i]] = method;
  }

  // Register API functions as internal Mono functions
     EOF

     for_each_fun(false) do |name, ret, args|
       @f.print "      mono_add_internal_call(\"Prologin.API::" + name + "\", (const void*)" + name + ");"
     end

     @f.puts <<-EOF
  LOG << "[C# Wrappper] done." << std::endl;
}

CSharpInterface::~CSharpInterface()
{
  LOG << "[C# Wrapper] cleaning up." << std::endl;
  mono_image_close(_image);
  mono_assembly_close(_assembly);
  mono_jit_cleanup(_domain);
}

/*
** Calls C# functions from C++
*/
void CSharpInterface::callCSharpMethod(const char* name)
{
  MonoMethod*	method;
  void** args = { NULL };
  UserMethodsMap::iterator iter;

  iter = _user_methods.find(name);
  if (iter == _user_methods.end())
    return ;
  method = (*iter).second;
  LOG << "[C# Wrapper] Invoking user-method " << name << std::endl;
  mono_runtime_invoke(method, NULL, args, NULL);
}

/*
** Functions called from stechec to C.
*/
    EOF

    for_each_user_fun(false) do |name, type_ret, args|
      print_proto(name, type_ret, args, 'extern "C"')
      print_body "  gl_csharp.callCSharpMethod(\"Prologin:" + name + "\");"
     end
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "cs"
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'
    @api_file = 'api.cs'

    generate_header
    generate_source
    generate_api
  end

end

class CSharpFileGenerator < FileGenerator
  def initialize
    @lang = "cs"
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each {|s| @f.print '// ', s }
    @f.puts ""
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

    ######################################
    ##  interface.hh file generating    ##
    ######################################
    CSharpCxxFileGenerator.new.build

    ######################################
    ##  prologin.cs file generating     ##
    ######################################
    @f = File.new(@path + @source_file, 'w')
    print_banner "generator_cs.rb"
    # Required stuff to call C from C#
    @f.puts "using System.Runtime.InteropServices;", "using System;", ""

    @f.puts "namespace Prologin {"
    @f.puts "\tclass Prologin {"

    # User methods (play_turn, etc.)
    for_each_user_fun do |name, ret, args|
      @f.puts "\t\tstatic void " + name + "()", "\t\t{"
      @f.puts "\t\t\t// Place ton code ici"
      @f.puts "\t\t}"
    end

    @f.puts "\t}"
    @f.puts "}"
    @f.close

    generate_makefile
  end
end
