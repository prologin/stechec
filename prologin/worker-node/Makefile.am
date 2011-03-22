workernodedir = $(pkglibdir)/workernode
workernodecfgdir = $(sysconfdir)/stechec

dist_workernode_DATA = \
	__main__.py \
	__init__.py \
	operations.py \
	paths.py

dist_bin_SCRIPTS = worker-node

dist_workernodecfg_DATA = worker-node.yml

EXTRA_DIST = worker-node.in worker-node.yml.in paths.py.in
