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

  def native2jtype(t)
    if t.is_simple? then
      {
        "bool" => "jboolean",
        "int" => "jint",
        "long" => "jlong"
      }[t.name]
    else
      "jobject"
    end
  end

  def initialize
    super
    @lang = "C++ (for java interface)"
  end

  def generate_header()
    @f = File.open(@path + @header_file, 'w')
    print_banner "generator_java.rb"
    build_constants
    @f.puts "", 'extern "C" {', ""
    for_each_fun { |f| @f.print cxx_proto(f, "", "extern \"C\""); @f.puts ";" }
    for_each_user_fun { |f| @f.print cxx_proto(f, "", ""); @f.puts ";" }
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
#include <java/lang/Throwable.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <stdio.h>

#include \"#{@java_file}.h\"
#include \"#{@java_interface}.h\"

#include \"interface.hh\"



template<typename Lang, typename Cxx>
Cxx lang2cxx(JNIEnv *env, Lang in)
{
  return in.error;
}

template<>
int lang2cxx<jint, int>(JNIEnv *env, int in)
{
  return in;
}
template<>
bool lang2cxx<jboolean, bool>(JNIEnv *env, bool in)
{
  return in;
}

template<typename Cxx>
std::vector<Cxx> lang2cxx_array(JNIEnv *env, jobject l)
{
  std::vector<Cxx> vect;
  int len = *( ((int *)l) -1 );
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back( lang2cxx<Lang, Cxx>(l[i]) );
return vect;
}

template<typename Lang, typename Cxx>
Lang cxx2lang(JNIEnv *env, Cxx in)
{
  return in.err;
}

template<>
int cxx2lang<int, int>(JNIEnv *env, int in)
{
  return in;
}
template<>
bool cxx2lang<bool, bool>(JNIEnv *env, bool in)
{
  return in;
}

template<typename Cxx>
jobject cxx2lang_array(JNIEnv *env, const std::vector<Cxx>& vect)
{
   size_t len = vect.size();
   class stringClass;
   jmethodID cid;
   jcharArray elemArr;
   jstring result;
   stringClass = (*env)->FindClass(env, \"java/util/ArrayList\");
   assert (stringClass != NULL);
   cid = (*env)->GetMethodID(env, stringClass, \"<init>\", \"(I)V\");
   assert(cid != NULL);
   result = (*env)->NewObject(env, stringClass, cid, len);
   for (int i = 0; i < len; ++i){

  }
  (*env)->DeleteLocalRef(env, stringClass);
  return result;
}

template <>
jbool getField<jbool>(JNIEnv *env, jobject obj, jfieldID fid){
  return (*env)->getBoolField(env, obj, fid);
}


template <>
jlong getField<jlong>(JNIEnv *env, jobject obj, jfieldID fid){
  return (*env)->getLongField(env, obj, fid);
}

template <>
jint getField<jint>(JNIEnv *env, jobject obj, jfieldID fid){
  return (*env)->getIntField(env, obj, fid);
}

template <>
jobject getField<jobject>(JNIEnv *env, jobject obj, jfieldID fid){
  return (*env)->getObjectField(env, obj, fid);
}

void setField(JNIEnv *env, jobject obj, jfieldID fid, jbool b){
  return (*env)->setBoolField(env, obj, fid, b);
}

void setField(JNIEnv *env, jobject obj, jfieldID fid, jlong l){
  return (*env)->setLongField(env, obj, fid, l);
}

void setField(JNIEnv *env, jobject obj, jfieldID fid, jint i){
  return (*env)->setIntField(env, obj, fid, i);
}

void setField(JNIEnv *env, jobject obj, jfieldID fid, jobject obj2){
  return (*env)->setObjectField(env, obj, fid, obj2);
}


"
    for_each_struct do |struct|
      name = struct["str_name"]
      name_java = name.capitalize
      fields = struct["str_field"]
      class_str = "L#{package}/#{name_java};"

      #generation of lang2cxx
      @f.puts "template <>"
      @f.puts "#{name} lang2cxx<jobject, #{name}>(JNIEnv *env, jobject obj){"
      @f.puts "  jclass cls = (*env)->GetObjectClass(env, obj);" #todo static
      @f.puts "  #{name} out;"
      fields.each do |f|
        field = f[0]
        type = @types[f[1]]
        jtype = native2jtype(type)
        cxxtype = cxx_type(type)
        fid = "field_#{field}_id"
        @f.puts "  jfieldID #{fid} =\n    (*env)->GetFieldID(env, cls, \"#{field}\", \"#{class_str}\");" #todo static
        @f.puts "  assert(#{fid} != NULL);"
        @f.puts "  out.#{field} = lang2cxx<#{jtype}, #{cxxtype}>(env, getField<#{jtype}>(env, obj, #{fid}) );" #todo array
      end
      @f.puts "  return out;\n}\n"

      #generation of cxx2lang
      @f.puts "template <>"
      @f.puts "jobject cxx2lang<jobject, #{name}>(JNIEnv *env, #{name} in){"
      @f.puts "  jclass cls = (*env)->findClass(env, obj);" #todo static
      @f.puts "  assert(cls != NULL);"
      @f.puts "  jnethodID cid = (*env)->GetMethodID(env, cls, \"<init>\", \"()V\")"
      @f.puts "  jobject out = (*env)->NewObject(env, cls, cid, 0);"
      fields.each do |f|
        field = f[0]
        type = @types[f[1]]
        jtype = native2jtype(type)
        cxxtype = cxx_type(type)
        fid = "field_#{field}_id"
        @f.puts "  jfieldID #{fid} =\n    (*env)->GetFieldID(env, cls, \"#{field}\", \"#{class_str}\");" #todo static
        @f.puts "  assert(#{fid} != NULL);"
        @f.puts "  setField(env, out, #{fid}, cxx2lang<#{jtype}, #{cxxtype}>(env, in.#{field}) );" #todo array
      end
      @f.puts "  (*env)->DeleteLocalRef(jclass);"
      @f.puts "  return out;\n}"

    end

    build_enums_int

    # Implementing methods generated by Interface.java, in C++
    for_each_fun do |f|
      name = f.name
      type_ret = f.ret
      args = f.args
      @f.print cxx_proto(f, " Interface::")
      @f.puts "", "{"
      args.each do |arg|
        #todo types
        langtype = native2jtype(arg.type)
          cxxtype = cxx_type(arg.type)
        @f.puts "#{cxxtype} arg_#{arg.name} = lang2cxx<#{langtype}, #{cxxtype}>(#{arg.name});"  #todo array
      end
      @f.print "api_", name, "("
      if args != []
        args[0..-2].each do |y|
          @f.print "arg_",y.name, ", "
        end
        @f.print "arg_",args[-1].name
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
      "java.util.ArrayList< #{ conv_java_type_aux(t.type, true) } >"
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
    @f.puts "package prologin.java;"
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
    @f.puts "package prologin.java ;"
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
#{target}-dists = interface.hh
#{target}-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
    EOF
    @f.close
  end

end
