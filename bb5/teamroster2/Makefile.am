# -*- Makefile.am -*-

bin_PROGRAMS = teamroster

teamroster_SOURCES = 				\
	BBowlWidget.cc				\
	BBowlWidget.hh				\
	Helper-classes.cc			\
	Helper-classes.hh			\
	My_DropDown.cc				\
	My_DropDown.hh				\
	PlayerLineWidget.cc			\
	PlayerLineWidget.hh			\
	Position.cc				\
	Position.hh				\
	Race.cc					\
	Race.hh					\
	InvalidParameterException.hh					\
	Player.cc					\
	Player.hh					\
	Team.cc					\
	Team.hh					\
	RaceHandler.cc				\
	RaceHandler.hh				\
	RaceParser.cc				\
	RaceParser.hh				\
	TeamWriter.cc				\
	TeamWriter.hh				\
	Teamroster.cc				\
	TeamrosterApp.cc			\
	TeamrosterApp.hh

teamroster_CPPFLAGS = $(XERCES_CPPFLAGS)
teamroster_CXXFLAGS = $(PARAGUI_CFLAGS)
teamroster_LDFLAGS =  -lxerces-c $(PARAGUI_LIBS)


