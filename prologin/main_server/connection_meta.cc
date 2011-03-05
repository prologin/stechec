/*
  Stechec project is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Stechec project is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the Stechec project; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Copyright (C) 2005 Prologin
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>
#include <errno.h>
#include <assert.h>

#include "verboseur.hh"
#include "simple_func.hh"
#include "Match.hh"
#include "Computer.hh"
#include "conf.hh"

/*!
** Make the for the connection to a meta server
** 
** @param ip	the string ip of the meta server
** @param port	the port of the meta
** 
** @return	the fd or -1 if an error occured
*/
int	             init_connection(char *ip, int port)
{
  int                   sockfd;
  struct sockaddr_in    sockad;
  struct hostent        *hote;

  print_verbose(1, 0, "[Main] Connection to meta server initializing: %s.\n", ip);
  if ((hote = gethostbyname(ip)) == NULL)
    return print_verbose(1, -1, 
			 "[Main] Meta Server error (gethostbyname): %s.\n", strerror(errno));
  if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    return print_verbose(1, -1, 
			 "[Main] Meta Server error (socket): %s.\n", strerror(errno));
  sockad.sin_family = AF_INET;
  sockad.sin_port = htons(port);
  memcpy(&sockad.sin_addr, hote->h_addr, hote->h_length);
  if (connect(sockfd, (struct sockaddr *) &sockad, sizeof (struct sockaddr)) == -1)
    return print_verbose(1, -1, 
			 "[Main] Meta Server error (connect): %s.\n", strerror(errno));
  print_verbose(2, 0, "[Main] Connection to meta server is ready.\n");
  return sockfd;
}


/*!
** The server call function
** it creates the good call for the meta.
** It is very important to respect the order of the argument
** because it will be easiest to parse for the meta server
** It calls create_player_str to make the player id argument for the server
** 
** @param match	the match structure
** 
** @return		1 if error, 0 otherwise
*/
static int	server_call(Computer *server, int id_match, int port_server)
{
  char		send[200];

  assert (server);
  print_verbose(2, 0, "[Main] Sending info to server for match.\n");
  sprintf(send, "server %d %d", id_match, port_server);
  server->_fd = init_connection(server->_ip, server->_port);
  write(server->_fd, send, strlen(send));
  close(server->_fd);
  print_verbose(3, 0, "[Main] Command send: \"%s\".\n", send);
  return 0;
}

/*!
** The client call function
** it creates the good call for the meta.
** It is very important to respect the order of the argument
** because it will be easiest to parse for the meta server
** 
** @param match		the match structure
** @param id_cleint	for the client tab
** 
** @return		1 if error, 0 otherwise
*/
static int	clients_call(Match *match)
{
  char		send[1024];

  assert (match);
  if (!match->_computer)
    return 0;
  for (int i = 0; i < match->_nb_players; i++)
  {
    print_verbose(2, 0, "[Main] Sending info to client %d for match.\n",
		  match->_computer[i]._id);
    sprintf(send, "client %d %d %d", 
	    match->_players[i], 
	    match->_computer_server._id,
	    match->_port_server);
    match->_computer[i]._fd = init_connection(match->_computer[i]._ip,
					      match->_computer[i]._port);
    print_verbose(3, 0, "[Main] Info sent: %s\n", send);
    write(match->_computer[i]._fd, send, strlen(send));
    close(match->_computer[i]._fd);
  }
  return 0;
}


/*!
** The main meta connection function
** 
** @param match	the match structure fully filled
** 
** @return		1 if error, 0 otherwise
*/
int		main_meta_connection(Match *match)
{
  print_verbose(2, 0, "[Main] Sending data to meta servers.\n");
  if (server_call(&(match->_computer_server), match->_id, match->_port_server))
    return 1;
  if (clients_call(match))
    return 1;
  return 0;
}
