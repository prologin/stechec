# -*- Makefile -*-

include $(top_srcdir)/build-aux/check-rules.mk

YAML_FILE = bb5
YAML_TEST_POOL = $(srcdir)/test_pool.yml

# File to include into the tarball
EXTRA_DIST +=					\
	basic_1.cmd				\
	basic_1.diff				\
	basic_2.cmd				\
	basic_2.diff				\
	block_1.cmd				\
	block_1.diff				\
	block_2.cmd				\
	block_2.diff				\
	formation.xml				\
	kickoffbug_1.cmd			\
	kickoffbug_1.diff			\
	kickoffbug_2.cmd			\
	kickoffbug_2.diff			\
	move_1.cmd				\
	move_1.diff				\
	move_2.cmd				\
	move_2.diff				\
	pass_1.cmd				\
	pass_1.diff				\
	pass_2.cmd				\
	pass_2.diff				\
	pickup_1.cmd				\
	pickup_1.diff				\
	pickup_2.cmd				\
	pickup_2.diff				\
	pickupf_1.cmd				\
	pickupf_1.diff				\
	pickupf_2.cmd				\
	pickupf_2.diff				\
	team.xml

# Tests expected to pass.
MY_TESTS =					\
	basic					\
	move					\
	pass					\
	pickup					\
	pickupf

# Tests expected to fail.
MY_XTESTS =					\
	kickoffbug				\
	block
