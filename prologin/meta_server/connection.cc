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

/* #include <sys/socketvar.h> */
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "verboseur.hh"
#include "conf.hh"

/*
** Bind the socket on a free port
*/
static int              my_bind(const int socket, int *port)
{
  struct sockaddr_in    sa_in;

  print_verbose(3, 0, "[Meta] Binding server.\n");
  memset(&sa_in, 0, sizeof (sa_in));
  sa_in.sin_family = AF_INET;
  sa_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sa_in.sin_port = htons(*port);
  while (bind(socket, (struct sockaddr *) &sa_in, sizeof (sa_in)) == -1)
    sa_in.sin_port = htons(++(*port));
  print_verbose(3, 0, "[Meta] Bind on port: %d.\n", *port);
  return *port;
}

/*
** Creation d'un serveur d'ecoute
*/
int		lisen_connect(int *port)
{
  int           open_socket = 0;

  print_verbose(3, 0, "[Meta] Initializing connection.\n");
  if ((open_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return print_verbose(1, -1, "[Meta] Error: %s\n.", strerror(errno));
  if (((*port) = my_bind(open_socket, port)) == -1)
    return -1;
  if (listen(open_socket, 10) == -1)
    return print_verbose(1, -1, "[Meta] Error: %s\n.", strerror(errno));
  print_verbose(3, 0, "[Meta] Connection initialized\n");
  return open_socket;
}


