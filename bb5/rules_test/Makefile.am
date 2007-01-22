# -*- Makefile -*-

include $(top_srcdir)/build-aux/check-rules.mk

YAML_FILE = bb5
YAML_TEST_POOL = $(srcdir)/test_pool.yml

# File to include into the tarball
EXTRA_DIST +=					\
	basic_1.cmd				\
	basic_1.ref				\
	basic_2.cmd				\
	basic_2.ref				\
	block_1.cmd				\
	block_1.ref				\
	block_2.cmd				\
	block_2.ref				\
	crash_1.cmd				\
	crash_1.ref				\
	crash_2.cmd				\
	crash_2.ref				\
	formation.xml				\
	kickoffbug_1.cmd			\
	kickoffbug_1.ref			\
	kickoffbug_2.cmd			\
	kickoffbug_2.ref			\
	move_1.cmd				\
	move_1.ref				\
	move_2.cmd				\
	move_2.ref				\
	pass_1.cmd				\
	pass_1.ref				\
	pass_2.cmd				\
	pass_2.ref				\
	pickup_1.cmd				\
	pickup_1.ref				\
	pickup_2.cmd				\
	pickup_2.ref				\
	pickupf_1.cmd				\
	pickupf_1.ref				\
	pickupf_2.cmd				\
	pickupf_2.ref
	team.xml

# Tests expected to pass.
MY_TESTS =					\
	basic					\
	pickup					\
	pickup_fail				\
	move					\
	pass					\
	block

# Tests expected to fail (show bugs that should be corrected)
MY_XTESTS =					\
	kickoffbug				\
	block2					\
	crash
