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
#  Copyright (C) 2005 Prologin
#


bin_PROGRAMS = main_server

main_server_SOURCES = 		\
	argument.cc 		\
	argument.hh		\
	Computer.cc		\
	Computer.hh		\
	init_close_sql.cc 	\
	init_close_sql.hh 	\
	make_computer.cc 	\
	make_computer.hh 	\
	make_int_tab.cc 	\
	make_int_tab.hh 	\
	Match.cc		\
	Match.hh		\
	connection_meta.cc 	\
	connection_meta.hh 	\
	gest_match.cc 		\
	simple_func.cc 		\
	simple_func.hh 		\
	gest_match.hh 		\
	conf.cc 		\
	conf.hh 		\
	main.cc			\
	end_delete_free.hh	\
	verboseur.cc		\
	verboseur.hh


main_server_CXXFLAGS = @MYSQL_CFLAGS@

main_server_LDADD = @MYSQL_LIBS@ 
