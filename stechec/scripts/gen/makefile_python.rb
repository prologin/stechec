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
  def initialize
    @makefile = <<EOF
#####################
# Macro Definitions #
#####################
CXX 	= g++
LIMEOBJ ?= ../includes/main.o
OBJS 	= interface.o $(LIMEOBJ)
RM 	= /bin/rm -f
AUXFILES= api.py

CFLAGS = -fPIC
CXXFLAGS = -fPIC -W -Wall \\
   `python-config --cflags` \\
   $(MY_CXXFLAGS)

LIBS = `python-config --ldflags`

##############################
# Basic Compile Instructions #
##############################

all: $(NAME)

interface.o : interface.cc interface.hh

$(NAME): $(OBJS)
\t$(CXX) #{TARGET_GCC_LINK} $(OBJS) $(LIBS) -o $(NAME) $(CHECK_CHEAT)
\t@echo Finished

clean:
\t@echo "Cleaning..."
\t$(RM) $(OBJS) *~ *.pyc #*#

distclean: clean
\t$(RM) $(NAME) prologin.tgz

tar:
\ttar cvzf prologin.tgz interface.cc interface.hh $(SRC) $(AUXFILES)
EOF
  end

  def build_client(path)
    f = File.new(path + "makepython", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-python", 'w')
    f.print <<-EOF
# -*- Makefile -*-

SRC	  = $(wildcard *.py)
NAME      = #{$conf['conf']['player_lib']}.so

LIMEOBJ	  = stechec_lime.o

    EOF
    f.print @makefile
    f.close
  end

end
