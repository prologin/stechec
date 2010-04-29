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

class PythonMakefile
  def build_metaserver(path)
    target = $conf['conf']['player_lib']
    f = File.open(path + "Makefile-python", 'w')
    f.print <<-EOF
# -*- Makefile -*-

lib_TARGETS = #{target}

# Tu peux rajouter des fichiers source ici
#{target}-dists = $(wildcard *.py)

# Evite de toucher a ce qui suit
#{target}-dists += api.py interface.hh
#{target}-srcs = interface.cc stechec_lime.cc

pc = $(shell [ which python-config >/dev/null 2>&1 && echo python-config || echo python2.5-config)
#{target}-cxxflags = -fPIC $(shell $(pc) --includes)
#{target}-ldflags = -s $(shell $(pc) --ldflags)

V=1
include $(MFPATH)/rules.mk
    EOF
    f.close
  end

end
