/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2007 Prologin
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE        65536

static void usage(const char *prgname)
{
  printf("usage: %s <limit>\n", prgname);
  printf("<limit> is in ko.\n");
  exit(10);
}

int main(int argc, char **argv)
{
  char buf[BUF_SIZE];
  int limit;
  int limit_reached = 0;
  int ret;

  if (argc < 2)
    usage(argv[0]);

  limit = atoi(argv[1]);
  if (limit <= 0)
    usage(argv[0]);
  limit *= 1024;

  while ((ret = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    if (!limit_reached)
      {
        limit -= ret;
        if (limit < 0)
          {
            ret -= -limit;
            limit_reached = 1;
          }
        write(STDOUT_FILENO, buf, ret);
      }
  }
  if (ret < 0)
    {
      perror("read");
      exit(11);
    }
  return limit_reached;
}
