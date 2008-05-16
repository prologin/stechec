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
    @types = @types_gcj

    # Implementing methods generated by Interface.java, in C++
    for_each_fun do |name, type_ret, args|
      print_proto(" Interface::" + name, type_ret, args)
      @f.puts "", "{"
      @f.print "  "
      @f.print "return" if type_ret && type_ret != "void"
      @f.print "::", name, "("
      if args != nil
        args[0..-2].each do |y|
          @f.print y[0], ", "
        end
        @f.print args[-1][0]
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
    for_each_user_fun do |name, type_ret, args|
      print_proto(name, type_ret, args, 'extern "C"')
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
    @lang = "java"
    @types = {
      'void' => 'void',
      'int' => 'int',
      'bool' => 'boolean'
    }
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    str.each {|s| @f.print '// ', s }
    @f.puts ""
  end

  # print a constant
  def print_constant(type, name, val)
      @f.print '  public static final int ', name, ' = ', val, ";\n"
  end

  # print a java prototype
  def print_proto(prefix, name, ret_type, args)
    @f.print prefix, " ", ret_type, " ", name, "("
    if args != nil and args != []
      args[0..-2].each do |arg|
        @f.print conv_type(arg[1]), " ", arg[0], ", "
      end
      @f.print conv_type(args[-1][1]), " ", args[-1][0]
    end
    @f.print ")"
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

    @f.puts "public class Interface", "{"
    build_constants
    for_each_fun do |x, y, z|
      print_proto("  public static native", x, conv_type(y), z)
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
    for_each_user_fun do |name, ret, args|
      @f.print "  "
      print_proto("public", name, conv_type(ret), args)
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
#{target}-srcs += interface.cc ../includes/main.c
include ../includes/rules.mk
    EOF
    @f.close
  end

end
