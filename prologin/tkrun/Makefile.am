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

rubydir = ${pkglibdir}/ruby

dist_ruby_DATA =	\
	champion.rb	\
	map.rb		\
	server.rb	\
	tkrun.rb	\
	interface.rb	\
	xmlconf.rb

bin_SCRIPTS =		\
	tkrun

CLEANFILES = $(bin_SCRIPTS)

tkrun : Makefile
	echo '#! /bin/sh' > $@
	echo 'exec @RUBY@ -I@RUBY_INC@ ${pkglibdir}/ruby/tkrun.rb $$@' >> $@

# manual page, mostly hacky...
dist_man_MANS        = tkrun.1
MAINTAINERCLEANFILES = $(dist_man_MANS)

tkrun.1: tkrun.rb
	@echo "#! @RUBY@" > $@.man
	@cat $< >> $@.man
	@chmod +x $@.man
	$(HELP2MAN) --source=Prologin --no-info ./$@.man -o $@
	@rm -f $@.man
