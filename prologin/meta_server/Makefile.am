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

bin_PROGRAMS = meta_server

meta_server_SOURCES = 		\
	argument.cc		\
	argument.hh		\
	conf.cc 		\
	conf.hh 		\
	connection.cc 		\
	connection.hh 		\
	db.cc 			\
	db.hh 			\
	explode.cc 		\
	explode.hh 		\
	command.hh		\
	command.cc		\
	command_client.cc 	\
	command_server.cc 	\
	command_compil.cc 	\
	command_triche.cc	\
	fork.cc 		\
	fork.hh 		\
	main_meta.cc 		\
	simple_func.cc		\
	simple_func.hh		\
	verboseur.cc		\
	verboseur.hh

meta_server_CXXFLAGS = @MYSQL_CFLAGS@

meta_server_LDADD = @MYSQL_LIBS@


