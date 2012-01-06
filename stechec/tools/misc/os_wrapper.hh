/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006 The TBT Team.
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef OS_WRAPPER_HH_
# define OS_WRAPPER_HH_

/*!
** @file os_wrapper.hh
** @brief Reimplement or wrap functions that doesn't exists on 'exotic' systems.
** @ingroup tools_misc
**
** Disclaimer: This file is not intended to be clean, nice, nor beautiful.
**
** The reference OS is GNU/Linux.
*/

//! @ingroup tools_misc
//! @{

# include <cstdarg>

//=====================================================================
// WIN32
//=====================================================================
# ifdef WIN32

#  define NOMINMAX
#  include <WinSock2.h>

// === Common functions
#  define MAX_PATH PATH_MAX
inline char* realpath(const char* pathname, char resolved_path[PATH_MAX])
{
  // FIXME: does nothing.
  strcpy(resolved_path, pathname);
  return resolved_path;
}

inline void sleep(int time)
{
  Sleep(time);
}

#  define min(A, B) _cpp_min(A, B)
#  define max(A, B) _cpp_max(A, B)


// === Sockets

typedef int socklen_t;
# define POLLIN 0x0001
struct pollfd {
    int fd;
    short events;
    short revents;
};
// poll() doesn't exists on win32. Wrap using select.
int poll(struct pollfd* pollfds, int nfds, int timeout);

// === Threads
#  include <process.h>
typedef HANDLE pthread_t;
typedef void* pthread_attr_t; // not used.
typedef CRITICAL_SECTION pthread_mutex_t;
typedef void* pthread_mutexattr_t; // not used.

inline pthread_t pthread_self()
{
  return (pthread_t)GetCurrentThreadId();
}

inline int pthread_create(pthread_t* thread, 
                          pthread_attr_t* attr, 
                          void* (*start_routine)(void *), void* arg)
{
  if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine,
                   arg, 0, NULL) == NULL)
    return 1;
  return 0;
}

inline int pthread_join(pthread_t thread, void** value_ptr)
{
  // FIXME: todo
  return 0;
}

inline int pthread_mutex_init(pthread_mutex_t* mutex,
                              const pthread_mutexattr_t* mutexattr)
{
  InitializeCriticalSection(mutex);
  return 0;
}

inline int pthread_mutex_lock(pthread_mutex_t* mutex)
{
  EnterCriticalSection(mutex);
  return 0;
}

inline int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
  LeaveCriticalSection(mutex);
  return 0;
}

inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
  DeleteCriticalSection(mutex);
  return 0;
}

// === <config.h> (not generated under windows).
#  define PACKAGE_VERSION "0.7M1"

//=====================================================================
// NOT WIN32
//=====================================================================
# else

// unistd.h doesn't exists on windows.
#  include <unistd.h>
#  include <sys/socket.h>
#  include <sys/poll.h>
#  include <sys/time.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  define closesocket close

#  include <pthread.h>

#  include <config.h>

# endif // !WIN32

// hack for BSDs
#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif

void initialize_socket();

//! @}

#endif /* !OS_WRAPPER_HH_ */
