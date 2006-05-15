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


class RubyMakefile
  def initialize
    @makefile = <<-EOF
#####################
# Macro Definitions #
#####################
CXX 	= g++
SRC     = interface.cc
OBJS 	= ${SRC:.cc=.o}
RM 	= /bin/rm -f

# FIXME: how to get the real good path ?
INCLUDES = -I/usr/lib/ruby/1.8/i486-linux/
CXXFLAGS = -fPIC -W -Wall ${INCLUDES}
LDFLAGS  = -lruby1.8

##############################
# Basic Compile Instructions #
##############################

all: ${NAME}

${NAME}: ${OBJS}
\t${CC} #{TARGET_GCC_LINK} ${OBJS} ${LDFLAGS} -o ${NAME} $(CHECK_CHEAT)
\t@echo Finished

clean:
\t@echo "Cleaning..."
\t${RM} ${OBJS} *~ \#*\#

distclean: clean
\t${RM} ${NAME} #{$conf['conf']['player_filename']}.tgz

tar:
\ttar cvzf #{$conf['conf']['player_filename']}.tgz $(SRC) *.rb
EOF
  end

  def build_client(path)
    f = File.new(path + "makeruby", 'w')
    f.puts "# -*- Makefile -*-"
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-ruby", 'w')
    f.print <<-EOF
# -*- Makefile -*-

NAME      = #{$conf['conf']['player_lib']}.so

CHECK_CHEAT = `while read i; do echo -Wl,"-wrap $$i"; done < forbidden_fun-c`

    EOF
    f.print @makefile
    f.close
  end

end
