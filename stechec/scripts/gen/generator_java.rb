# -*- ruby -*-
# Stechec project is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `NOTICE' file in the root directory.
#
# Copyright (C) 2005, 2006, 2007 Prologin
#


# C++ generator, for java-interface
class JavaCxxFileGenerator < CxxProto

  def cxx_type(type)
    # Returns the C++ type for the provided type.
    # Only really useful for arrays.
    if type.is_array?
      type.type.name
    else
      type.name
    end
  end

  def native2jtype(t)
    if t.is_simple? then
      if not t.is_nil? then
        {
          "bool" => "jboolean",
          "int" => "jint",
          "long" => "jlong"
        }[t.name]
      else
        "void"
      end
    else
      if t.is_array? then
        "JArray< #{t.type.name.capitalize()}* >*"
      else
        t.name.capitalize() + "*"
      end
    end
  end

  def initialize
    super
    @lang = "C++ (for java interface)"
  end

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_java.rb"
    @f.puts "#include <vector>"
    @f.puts "#include <java/util/ArrayList.h>"
    $conf['struct'].each do |x|
        @f.puts "#include \"#{x['str_name'].capitalize}.h\""
    end
    $conf['enum'].each do |x|
       @f.puts "#include \"#{x['enum_name'].capitalize}.h\""
    end
    @f.puts "#include \"#{@java_file}.h\""
    @f.puts "#include \"#{@java_interface}.h\""

    build_constants
    build_enums
    build_structs
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |f| @f.print CxxProto.new.cxx_proto(f, "api_", ""); @f.puts ";" }
    for_each_user_fun { |f| @f.print CxxProto.new.cxx_proto(f, "", ""); @f.puts ";" }
    @f.puts "}"
    @f.close
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_java.rb"
    package = "prologin/java"
    # Needed includes
    @f.puts "
// we want to use the CNI
#include <gcj/cni.h>
#include <gcj/array.h>
#include <java/lang/Throwable.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <stdio.h>

#include \"interface.hh\"


template<typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.error;
}

template<>
int lang2cxx<jint, int>(jint in)
{
  return in;
}
template<>
bool lang2cxx<jboolean, bool>(jboolean in)
{
  return in;
}

template<typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.err;
}

template<>
jint cxx2lang<jint, int>(int in)
{
  return in;
}
template<>
jboolean cxx2lang<jboolean, bool>(bool in)
{
  return in;
}

"
    for_each_enum do |e|
      name = e['enum_name']
      fields = e['enum_field'][0][0].capitalize()
      @f.puts "template<>"
      @f.puts "#{name} lang2cxx<#{name.capitalize()}*, #{name} >(#{name.capitalize()}* in)", "{"
      @f.puts "  return #{name}(lang2cxx<jint, int>( in->ordinal() ));", "}"

      @f.puts "template<>"
      @f.puts "#{name.capitalize}* cxx2lang<#{name.capitalize()}*, #{name} >(#{name} in)", "{"
      @f.puts "  return *(&#{name.capitalize}::#{fields} + (int)in);", "}"
    end

    for_each_struct do |struct|
      name = struct["str_name"]
      name_java = name.capitalize
      fields = struct["str_field"]

      #generation of lang2cxx
      @f.puts "template <>"
      @f.puts "#{name} lang2cxx<#{name.capitalize()}* , #{name} >(#{name.capitalize()}* in){"
      @f.puts "  #{name} out;"
      fields.each do |f|
        field = f[0]
        type = @types[f[1]]
        jtype = native2jtype(type)
        cxxtype = cxx_type(type)
        @f.print "  out.#{field} = "
        @f.puts get_lang2cxx(type, jtype, cxxtype) + "(in->#{field});"
      end
      @f.puts "  return out;\n}\n"

      #generation of cxx2lang
      @f.puts "template <>"
      @f.puts "#{name.capitalize()}* cxx2lang<#{name.capitalize()}*, #{name}>(#{name} in){"
      @f.puts "  #{name.capitalize()}* klass = new #{name.capitalize()}();"
      fields.each do |f|
        field = f[0]
        type = @types[f[1]]
        jtype = native2jtype(type)
        cxxtype = cxx_type(type)
        @f.puts "  klass->#{field} = " + get_cxx2lang(type, jtype, cxxtype) + "(in.#{field});"
      end
      @f.puts "  return klass;\n}"

    end

    @f.puts <<-EOF
template<class Lang, class Cxx>
std::vector<Cxx> lang2cxx_array(JArray<Lang*>* in)
{
  std::vector<Cxx> vect;
  vect.resize(lang2cxx<jint, int>(in->length));
  Lang** T = elements(in);

  for (int i = 0; i < vect.size(); i++)
    vect[i] = lang2cxx<Lang*, Cxx>(T[i]);

  return vect;
}

template<class Lang, class Cxx>
JArray<Lang*>* cxx2lang_array(const std::vector<Cxx>& in)
{
  JArray<Lang*>* out = ( JArray<Lang* > * ) JvNewObjectArray(in.size(), &Lang::class$ , NULL);
  Lang** T = elements(out);

  for (int i = 0; i < in.size(); i++)
    T[i] = cxx2lang<Lang*, Cxx>(in[i]);

  return out;
}
    EOF

    # Implementing methods generated by Interface.java, in C++
    build_helper_funcs()

    # The java virtual machine.
    @f.puts <<-EOF
extern void GC_disable();

struct ProloginJavaVm
{
  ProloginJavaVm()
  {
    using namespace java::lang;
    try
    {
      // create the virtual machine
      JvCreateJavaVM(NULL);
      JvAttachCurrentThread(NULL, NULL);
      // FIXME: grrrr
      // GC_disable();
      c = new #{@java_file}();
    }
    catch (Throwable *t)
    {
      System::err->println(JvNewStringLatin1("Unhandled Java exception:"));
      t->printStackTrace();
    }
  }

  ~ProloginJavaVm()
  {
    // destroy the virual machine
    JvDetachCurrentThread();
  }

  #{@java_file}* c;

};

struct ProloginJavaVm javaVm;
    EOF

    for_each_user_fun do |f|
      name = f.name
      @f.print cxx_proto(f, "", 'extern "C"')
      @f.puts "", "{"
      @f.puts "  try {"
      @f.print "return #{get_lang2cxx(f.ret, native2jtype(f.ret), cxx_type(f.ret))}(" if not f.ret.is_nil?
      @f.print "    javaVm.c->" + name + "()"
      @f.print ")" if not f.ret.is_nil?
      @f.puts ";"
      @f.puts "  } catch (java::lang::Throwable *t) {"
      @f.puts '    fprintf(stderr, "Unhandled Java exception:\n");'
      @f.puts "    t->printStackTrace();", "  }"
      @f.puts "}"
    end

    @f.close
  end

  def get_cxx2lang(type, arg1, arg2)
    if type.is_array? then
        "cxx2lang_array< #{arg2.capitalize()}, #{arg2} >"
    else
      if type.is_nil? then
        ""
      else
        "cxx2lang<#{arg1}, #{arg2} >"
      end
    end
  end

  def get_lang2cxx(type, arg1, arg2)
    if type.is_array? then
        "lang2cxx_array< #{arg2.capitalize()}, #{arg2} >"
    else
      if type.is_nil? then
        ""
      else
        "lang2cxx<#{arg1}, #{arg2} >"
      end
    end
  end

  def build_helper_funcs
     for_each_fun(false) do |fn|
       @f.print "#{native2jtype(fn.ret)} Interface::#{fn.name}("
       args = fn.args.map { |arg| "#{native2jtype(arg.type)} #{arg.name}" }
       @f.puts args.join(", ") + ")"
       @f.puts "{"
       @f.print "\t"
       @f.print "return #{get_cxx2lang(fn.ret, native2jtype(fn.ret), cxx_type(fn.ret))}(" if not fn.ret.is_nil?
       @f.print "api_" + fn.name + "("
        args = fn.args.map { |arg|
            get_lang2cxx(arg.type, native2jtype(arg.type), cxx_type(arg.type)) + "(#{arg.name})" }
       @f.print args.join(", ") + ")"
       @f.print ")" if not fn.ret.is_nil?
       @f.puts ";\n}"
     end
  end

  def build
    @path = Pathname.new($install_path) + "java"
    @header_file = 'interface.hh'
    @source_file = 'interface.cc'
    @java_interface = 'Interface'
    @java_file = $conf['conf']['player_filename'].capitalize

    generate_header
    generate_source
  end
end


class JavaFileGenerator < FileGenerator
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

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each_line {|s| @f.print '// ', s }
    @f.puts ""
  end

  # print a constant
  def print_constant(type, name, val)
    @f.print '  public static final int ', name, ' = ', val, ";\n"
  end

  # print a java prototype
  def print_proto(prefix, f)
    name = f.name
    ret_type = f.ret
    args = f.args
    @f.print prefix, " ", conv_java_type(ret_type), " ", name, "("
    if args != nil and args != []
      str_args = args.map do |arg|
        "#{conv_java_type(arg.type)} #{arg.name}"
      end
      @f.print "#{ str_args.join(", ")}"
    end
    @f.print ")"
  end

  def conv_java_type(x)
    if x.is_a?(String) then t = @types[x] else t = x end
    conv_java_type_aux(t, false)
  end
  def conv_java_type_aux(t, in_generic)
    if t.is_array?
    then
      "#{ conv_java_type_aux(t.type, true) }[]"
    else
      if t.is_struct? then
        t.name.capitalize()
      else
        if t.is_simple? then
          if in_generic then
            (@java_obj_types[t.name]).capitalize()
          else
            @java_types[t.name]
          end
        else
          t.name.capitalize()
        end
      end
    end
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
    for_each_struct do |x|
      name = x['str_name'].capitalize
      @f.puts "class #{name} {"
      x["str_field"].each do |f|
        @f.puts "  public #{conv_java_type(f[1])} #{f[0]}; // #{f[2]}"
      end
      @f.puts "}"
    end

    for_each_enum do |x|
      name = x['enum_name'].capitalize
      @f.puts "enum #{name}{"
      x['enum_field'].each do |f|
        name = f[0].downcase
        @f.puts "  #{name.capitalize()}, // <- #{f[1]}"
      end
      @f.puts "}"
    end

    @f.puts "public class Interface", "{"
    build_constants
    for_each_fun do |f|
      print_proto("  public static native", f)
      @f.puts ";"
    end
    @f.puts "}"
    @f.close

    #####################################
    ##  Champion.java file generating  ##
    #####################################
    @f = File.new(@path + (@java_file + '.java'), 'w')
    print_banner "generator_java.rb"
    # generate functions bodies
    @f.puts "public class #{@java_file} extends #{@java_interface}", "{"
    for_each_user_fun do |f|
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
