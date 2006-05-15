#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005 Prologin
#

require 'tk'
require "pathname"

require 'map'
require 'opt'
require 'menu'
require 'new'


if (ARGV.length == 1 and (ARGV[0] == '-h' or ARGV[0] == '--help'))
  print <<-EOF
Usage: map_editor [file]

map_editor is a useful map editor, for prolo2005 contest.

If [file] is not provided, start editing a new map file
If [file] is provided:
 - start map edition, if you are the owner of [file]
 - open map read-only, if you are not the owner

[file] is first searched on the current directory, then in
in directory given by the following environment variable: 
PROLO2005_SHARED_MAP
  EOF
  exit 0
end

#
# Search map file, if provided
#
$map_file = "<default>"
if ENV['PROLO2006_SHARED_MAP']
  $map_shared_dir = Pathname.new(ENV['PROLO2006_SHARED_MAP'])
end

$restricted = false
if ARGV.length >= 1
  if File.exist?(ARGV[0])
    $map_file = File.expand_path(ARGV[0])
  elsif $map_shared_dir and File.exist?($map_shared_dir + $map_file)
    $map_file = ($map_shared_dir + $map_file).to_s
  else
    puts "file " + $map_file + " not found, exiting"
    exit 1
  end
  $restricted = !File.stat($map_file).owned?
end

$root = TkRoot.new(:title => 'Editeur de map Prologin 2006')

opt = Option.new
map = Map.new(opt)
opt.map = map
menu = Menu.new(map, opt)

if $map_file != "<default>"
  # load configuration
  map.loadMap
  map.loadConfig
  opt.loadConfig
end

#
# Wait and see...
#
Tk.mainloop
