# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers source ici
champion-srcs = prologin.cs

# Evite de toucher a ce qui suit
champion-dists = interface.hh
champion-srcs += api.cs interface.cc ../includes/main.cc
champion-cxxflags = -fPIC     $(shell pkg-config --cflags glib-2.0 mono)
champion-ldflags = -lm     $(shell pkg-config --libs glib-2.0 mono)
champion-csflags = -target:library -nowarn:0169,0649

include ../includes/rules.mk
