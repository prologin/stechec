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

class CamlMakefile
  def initialize
    @makefile = <<EOF
###########################
# C Interface Definitions #
###########################
CAMLOBJS = api.cmo $(SRC:.ml=.cmo)
LIMEOBJ  ?= ../includes/main.o
CINTOBJ  = interface.o $(LIMEOBJ)

#####################
# Macro Definitions #
#####################
# sorry, can't use ocamlopt, it doesn't work
OCAMLC 	= ocamlc
LIBS    = -L`ocamlc -where` -lcamlrun -lcurses -lm
GCC 	= gcc
RM 	= /bin/rm -f

CFLAGS  = -fPIC -O2 -I`ocamlc -where`

##############################
# Basic Compile Instructions #
##############################
.SUFFIXES: .ml

all: $(NAME)

$(NAME): prolocaml.o $(CINTOBJ)
\t$(GCC) #{TARGET_GCC_LINK} prolocaml.o $(CINTOBJ) $(LIBS) -o $(NAME) $(CHECK_CHEAT)
\t@echo Finished

clean:
\t@echo "Cleaning..."
\t$(RM) *.o *.cm* *~ \#*\#

distclean: clean
\t$(RM) $(NAME) #{$conf['conf']['player_filename']}.tgz

tar:
\t@tar cvzf #{$conf['conf']['player_filename']}.tgz api.ml $(SRC) interface.{c,h}

prolocaml.o : $(CAMLOBJS)
\t$(OCAMLC) $(CAMLFLAGS) -output-obj -o $@ $(CAMLOBJS)

%.cmo   : %.ml
\t$(OCAMLC) $(CAMLFLAGS) -c -o $@ $<
EOF
  end

  def build_client(path)
    f = File.new(path + "makecaml", 'w')
    f.puts "# -*- Makefile -*-"  
    f.print @makefile
    f.close
  end

  def build_metaserver(path)
    f = File.new(path + "Makefile-caml", 'w')
    f.print <<-EOF
# -*- Makefile -*-

SRC       = $(wildcard *.ml)
NAME      = #{$conf['conf']['player_lib']}.so

CHECK_CHEAT = `while read i; do echo -Wl,-wrap -Wl,$$i; done < forbidden_fun-caml`
LIMEOBJ   = stechec_lime.o

    EOF
    f.print @makefile
    f.close
  end

end
