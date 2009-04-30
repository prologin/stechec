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

class CSharpMakefile
  def initialize
    @makefile = <<EOF
#####################
# Macro Definitions #
#####################
CXX 	= g++
MCS	= gmcs
LIMEOBJ ?= ../includes/main.o
OBJS 	= interface.o $(LIMEOBJ)
CS	= $(SRC) api.cs
CHAMPION= prologin.dll
RM 	= /bin/rm -f

CXXFLAGS = -fPIC -W -Wall \\
   `pkg-config --cflags glib-2.0` \\
   `pkg-config --cflags mono` \\
   $(MY_CXXFLAGS)

MCSFLAGS = -target:library -nowarn:0169

LIBS = `pkg-config --libs glib-2.0` `pkg-config --libs mono` -lm

##############################
# Basic Compile Instructions #
##############################

all: $(NAME) $(CHAMPION)

interface.o : interface.cc interface.hh

$(NAME): $(OBJS)
\t$(CXX) #{TARGET_GCC_LINK} $(OBJS) $(LIBS) -o $(NAME) $(CHECK_CHEAT)
\t@echo Finished

$(CHAMPION): $(CS)
\t$(MCS) -out:$(CHAMPION) $(MCSFLAGS) $(CS)

clean:
\t@echo "Cleaning..."
\t$(RM) $(OBJS) *~ #*#

distclean: clean
\t$(RM) $(CHAMPION) $(NAME) prologin.tgz

tar:
\ttar cvzf prologin.tgz interface.cc interface.hh $(CS)
EOF
  end

  def build_client(path)
    f = File.new(path + "makecs", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-cs", 'w')
    f.print <<-EOF
# -*- Makefile -*-

SRC	  = $(wildcard *.cs)
NAME      = #{$conf['conf']['player_lib']}.so

MY_CXXFLAGS = -O2

CHECK_CHEAT = `while read i; do echo -Wl,-wrap $$i; done < forbidden_fun-c`
LIMEOBJ     = stechec-lime.o

    EOF
    f.print @makefile
    f.close
  end

end
