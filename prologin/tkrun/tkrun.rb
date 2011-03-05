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

if ARGV[0] == "--version"
  print <<EOF
tkrun 3.0

Copyright (C) 2005, 2006 Prologin.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Written by Olivier Gournet.
EOF
  exit 0

elsif (ARGV[0] == '-h') or (ARGV[0] == '--help')
  print <<-EOF
\`tkrun\' is part of \`stechec\'. It is a top-level gui application for
\`run\', to please people frozen by command-line interface.

Usage: tkrun [config-file]

It searchs and can upload maps in a directory given by the 
following environment variable: <contest-name>_SHARED_MAP

Ensure this directory has sticky-bit set, so everyone can upload.

It loads config-file in this order:
 - from the command line, if supplied
 - from last opened configuration file
 - from a default configuration file, given by run.sh

Report bugs to <info@prologin.org>
  EOF
  exit 0
end

require 'yaml'
require 'xmlconf'

# Common procedure to add a label + text box on a single line
def createOption(parent, label, input_tkvar)
  l = TkLabel.new(parent, :text => label)
  txtbox = TkEntry.new(parent) do
    textvariable input_tkvar
    width 7
    justify 'right'
  end
  TkGrid(l, txtbox, :sticky => 'w')
  return txtbox
end


#
# Check if 'run' is here, as we heavily depend on it.
#
if not system("which run > /dev/null 2>&1")
  puts "'run' not found. check your PATH."
  exit 1
end

#
# Load configuration file. Try hard to find one.
#
$conf = XMLConf.new
if ARGV.length == 1
  # load given file
  $conf.open ARGV[0]

elsif (File.exist?(File.expand_path("~/.tkrun-rc")))
  # open last opened configuration file
  File.open(File.expand_path("~/.tkrun-rc")) do |f|
    if (fn = f.gets) != nil
      fn = File.expand_path(fn.strip)
      $conf.open fn
    end
  end

else
  # open default config file
  file = Pathname.new(PKGDATADIR) + "default_conf.xml"
  File.open(file.to_s, "r") do |f|
    $conf.load f
  end
end

#
# Launch interface
#

require 'tk'
require 'map'
require 'server'
require 'champion'
require 'interface'

Interface.new
trap "SIGINT", "SIG_IGN"
Tk.mainloop

`echo "#{$conf.filename}" > ~/.tkrun-rc`if $conf.filename
