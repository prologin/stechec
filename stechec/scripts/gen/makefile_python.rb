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
OBJS 	= interface.o ../includes/main.o
RM 	= /bin/rm -f

CXXFLAGS = -fPIC -W -Wall \\
   `python2.5-config --cflags` \\
   $(MY_CXXFLAGS)

LIBS = `python2.5-config --ldflags`

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
\ttar cvzf prologin.tgz constants.py interface.cc interface.hh $(SRC)
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

NAME      = #{$conf['conf']['player_lib']}.so

MY_CXXFLAGS = -O2

CHECK_CHEAT = `while read i; do echo -Wl,-wrap $$i; done < forbidden_fun-c`

    EOF
    f.print @makefile
    f.close
  end

end
