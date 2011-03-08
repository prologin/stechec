# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.c
champion-dists =
champion-cflags = -ggdb3 -Wall -Wextra

# Evite de toucher a ce qui suit
champion-dists += prologin.h
champion-srcs += interface.cc ../includes/main.cc
include ../includes/rules.mk
