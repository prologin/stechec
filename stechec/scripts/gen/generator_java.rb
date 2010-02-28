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

  def initialize
    super
    @lang = "C++ (for java interface)"
    @types_orig = @types
    @types_gcj = {
      'void' => 'void',
      'int' => 'jint',
      'bool' => 'jboolean'
    }
  end

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_java.rb"
    build_constants
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |x, y, z| print_proto(x, y, z, "extern"); @f.puts ";" }
    for_each_user_fun { |x, y, z| print_proto(x, y, z); @f.puts ";" }
    @f.puts "}"
    @f.close
  end

  def generate_source()
    @f = File.open(@path + @source_file, 'w')
    print_banner "generator_java.rb"
 
    # Needed includes
    @f.puts "
// we want to use the CNI
#include <gcj/cni.h>
#include <java/lang/Throwable.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <stdio.h>

#include \"#{@java_file}.h\"
#include \"#{@java_interface}.h\"

#include \"interface.hh\"

"
    # Switch types
    #@types = @types_gcj # TODO

    # Implementing methods generated by Interface.java, in C++
    for_each_fun do |f|
      name = f.name
      type_ret = f.ret
      args = f.args
      @f.print cxx_proto(f, " Interface::")
      @f.puts "", "{"
      @f.print "  "
      @f.print "return" if type_ret && type_ret != "void"
      @f.print "::", name, "("
      if args != []
        args[0..-2].each do |y|
          @f.print y.name, ", "
        end
        @f.print args[-1].name
      end
      @f.puts ");", "}", ""
     end

    # The java virtual machine.
    @f.puts <<-EOF
extern void GC_disable();

struct JavaVm
{
  JavaVm() : c(0)
  {
    using namespace java::lang;
    try
      {
	// create the virtual machine
	JvCreateJavaVM(NULL);
        // FIXME: grrrr
	// GC_disable();
	JvAttachCurrentThread(NULL, NULL);
	c = new #{@java_file}();
      }
    catch (Throwable *t)
      {
	System::err->println(JvNewStringLatin1("Unhandled Java exception:"));
	t->printStackTrace();
      }
  }
  
  ~JavaVm()
  {
    // destroy the virual machine
    JvDetachCurrentThread();
  }

  #{@java_file}* c;

} javaVm;


    EOF

    # Implementing C -> java glue
    @types = @types_orig
    for_each_user_fun do |f|
      name = f.name
      @f.print cxx_proto(f, "", 'extern "C"')
      @f.puts "", "{"
      @f.puts "  try {"
      @f.puts "    javaVm.c->" + name + "();"
      @f.puts "  } catch (java::lang::Throwable *t) {"
      @f.puts '    fprintf(stderr, "Unhandled Java exception:\n");'
      @f.puts "    t->printStackTrace();", "  }"
      @f.puts "}"
    end

    @f.close
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
      "java.utils.ArrayList< #{ conv_java_type_aux(t.type, true) } >"
    else
      if t.is_struct? then
        t.name.capitalize()
      else
        if t.is_simple? then
          if in_generic then
            conv = @java_obj_types
          else
            conv = @java_types
          end
          name = conv[t.name]
          name.capitalize()
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
      @f.puts "protected class #{name} {"
      x["str_field"].each do |f|
        @f.puts "  public #{conv_java_type(f[1])} #{f[0]}; // #{f[2]}"
      end
      @f.puts "}"
    end

    for_each_enum do |x|
      name = x['enum_name'].capitalize
      @f.puts "protected enum #{name}{"
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

# Tu peux rajouter des fichiers .ml ou changer les flags de g++
#{target}-srcs = Interface.java Prologin.java
#{target}-cxxflags = -I. -ggdb3

# Evite de toucher a ce qui suit
#{target}-dists = interface.hh
#{target}-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
    EOF
    @f.close
  end

end
