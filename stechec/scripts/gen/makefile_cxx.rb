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

class CxxMakefile
  def initialize
    @makefile = <<EOF
#####################
# Macro Definitions #
#####################
CXX 	= g++
MAKE 	= make
LIMEOBJ ?= ../includes/main.o
OBJS 	= ${SRC:.cc=.o} $(LIMEOBJ)
RM 	= /bin/rm -f

CFLAGS = -fPIC
CXXFLAGS  = -fPIC -Wall ${MY_CXXFLAGS}

##############################
# Basic Compile Instructions #
##############################

all: ${NAME}

${NAME}: ${OBJS} ${MY_LIBS}
\t$(CXX) #{TARGET_GCC_LINK} ${OBJS} ${MY_LIBS} -o ${NAME} $(CHECK_CHEAT)
\t@echo Finished

clean:
\t@echo "Cleaning..."
\t${RM} ${OBJS} *~ \#*\#

distclean: clean
\t${RM} ${NAME} #{$conf['conf']['player_filename']}.tgz

tar:
\ttar cvzf #{$conf['conf']['player_filename']}.tgz $(SRC) *.hh
EOF
  end

  def build_client(path)
    f = File.new(path + "makecxx", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-cxx", 'w')
    f.print <<-EOF
# -*- Makefile -*-

INCL      = $(wildcard *.hh)
SRC       = $(wildcard *.cc *.cpp)
NAME      = #{$conf['conf']['player_lib']}.so

MY_CXXFLAGS = -O2

CHECK_CHEAT = `while read i; do echo -Wl,--wrap=$$i; done < forbidden_fun-c`
LIMEOBJ   = stechec_lime.o

    EOF
    f.print @makefile
    f.close
  end

end
