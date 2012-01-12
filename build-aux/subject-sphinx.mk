# -*- Makefile -*-
#
#  Stechec project is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Copyright (C) 2012 Prologin
#

$(API): $(YAML)
	$(RUBY) \
	  -I$(top_srcdir)/stechec/scripts \
	  -I$(top_builddir)/stechec/scripts \
	  $(top_srcdir)/stechec/scripts/generator.rb \
	  sphinxdoc $(YAML) tmp
	mv tmp/file.rst $(API)
	rm -f $(srcdir)tmp

sphinx-html:
	$(MAKE) -f $(SPHINXMF) html

sphinx-pdf:
	$(MAKE) -f $(SPHINXMF) latexpdf

subject: $(YAML) $(API) sphinx-html sphinx-pdf

clean-local:
	$(MAKE) -f $(SPHINXMF) clean
	rm -f source/$(API)

# EOF

