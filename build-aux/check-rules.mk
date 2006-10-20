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
#  Copyright (C) 2005, 2006 Prologin
#

# do not try to erase this variable
EXTRA_DIST = test_pool.yml

clean-local::
	-rm -rf *-rules-test
	-rm -f *.log

installcheck-local::
	-@rm -rf log
	@to=0;						\
	for t in $(MY_TESTS); do			\
	  LD_LIBRARY_PATH="$(libdir)"			\
	  RUBYLIB="$(pkglibdir)/ruby"			\
	  BASHLIB="$(pkglibdir)/bash"			\
	  PATH="$(bindir):$$PATH"			\
	  srcdir="$(srcdir)/"				\
	  xml_parser_path="$(pkglibdir)/bash/"		\
	  $(RUBY) $(pkglibdir)/ruby/rules_test.rb	\
	    $(YAML_TEST_POOL) $(YAML_FILE) $$t		\
	     > $$t.log 2>&1; res=$$?; to=$$((to + res));	\
	  test $$res -eq 0 && echo "PASS: $$t" || echo "FAIL: $$t"; \
	done; 						\
	test $$to -eq 0
