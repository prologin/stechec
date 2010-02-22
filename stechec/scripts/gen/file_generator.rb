# -*- ruby -*-
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

class Type
  attr_reader :name

  def is_simple?
    false
  end

  def is_struct?
    false
  end

  def is_enum?
    false
  end

  def is_array?
    false
  end

  def is_nil?
    false
  end
end

class SimpleType < Type
  def initialize(name)
    @name = name
  end

  def is_simple?
    true
  end

  def is_nil?
    @name == "void"
  end
end

class StructType < Type
  attr_reader :conf

  def initialize(conf)
    @conf = conf
    @name = conf['str_name']
  end

  def is_struct?
    true
  end
end

class EnumType < Type
  attr_reader :conf

  def initialize(conf)
    @conf = conf
    @name = conf['enum_name']
  end

  def is_enum?
    true
  end
end

class ArrayType < Type
  attr_reader :type

  def initialize(type)
    @type = type
  end

  def is_array?
    true
  end
end

class FunctionArg
  attr_reader :type
  attr_accessor :name
  attr_reader :conf

  def initialize(types, conf)
    @conf = conf
    @name = conf[0]
    @type = types[conf[1]]
  end
end

class Function
  attr_reader :name
  attr_reader :ret
  attr_reader :args
  attr_reader :conf

  def initialize(types, conf)
    @conf = conf
    @name = conf['fct_name']
    @ret = types[conf['fct_ret_type']]
    if conf['fct_arg']
      @args = conf['fct_arg'].map do |arg|
        FunctionArg.new(types, arg)
      end
    else
      @args = []
    end
  end
end

class TypesHash < Hash
  def [](x) # returns Type
    m = x.match(/^([^ ]*) array$/) # ml-like declaration to array
    if m
      ArrayType.new super(m[1])
    else
      super(x)
    end
  end
end

class FileGenerator

  def initialize
    # Add required builtin types
    @types = TypesHash.new
    ['void', 'int', 'bool'].each do |x|
      @types[x] = SimpleType.new x
    end

    $conf['struct'].each do |x|
      @types[x['str_name']] = StructType.new x
    end

    $conf['enum'].each do |x|
      @types[x['enum_name']] = EnumType.new x
    end
  end

  def print_banner(script)
    print_multiline_comment \
"This file has been generated, if you wish to
modify it in a permanent way, please refer
to the script file : gen/" + script
    @f.puts
  end

  def build_constants
    $conf['constant'].delete_if {|x| x['doc_extra'] }
    $conf['constant'].each do |x|
      print_multiline_comment(x['cst_comment'])
      print_constant(x['cst_type'], x['cst_name'], x['cst_val'])
      @f.puts "\n"
    end
  end

  def for_each_enum(print_comment = true, &block)
    $conf['enum'].delete_if {|x| x['doc_extra'] }
    $conf['enum'].each do |x|
      print_multiline_comment(x['enum_summary']) if print_comment
      block.call(x)
      @f.puts
    end
  end

  def for_each_struct(print_comment = true, &block)
    $conf['struct'].delete_if {|x| x['doc_extra'] }
    $conf['struct'].each do |x|
      print_multiline_comment(x['str_summary']) if print_comment
      block.call(x)
      @f.puts
    end
  end

  def for_each_fun(print_comment = true, arr = 'function', &block)
    $conf[arr].delete_if {|x| x['doc_extra'] }
    $conf[arr].each do |x|
      fn = Function.new(@types, x)
      print_multiline_comment(x['fct_summary']) if print_comment
      block.call(fn)
      @f.puts
    end
  end

  def for_each_user_fun(print_comment = true, &block)
    for_each_fun(print_comment, 'user_function') { |fn| block.call(fn) }
  end

end


# A generic C file generator (proto, ...)
class CProto < FileGenerator

  def initialize
    super
    @lang = "C"
  end

  def print_comment(str)
    @f.puts '/* ' + str + ' */' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '/*!'
    str.each_line {|s| @f.print '** ', s }
    @f.puts "", "*/"
  end

  def print_include(file, std_path = false)
    if std_path
      @f.puts '#include <' + file + '>'
    else
      @f.puts '#include "' + file + '"'
    end
  end

  def print_constant(type, name, val)
      @f.print '# define ', name
      (25 - name.to_s.length).times { @f.print " " }
      @f.print " ", val, "\n"
  end

  def print_proto(name, ret_type, args, ext = "", types = @types)
    ext = ext + " " if ext != ""
    @f.print ext, ret_type
    @f.print " ", name, "("
    if args != nil and args != []
      print_args = args.collect { |arg| 
        arg[1] + " " + arg[0]
      }
      @f.print print_args.join(", ")
    else
      print_empty_arg
    end
    @f.print ")"
  end

  # must be called right after print_proto
  def print_body(content)
    @f.puts "", "{"
    @f.puts content
    @f.puts "}"
  end

  def print_empty_arg
    @f.print "void"
  end

  def build_enums
    for_each_enum do |x|
      @f.puts "typedef enum #{x['enum_name']} {"
      x['enum_field'].each do |f|
        name = f[0].upcase
        @f.print "  ", name, ", "
        @f.print "/* <- ", f[1], " */\n"
      end
      @f.print "} ", x['enum_name'], ";\n\n"
    end
  end

  def conv_type(type)
    if type.is_array?
      "std::vector<#{type.type.name}>"
    else
      type.name
    end
  end

  def build_structs
    build_structs_generic do |field, type| "  #{conv_type(@types[type])} #{field};" end
  end

  def build_structs_generic(&show_field)
    for_each_struct do |x|
      @f.puts "typedef struct #{x['str_name']} {"
      x['str_field'].each do |f|
        @f.print "#{show_field.call(f[0], f[1])}  /* <- ", f[2], " */\n"
      end
      @f.print "} ", x['str_name'], ";\n\n"
    end
  end
end

# A generic Cxx file generator (proto, ...)
class CxxProto < CProto

  def initialize
    super
    @lang = "C++"
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '///'
    str.each_line {|s| @f.print '// ', s }
    # @f.puts
    @f.puts "", "//"
  end

  def print_empty_arg
  end

  def cxx_proto(fn, name_prefix = "", modifiers = "")
    buf = ""
    modifiers = modifiers + " " if modifiers != ""
    buf += modifiers + cxx_type(fn.ret) + " " + name_prefix + fn.name + "("
    args = fn.args.map { |arg| "#{cxx_type(arg.type)} #{arg.name}" }
    buf += args.join(", ") + ")"
    buf
  end

  def cxx_type(type)
    # Returns the C++ type for the provided type.
    # Only really useful for arrays.
    if type.is_array?
      "std::vector<#{type.type.name}>"
    else
      type.name
    end
  end

end

# A generic CSharp file generator (proto, ...)
# We can inherit from CProto, as C# ressembles C, it works for now.
# And I'm a bit lazy. :-)
class CSharpProto < CProto

  def initialize
    super
    @lang = "CSharp"
  end

  def print_comment(str)
    @f.puts '// ' + str if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '///'
    str.each {|s| @f.print '// ', s }
    # @f.puts
    @f.puts "", "//"
  end

  def print_empty_arg
  end

  def print_proto(name, ret_type, args, ext = "", types = @types)
    ext = ext + " " if ext != ""
    @f.print ext, @typehandler.ret(ret_type)
    @f.print " ", name, "("
    if args != nil and args != []
      print_args = args.collect {
        |arg| @typehandler.arg(arg[1], arg[0])
      }
      @f.print print_args.join(", ")
    else
      print_empty_arg
    end
    @f.print ")"
  end
end
