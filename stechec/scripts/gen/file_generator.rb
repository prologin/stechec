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

class FileGenerator

  def print_banner(script)
    print_multiline_comment \
"This file has been generated, if you wish to
modify it in a permanent way, please refer
to the script file : gen/" + script
    @f.puts
  end

  def conv_type(val, table = @types)
    if val == nil or val == ""
      return table['void']
    elsif table.has_key?(val)
      return table[val]
    else
      print "Warning: type '", val, "' isn't specified for language "
      print @lang, "\n"
    end
  end

  def build_constants
    $conf['constant'].delete_if {|x| x['doc_extra'] }
    $conf['constant'].each do |x|
      print_multiline_comment(x['cst_comment'])
      print_constant(x['cst_type'], x['cst_name'], x['cst_val'])
      @f.puts "\n"
    end
  end

  def for_each_fun(print_comment = true, &block)
    $conf['function'].delete_if {|x| x['doc_extra'] }
    $conf['function'].each do |x|
      print_multiline_comment(x['fct_summary']) if print_comment
      block.call(x['fct_name'], x['fct_ret_type'] , x['fct_arg'])
      @f.puts
    end
  end

  def for_each_user_fun(print_comment = true, &block)
    [ {"fct_summary" => "Fonction appele au debut de la partie", 
        "fct_name"=>"init_game"}, 
      {"fct_summary" => "Fonction appele a chaque tour", 
        "fct_name" => "play_turn"},
    ].each do |x|
      print_multiline_comment(x['fct_summary']) if print_comment
      block.call(x['fct_name'], x['fct_ret_type'] , x['fct_arg'])
      @f.puts
    end
  end

end


# A generic C file generator (proto, ...)
class CProto < FileGenerator

  def initialize
    @lang = "C"
    @types = {
      'void' => 'void',
      'int' => 'int',
      'bool' => 'int'
    }
  end

  def print_comment(str)
    @f.puts '/* ' + str + ' */' if str
  end

  def print_multiline_comment(str)
    return unless str
    @f.puts '/*!'
    str.each {|s| @f.print '** ', s }
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
    @f.print ext, conv_type(ret_type, types)
    @f.print " ", name, "("
    if args != nil and args != []
      print_args = args.collect { |arg| conv_type(arg[1], types) + " " + arg[0] }
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
    @f.print conv_type("void")
  end
end

# A generic Cxx file generator (proto, ...)
class CxxProto < CProto

  def initialize
    @lang = "C++"
    @types = {
      'void' => 'void',
      'int' => 'int',
      'bool' => 'bool'
    }
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

end
