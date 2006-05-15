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
%w[Api.cc Api.hh interface.cc Constant.hh].each do |x|
  if File.exist? 'files/' + x
    File.copy 'files/' + x, install_path.to_s
  else
    File.copy Pathname.new(PKGDATADIR) + 'files' + x, install_path.to_s
  end
end

# genere les entrees pour les autres fichiers

puts "Generating API files from YAML."
Dir.chdir(install_path)

class CxxFileGenerator < CxxProto

  # we do not need comment
  def print_comment(str)
  end
  def print_multiline_comment(str)
  end

  def fill_file_section(filename, &block)
    puts "Cook #{filename}..."
    File.mv(filename, filename + ".tmp")
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

  # Api.cc
  def print_cxx_api
    for_each_fun do |x, y, z| 
      print_proto("Api::" + x, y, z); 
      @f.puts "
{
  return 0;
}
"
    end
  end

  # interface.cc
  def print_interface
    for_each_fun do |x, y, z| 
      print_proto(x, y, z); 
      @f.print "
{
  return api->#{x}("
      args = z
      if args != nil and args != []
        args[0..-2].each do |arg|
          @f.print arg[0], ", "
        end
        @f.print args[-1][0]
      end
      @f.puts ");
}"
    end
  end

  # Api.hh
  def print_cxx_api_head
    for_each_fun do |x, y, z| 
      print_proto(x, y, z, "  "); 
      @f.print ";"
    end
  end

  # Constant.hh
  def print_cst
    build_constants
  end
end

gen = CxxFileGenerator.new
gen.fill_file_section("Api.cc") { gen.print_cxx_api }
gen.fill_file_section("Api.hh") { gen.print_cxx_api_head }
gen.fill_file_section("interface.cc") { gen.print_interface }
gen.fill_file_section("Constant.hh") { gen.print_cst }

puts "Done."
