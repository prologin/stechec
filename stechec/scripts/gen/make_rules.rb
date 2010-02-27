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

#
# Cree un template pour les regles d'un nouveau concours, decrit par
# un fichier YAML
# Appelé depuis generator.rb
#

require "gen/generator_c.rb"

# cree le repertoire et detare la tarball dedans
package_name = $conf['conf']['package_name']
librule_name = $conf['conf']['rule_lib']
install_path = Pathname.new($install_path) + package_name + "rules"
install_path.mkpath

# Copie les quelques squelettes.
files = %w{
  Api.cc
  Api.hh
  Client.cc
  Client.hh
  ClientDiffer.cc
  ClientDiffer.hh
  ClientEntry.cc
  ClientEntry.hh
  Constant.hh
  GameData.cc
  GameData.hh
  Makefile.am
  Server.cc
  Server.hh
  ServerEntry.cc
  ServerEntry.hh
  ServerResolver.cc
  ServerResolver.hh
  check.cc
  interface.cc
  load_rules.cc
}
files.each do |x|
  if File.exist? 'files/' + x
    FileUtils.cp 'files/' + x, install_path.to_s
  else
    FileUtils.cp Pathname.new(PKGDATADIR) + 'files' + x, install_path.to_s
  end
end

# genere les entrees pour les autres fichiers

puts "Generating API files from YAML."
Dir.chdir(install_path)

class CxxFileGenerator < CxxProto
  def replace(line)
    replaces = {}
    replaces['!!year!!'] = Date.today.year.to_s
    replaces['!!package_name!!'] = $conf['conf']['package_name']
    replaces['!!rule_lib!!'] = $conf['conf']['server_rule_lib']
    replaces.each do |key, value|
      line = line.sub(key, value)
    end
    line
  end

  def fill_file_section(filename, &block)
    puts "Cook #{filename}..."
    FileUtils.mv(filename, filename + ".tmp")
    File.open(filename + ".tmp" , 'r') do |fr|
      @f = File.new( filename, 'w')
      while s = fr.gets do
        if s =~ /@@GEN_HERE@@/ then
          block.call
        else
          @f.puts s
        end
      end
      @f.close
    end
    File.unlink(filename + ".tmp")
  end

  def expand_variables(filename)
    puts "Expanding #{filename}..."
    FileUtils.mv(filename, filename + ".tmp")
    File.open(filename + ".tmp" , 'r') do |fr|
      @f = File.new( filename, 'w')
      while s = fr.gets do
        if s =~ /!!.*!!/ then
          @f.puts(replace s)
        else
          @f.puts s
        end
      end
      @f.close
    end
    File.unlink(filename + ".tmp")
  end

  # Api.cc
  def print_cxx_api
    for_each_fun do |fn|
      @f.print cxx_proto(fn, "Api::")
      @f.puts "
{
  // TODO
  abort();
}
"
    end
  end

  # interface.cc
  def print_interface
    for_each_fun do |fn| 
      if fn.dumps then
        t = fn.dumps
        @f.puts "std::ostream& operator<<(std::ostream& os, #{t.name} v)"
        @f.puts "{"
        if t.is_enum?
          @f.puts "  switch (v) {"
          t.conf['enum_field'].each do |f|
            f = f[0].upcase
            @f.puts "  case #{f}: os << \"#{f}\"; break;"
          end
          @f.puts "  }"
        else
          @f.puts "  os << \"{ \";"
          t.conf['str_field'].each do |f|
            if f != t.conf['str_field'][0]
              @f.puts "  os << \", \";"
            end
            ty = @types[f[1]]
            na = f[0]
            @f.puts "  os << \"#{na}\" << \"=\" << v.#{na};"
          end
          @f.puts "  os << \" }\";"
        end
        @f.puts "  return os;"
        @f.puts "}"

        @f.print cxx_proto(fn, "api_", 'extern "C"')
        @f.puts "", "{"
        @f.puts "  std::cerr << v << std::endl;"
        @f.puts "}"
      else
        @f.print cxx_proto(fn, "api_", 'extern "C"')
        @f.puts "", "{"
        @f.print "  "
        unless fn.ret.is_nil? then
          @f.print "return "
        end
        @f.print "api->#{fn.name}("
        args = fn.args
        if args != []
          args[0..-2].each do |arg|
            @f.print arg.name, ", "
          end
          @f.print args[-1].name
        end
        @f.puts ");"
        @f.puts "}"
      end
    end
  end

  # Api.hh
  def print_cxx_api_head
    for_each_fun do |fn|
      @f.print cxx_proto(fn, "", "  ");
      @f.print ";"
    end
  end

  # Constant.hh
  def print_cst
    build_constants
    build_enums
    build_structs
  end
end

def do_nothing
end

gen = CxxFileGenerator.new
files.each do |fn|
  gen.expand_variables(fn)
end
gen.fill_file_section("Api.cc") { gen.print_cxx_api }
gen.fill_file_section("Api.hh") { gen.print_cxx_api_head }
gen.fill_file_section("interface.cc") { gen.print_interface }
gen.fill_file_section("Constant.hh") { gen.print_cst }

puts "Done."
