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

  def generate_header
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_ruby.rb"
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

  def generate_source
    #
    # Cxx interface file
    #
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_ruby.rb"
    print_include "ruby.h", true
    print_include "vector", true
    print_include "interface.hh"
@f.puts "
/*
** execute 'str', protected from possible exception, ...
*/
VALUE my_rb_eval_string(VALUE str)
{
  rb_eval_string((const char*) str);
  return T_NIL;
}

template<typename Cxx>
VALUE cxx2lang(Cxx in)
{
}

template<typename Cxx>
Cxx lang2cxx(VALUE in)
{
}

template<>
int lang2cxx<int>(VALUE in)
{
  return FIX2INT(in);
}

template<>
bool lang2cxx<bool>(VALUE in)
{
  return (in == T_TRUE)? true : false;
}

template<typename Cxx>
std::vector<Cxx> lang2cxx_array(VALUE *l)
{
  VALUE *tab = RARRAY_LEN(l);
  int len = RARRAY_PTR(l);
  std::vector<Cxx> vect;
  for (int i = 0; i < len; i ++){
    vect.push_back( cxx2lang<Cxx>( tab[i] ) );
  }
  return vect;
}

template<>
VALUE cxx2lang<int>(int in)
{
  return INT2FIX(in);
}

template<>
VALUE cxx2lang<bool>(bool in)
{
  return in?T_TRUE:T_FALSE;
}

template<typename Cxx>
VALUE cxx2lang_array(const std::vector<Cxx>& vect)
{
  long len = vect.size();
  VALUE ary = rb_ary_new();
  for (int i = 0; i < len; ++i){
    rb_ary_push(ary, cxx2lang<Cxx>(vect[i]) );
  }
  return ary;
}
"

    for_each_enum do |e|
      ty = e['enum_name'];
      fields = e['enum_field']

      @f.puts "template<>"
      @f.puts "#{ty} lang2cxx<#{ty}>(VALUE in)\n{"
      @f.puts "char* v = RSTRING_PTR(in);"
      fields.each do |f|
        @f.puts "  if (strcmp(v, \"#{f[0]}\") == 0)"
        @f.puts "      return #{f[0].upcase};"
      end
      @f.puts "}\n"

      @f.puts "template<>"
      @f.puts "VALUE cxx2lang<#{ty}>(#{ty} in)\n{"
      @f.puts "  switch (in)\n  {"
      fields.each do |f|
        @f.puts "    case #{f[0].upcase}:"
        @f.puts "      return rb_str_new(\"#{f[0]}\", #{f[0].length});"
      end
      @f.puts "  }\n}\n"
    end


    for_each_struct do |s|
      type = s["str_name"]
      @f.puts "template<>"
      @f.puts "#{type} lang2cxx<#{type}>(VALUE in)\n{"
      @f.puts "  #{type} out ;"
      s['str_field'].each do |f|
        name =f[0]
        type = @types[f[1]]
        if type.is_array? then
          type = type.type.name
          fun = '_array'
        else
          type = type.name
          fun = ''
        end
        @f.puts "  VALUE #{name} = rb_iv_get(in, \"@#{name}\");"
        @f.puts "  out.#{name} = lang2cxx#{fun}<#{type}>(#{name});"
      end
      @f.puts "  return out;"
      @f.puts "}\n"
      type = s["str_name"]
      @f.puts "template<>"
      @f.puts "VALUE cxx2lang<#{type}>(#{type} in)\n{"
      args = s['str_field'].map do |f|
        name =f[0]
        type = @types[f[1]]
        if type.is_array? then
          type = type.type.name
          fun = '_array'
        else
          type = type.name
          fun = ''
        end
        o = "cxx2lang#{fun}<#{type}>(in.#{name})"
      end
      @f.puts "  VALUE argv[] = {#{args.join ', '}};"
      @f.puts "  int argc = #{args.size };"
      @f.puts "  ID class_id = rb_intern(\"#{type}\");"
      @f.puts "  VALUE class_ = rb_const_get(rb_cObject, class_id);"
      @f.puts "  VALUE out = rb_class_new_instance(argc, argv, class_);"
      @f.puts "  return out;"
      @f.puts "}"

    end
    # create callbacks
    for_each_fun(false) do |fn|
      name = fn.name
      type_ret = fn.ret
      args = fn.args
      args_str = (args.map do |arg| "VALUE #{arg.name}" end).join ', '
      @f.print "VALUE rb_#{name}(#{args_str})"
      s_args = ""
      if args != []
        args_str = args.map do |y|
          if y.type.is_array? then
            arr = '_array'
          else
            arr = ''
          end
          fun_name = "lang2cxx#{arr}<#{y.type.name}>"
          fun_name+"(" + y.name + ")"
        end
        s_args = args_str.join ', '
      end
      out = "api_#{name}(#{s_args})"
      if type_ret.is_nil? then
        print_body out+";"
      else
        if type_ret.is_array? then
          arr = '_array'
        else
          arr = ''
        end
        ret_fun_name = "cxx2lang#{arr}<#{type_ret.name}>"
        print_body "  return #{ret_fun_name}(#{out});"
      end
    end

    @f.puts "

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
    RUBY_INIT_STACK;
    // load ruby and evaluate candidat's file
    ruby_init();
    ruby_init_loadpath();

    char* file = \"#{$conf['conf']['player_filename']}.rb\";
    ruby_script(file);
    void* node = rb_load_file(file);
    status = ruby_run_node(node);
    if (status)
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
    @header_file = 'interface.hh'

    generate_header
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
    for_each_struct do |s|
      args = s['str_field'].map do |f| f[0] end
      @f.puts "def #{s['str_name']}"
      @f.puts "  def initialize(#{args.join ', '})"
      s['str_field'].each do |f|
        @f.puts "    @#{f[0]} = #{f[0]}"
      end
      @f.puts "  end"
      @f.puts "end"
    end


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

