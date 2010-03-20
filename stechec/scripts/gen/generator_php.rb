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

# include <cstdlib>
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

  def generate_fun(f)
    @f.puts "PHP_FUNCTION(php_api_#{f.name})"
    @f.puts "{"
    @f.puts "    RETURN_NULL();"
    @f.puts "}"
  end

  def generate_user_fun(f)
    @f.puts cxx_proto(f, "", 'extern "C"')
    @f.puts "{"
    @f.puts "    zval ret;"
    @f.puts "    zval fname;"
    if f.args.length > 0 then
      @f.puts "    zval params[#{f.args.length}];"
    end
    @f.puts "    _init_php();"
    @f.puts
    @f.puts "    MAKE_STD_ZVAL(ret);"
    @f.puts "    ZVAL_STRING(&fname, \"#{f.name}\", 0);"

    i = 0
    f.args.each do |a|
      @f.puts "    params[#{i}] = cxx2lang<zval, #{cxx_type(a.type)}>(#{a.name});"
      i += 1
    end

    p = if f.args.length != 0 then "params" else "NULL" end
    @f.puts "    if (call_user_function(EG(function_table), NULL, &fname, ret, #{i}, #{p} TSRMLS_CC) == FAILURE)"
    @f.puts "    {"
    @f.puts "        abort();"
    @f.puts "    }"

    if not f.ret.is_nil? then
      @f.puts "    #{cxx_type(f.ret)} cxxret = lang2cxx<zval, #{cxx_type(f.ret)}>(ret);"
    elsif f.ret.is_array? then
      @f.puts "    #{cxx_type(f.ret)} cxxret = lang2cxx_array<#{cxx_type(f.ret.type)}>(ret);"
    end

    @f.puts "    zval_dtor(ret);"
    @f.puts "    zval_dtor(&fname);"
    i = 0
    f.args.each do |a|
      @f.puts "    zval_dtor(params[#{i}]);"
      i += 1
    end
    if not f.ret.is_nil?
      @f.puts "    return cxxret;"
    end
    @f.puts "}"
  end

  def generate_source
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_php.rb"

    @f.puts <<-EOF
#include "interface.hh"

static void _init_php();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
zval* cxx2lang<zval*, int>(int in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    ZVAL_LONG(x, in);
    return x;
}

template <>
zval* cxx2lang<zval*, bool>(bool in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    ZVAL_BOOL(x, in);
    return x;
}

template <typename Cxx>
zval* cxx2lang_array(const std::vector<Cxx>& in)
{
    zval* x;
    MAKE_STD_ZVAL(x);
    array_init(x);

    size_t s = in.size();

    for (size_t i = 0; i < s; ++i)
      add_next_index_zval(x, cxx2lang<zval*, Cxx>(in[i]));

    return x;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<zval*, int>(zval* in)
{
    int r;
    zend_vm_stack_push(in);
    if (zend_parse_parameters(1 TSRMLS_CC, "l", &r) == FAILURE)
        abort();
    zend_vm_stack_pop();
    return r;
}

template <>
bool lang2cxx<zval*, bool>(zval* in)
{
    bool r;
    zend_vm_stack_push(in);
    if (zend_parse_parameters(1 TSRMLS_CC, "b", &r) == FAILURE)
        abort();
    zend_vm_stack_pop();
    return r;
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(zval* in)
{
    HashTable* ht = Z_ARRVAL_P(in);
    std::vector<Cxx> out;
    size_t s = zend_hash_num_elements(ht);

    for (size_t i = 0; i < s; ++i)
    {
        zval* v;
        zend_hash_index_find(ht, i, &v);
        out.push_back(lang2cxx<zval*, Cxx>(v));
    }

    return out;
}
EOF

    for_each_fun do |f|
      generate_fun(f)
    end

    @f.puts <<-EOF
static function_entry module_functions_table[] = {
EOF
    for_each_fun(false) do |f|
      @f.print "    PHP_FALIAS(#{f.name}, php_api_#{f.name}, NULL)"
    end
    @f.puts <<-EOF
    {NULL, NULL, NULL}
};
EOF

    @f.puts <<-EOF
static zend_module_entry api_module_entry = {
    STANDARD_MODULE_HEADER,
    "api",
    module_functions_table,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "1.0",
    STANDARD_MODULE_PROPERTIES
};

static void _init_php()
{
    static int initialized = 0;
    if (initialized)
        return;
    initialized = 1;

    char* argv[] = { "#{$conf['conf']['player_lib']}", NULL };
    char buffer[1024];
    const char* path;
    zend_file_handle script;

    path = getenv("CHAMPION_PATH");
    if (!path)
        path = ".";

    snprintf(buffer, 1024, "include('%s/%s.php');", path, "#{$conf['conf']['player_filename']}");

    php_embed_init(1, argv PTSRMLS_CC);
    zend_startup_module(&api_module_entry);
    zend_eval_string(buffer, NULL, "PHP to Stechec interface");
}

EOF

    for_each_user_fun do |f|
      generate_user_fun(f)
    end

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

  def generate_makefile
    target = $conf['conf']['player_lib']
    @f = File.open(@path + 'Makefile', 'w')
    @f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

# Tu peux rajouter des fichiers sources ici
#{target}-dists = #{@source_file}

# Evite de toucher a ce qui suit
#{target}-dists += api.php interface.hh
#{target}-srcs = interface.cc
#{target}-cxxflags = -ggdb3 $(shell php-config --includes)
#{target}-ldflags = -s $(shell php-config --libs --ldflags)

include ../includes/rules.mk
EOF
    @f.close
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

    generate_makefile
  end
end
