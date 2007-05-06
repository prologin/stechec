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
	blitz1_1.cmd				\
	blitz1_1.ref				\
	blitz1_2.cmd				\
	blitz1_2.ref				\
	block_1.cmd				\
	block_1.ref				\
	block_2.cmd				\
	block_2.ref				\
	block2_1.cmd				\
	block2_1.ref				\
	block2_2.cmd				\
	block2_2.ref				\
	blockfollow1_1.cmd			\
	blockfollow1_1.ref			\
	blockfollow1_2.cmd			\
	blockfollow1_2.ref			\
	blockfollow2_1.cmd			\
	blockfollow2_1.ref			\
	blockfollow2_2.cmd			\
	blockfollow2_2.ref			\
	crash_1.cmd				\
	crash_1.ref				\
	crash_2.cmd				\
	crash_2.ref				\
	crowd1_1.cmd				\
	crowd1_1.ref				\
	crowd1_2.cmd				\
	crowd1_2.ref				\
	crowd2_1.cmd				\
	crowd2_1.ref				\
	crowd2_2.cmd				\
	crowd2_2.ref				\
	formation.xml				\
	kickoff_basic_1.cmd			\
	kickoff_basic_1.ref			\
	kickoff_basic_2.cmd			\
	kickoff_basic_2.ref			\
	kickoff_scatter_scrimmage_1.cmd		\
	kickoff_scatter_scrimmage_1.ref		\
	kickoff_scatter_scrimmage_2.cmd		\
	kickoff_scatter_scrimmage_2.ref		\
	move_basic_1.cmd			\
	move_basic_1.ref			\
	move_basic_2.cmd			\
	move_basic_2.ref			\
	move_dodge_1.cmd			\
	move_dodge_1.ref			\
	move_dodge_2.cmd			\
	move_dodge_2.ref			\
	pass_1.cmd				\
	pass_1.ref				\
	pass_2.cmd				\
	pass_2.ref				\
	pass2_1.cmd				\
	pass2_1.ref				\
	pass2_2.cmd				\
	pass2_2.ref				\
	pickup_1.cmd				\
	pickup_1.ref				\
	pickup_2.cmd				\
	pickup_2.ref				\
	pickupf_1.cmd				\
	pickupf_1.ref				\
	pickupf_2.cmd				\
	pickupf_2.ref				\
	team.xml

# Tests expected to pass.
MY_TESTS =					\
	basic					\
	blitz1					\
	block					\
	block2					\
	blockfollow1				\
	blockfollow2				\
	crash					\
	kickoff_basic				\
	kickoff_scatter_scrimmage		\
	move_basic				\
	move_dodge				\
	pass					\
	pass2					\
	pickup					\
	pickup_fail

# Tests expected to fail (show bugs that should be corrected)
MY_XTESTS =					\
	crowd1					\
	crowd2

