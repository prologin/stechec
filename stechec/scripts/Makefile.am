#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Stechec project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Stechec project; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  Copyright (C) 2005, 2006, 2007 Prologin
#

EXTRA_DIST =					\
	files					\
	conf.rb.in

dist-hook:
	rm -rf `find $(distdir) -name .svn`

rubydir = $(pkglibdir)/ruby
bashdir = $(pkglibdir)/bash

nobase_dist_ruby_DATA =				\
	generator.rb				\
	gen/file_generator.rb			\
	gen/generator_c.rb			\
	gen/generator_caml.rb			\
	gen/generator_cxx.rb			\
	gen/generator_java.rb			\
	gen/generator_pascal.rb			\
	gen/generator_haskell.rb		\
	gen/generator_ruby.rb			\
	gen/generator_lua.rb			\
	gen/generator_python.rb			\
	gen/generator_cs.rb			\
	gen/makefile_c.rb			\
	gen/makefile_caml.rb			\
	gen/makefile_cxx.rb			\
	gen/makefile_java.rb			\
	gen/makefile_pascal.rb			\
	gen/makefile_haskell.rb			\
	gen/makefile_ruby.rb			\
	gen/makefile_lua.rb			\
	gen/makefile_python.rb			\
	gen/makefile_cs.rb			\
	gen/make_rules.rb			\
	gen/make_tex.rtex			\
	rules_test.rb

nodist_ruby_DATA =				\
	conf.rb

dist_bash_DATA =				\
	run.sh					\
	xmlparser.awk

nobase_pkgdata_DATA =				\
	files/main.cc				\
	files/rules.mk				\
	files/Api.cc				\
	files/Api.hh				\
	files/Client.hh				\
	files/Client.cc				\
	files/ClientDiffer.hh				\
	files/ClientDiffer.cc				\
	files/ClientEntry.hh				\
	files/ClientEntry.cc				\
	files/Constant.hh			\
	files/GameData.hh				\
	files/GameData.cc				\
	files/Makefile.am				\
	files/Server.hh				\
	files/Server.cc				\
	files/ServerEntry.hh				\
	files/ServerEntry.cc				\
	files/ServerResolver.hh				\
	files/ServerResolver.cc				\
	files/interface.cc				\
	files/check.cc				\
	files/load_rules.cc				\
	files/bin2c.lua				\
	files/forbidden_fun-c			\
	files/forbidden_fun-java		\
	files/forbidden_fun-pascal		\
	files/forbidden_fun-caml

bin_SCRIPTS =			\
	run			\
	generator

CLEANFILES = $(bin_SCRIPTS)

run	: Makefile
	echo '#! /bin/sh' > $@
	echo "export xml_parser_path=$(bashdir)/" >> $@
	echo 'exec @BASH@ $(pkglibdir)/bash/run.sh $$@' >> $@

generator : Makefile
	echo '#! /bin/sh' > $@
	echo 'exec @RUBY@ -I$(RUBY_INC) $(RUBY_INC)/generator.rb $$@' >> $@

# manual pages
#dist_man_MANS        = run.1 gen/generator.1
MAINTAINERCLEANFILES = $(dist_man_MANS)

run.1: run.sh
	@echo "#! @BASH@" > $@.man
	@cat $< >> $@.man
	@chmod +x $@.man
	$(HELP2MAN) --source=Prologin --no-info ./$@.man -o $@
	@rm -f $@.man

gen/generator.1: gen/generator.rb
	@echo "#! @RUBY@" > $@.man
	@cat $< >> $@.man
	@chmod +x $@.man
	$(HELP2MAN) --source=Prologin --no-info ./$@.man -o $@
	@rm -f $@.man
