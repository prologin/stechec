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


class JavaMakefile
  def initialize
    @makefile = <<-EOF
#####################
# Macro Definitions #
#####################
CNISRC   = interface.cc interface.hh
CNI      = interface.o
CLASS    = $(SRC:.java=.class)
LIMEOBJ  ?= ../includes/main.o
OBJS     = $(SRC:.java=.o) $(LIMEOBJ)
HEADER   = $(SRC:.java=.h)

CXXFLAGS = -W -Wall -I. -fPIC -O2
CXX      = g++
CJ       = gcj
CJH	 = gcjh
RM       = /bin/rm -f

##############################
# Basic Compile Instructions #
##############################

.SUFFIXES: .java .class

all: $(NAME)

$(NAME)	: $(OBJS) $(HEADER) $(CNI)
\t$(CXX) #{TARGET_GCC_LINK} -o $(NAME) $(OBJS) $(CNI) -lgcj $(CHECK_CHEAT)

%.class : %.java
\t$(CJ) -C $<

%.o 	: %.java
\t$(CJ) -c $< -o $@

%.h 	: %.class
\t$(CJH) -I. $(@:.h=)

tar:
\ttar cvzf #{$conf['conf']['player_filename']}.tgz $(SRC) $(CNISRC)

clean:
\t@echo 'Cleaning...'
\t$(RM) $(HEADER) *.o *.class *~ \\#*\\#

distclean: clean
\t$(RM) $(NAME) #{$conf['conf']['player_filename']}.tgz
  EOF
  end

  def build_client(path)
    f = File.new(path + "makejava", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-java", 'w')
    f.print <<-EOF
# -*- Makefile -*-

SRC       = $(wildcard *.java)
NAME      = #{$conf['conf']['player_lib']}.so

CHECK_CHEAT = `while read i; do echo -Wl,-wrap -Wl,$$i; done < forbidden_fun-java`
LIMEOBJ     = stechec_lime.o

    EOF
    f.print @makefile
    f.close
  end

end
