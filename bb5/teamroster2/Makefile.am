# -*- Makefile.am -*-

bin_PROGRAMS = teamroster

teamroster_SOURCES = 						\
	gui/paragui/SkillsDialog.cc			\
	gui/paragui/SkillsDialog.hh			\
	gui/paragui/InputDialog.cc			\
	gui/paragui/InputDialog.hh			\
	gui/paragui/BBowlWidget.cc			\
	gui/paragui/BBowlWidget.hh			\
	gui/paragui/My_DropDown.cc			\
	gui/paragui/My_DropDown.hh			\
	gui/paragui/PlayerLineWidget.cc		\
	gui/paragui/PlayerLineWidget.hh		\
	gui/paragui/TeamrosterApp.cc			\
	gui/paragui/TeamrosterApp.hh			\
	common/InvalidParameterException.hh	\
	common/Position.cc					\
	common/Position.hh					\
	common/Race.cc						\
	common/Race.hh						\
	common/Player.cc						\
	common/Player.hh						\
	common/Team.cc						\
	common/Team.hh						\
	xml/Helper-classes.cc					\
	xml/Helper-classes.hh					\
	xml/TRParser.cc						\
	xml/TRParser.hh						\
	xml/ParametersHandler.hh				\
	xml/ParametersHandler.cc				\
	xml/RaceHandler.hh					\
	xml/RaceHandler.cc					\
	xml/RaceHandler.hh					\
	xml/TeamHandler.cc					\
	xml/TeamHandler.hh					\
	xml/TeamWriter.cc					\
	xml/TeamWriter.hh					\
	Teamroster.cc				

teamroster_CPPFLAGS = $(XERCES_CPPFLAGS)
teamroster_CXXFLAGS = $(PARAGUI_CFLAGS)
teamroster_LDFLAGS =  -lxerces-c $(PARAGUI_LIBS)


