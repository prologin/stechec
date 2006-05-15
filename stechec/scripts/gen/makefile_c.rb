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

class CMakefile
  def initialize
    @makefile = <<-EOF
#####################
# Macro Definitions #
#####################
CC 	= gcc
MAKE 	= make
LIMEOBJ ?= ../includes/main.o
OBJS 	= ${SRC:.c=.o} $(LIMEOBJ)
RM 	= /bin/rm -f

CFLAGS  = -fPIC -W -Wall ${MY_CFLAGS}

##############################
# Basic Compile Instructions #
##############################

all: ${NAME}

${NAME}: $(INCL) ${OBJS}
\t${CC} #{TARGET_GCC_LINK} ${OBJS} -o ${NAME} $(CHECK_CHEAT)
\t@echo Finished

clean:
\t@echo "Cleaning..."
\t${RM} ${OBJS} *~ \#*\#

distclean: clean
\t${RM} ${NAME} #{$conf['conf']['player_filename']}.tgz

tar:
\ttar cvzf #{$conf['conf']['player_filename']}.tgz $(SRC) *.h
EOF
  end

  def build_client(path)
    f = File.new(path + "makec", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-c", 'w')
    f.print <<-EOF
# -*- Makefile -*-

INCL      = $(wildcard *.h)
SRC       = $(wildcard *.c)
NAME      = #{$conf['conf']['player_lib']}.so

MY_CFLAGS = -O2

CHECK_CHEAT = `while read i; do echo -Wl,"--wrap=$$i"; done < forbidden_fun-c`
LIMEOBJ   = 

    EOF
    f.print @makefile
    f.close
  end

end
