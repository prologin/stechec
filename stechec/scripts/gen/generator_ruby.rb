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

require "gen/file_generator"

# C++ generator, for java-interface
class RubyCxxFileGenerator < CxxProto
  def initialize
    super
    @lang = "C++ (for ruby interface)"
    @rb_types = {
      'void' => 'VALUE',
      'int' => 'VALUE',
      'bool' => 'VALUE'
    }
  end

  def generate_source
    #
    # Cxx interface file
    #
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_ruby.rb"
    print_include "ruby.h", true
    @f.puts "
extern \"C\" {

/*
** execute 'str', protected from possible exception, ...
*/
VALUE my_rb_eval_string(VALUE str)
{
  rb_eval_string((const char*) str);
  return T_NIL;
}

"

    # create callbacks
    for_each_fun(false) do |fn|
      name = fn.name
      type_ret = fn.ret
      args = fn.args
      @f.puts cxx_proto fn, "rb_", 'extern "C"'
      s_args = ""
      if args != []
        args[0..-2].each do |y|
          s_args += "FIX2INT(" + y.name + "), "
        end
        s_args += "FIX2INT(" + args[-1].name + ")"
      end
      print_body "  return INT2FIX(#{name}(#{s_args}));"
    end

    @f.puts "
}

struct RubyVm
{
  void loadCallback()
  { 
"

    # configure callbacks in the Ruby environment
    for_each_fun do |x|
      args = x.args
      fct_name = x.name
      l = args ? args.nitems : 0
      @f.puts "    rb_define_global_function(\"rb_#{fct_name}\", (VALUE(*)(...))(rb_#{fct_name}), #{l});"
    end

    @f.puts "
  }

  RubyVm()
  {
    int status;
    // load ruby and evaluate candidat's file
    ruby_init();
    ruby_init_loadpath();
    rb_load_file(\"#{$conf['conf']['player_filename']}.rb\");
    if ((status = ruby_exec()))
    {
      fprintf(stderr, \"failed to load ruby code (%d)\\n\", status);
      exit(1);
    }
    rb_protect(my_rb_eval_string, (VALUE) \"game = Prologin.new\", &status);
    if (status)
    {
      fprintf(stderr, \"error while instantiating Prologin class\\n\");
      exit(1);
    }
    loadCallback();
  }

  ~RubyVm()
  {
    // finalize (don't seem to work well)
    int status = 0;
    status = ruby_cleanup(status);
    ruby_finalize();
  }
} rubyVm;

"
  
    @f.puts "extern \"C\" {"
    for_each_user_fun do |fn|
      name = fn.name
      ret = fn.ret
      args = fn.args
      @f.print cxx_proto(fn)
      print_body "  int status;
  rb_protect(my_rb_eval_string, (VALUE) \"game.#{name}\", &status);
  if (status)
    fprintf(stderr, \"error while calling ruby function: #{name} (%d)\\n\", status);
"
    end
    @f.puts "}"
    @f.close
  end

  def build
    @path = Pathname.new($install_path) + "ruby"
    @source_file = 'interface.cc'

    generate_source
  end

end

class RubyFileGenerator < CProto

  def print_comment(str)
    @f.puts '# ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each {|s| @f.print '# ', s }
    @f.puts ""
  end

  def print_constant(type, name, val)
      @f.print name, ' = ', val, "\n"
  end

  def generate_source()

    #
    # Ruby contants file
    #
    @f = File.open(@path + @ruby_constant_file, 'w')
    print_banner "generator_ruby.rb"
    build_constants
    @f.close

    #
    # Ruby main file
    #
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_ruby.rb"
    @f.puts "require '#{@ruby_constant_file}'"
    @f.puts
    @f.puts "class Prologin"
    for_each_user_fun(false) do |name, type_ret, args|
      @f.print "  def ", name, "\n", "    # fonction a completer\n", "  end\n" 
    end
    @f.puts "end"
    @f.close
  end

  def generate_makefile
    @f = File.open(@path + "Makefile", 'w')
    @f.print <<-EOF
#
# Makefile
#

NAME      = #{$conf['conf']['player_lib']}.so

include ../includes/makeruby
    EOF
    @f.close
  end


  def build()
    @path = Pathname.new($install_path) + "ruby"
    @ruby_constant_file = 'constants.rb'
    @source_file = $conf['conf']['player_filename'] + '.rb'

    RubyCxxFileGenerator.new.build
    generate_source
    generate_makefile
  end
end

