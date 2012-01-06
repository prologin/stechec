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

#include "tools.hh"
#include "log.hh"

#include <cstring>

Log::ThreadList Log::thread_list_;
int Log::ref_count_ = 0;
pthread_mutex_t Log::lock_;


Log::Log(int verbose_level, const char* suffix)
{
  if (!ref_count_++)
    pthread_mutex_init(&lock_, NULL);

  pth_id_ = pthread_self();

  print_loc_ = false;
  use_color_ = true;
  
  pthread_mutex_lock(&lock_);
  thread_list_.insert(std::make_pair(pth_id_, this));
  pthread_mutex_unlock(&lock_);

  setVerboseLevel(verbose_level);
  setModuleSuffix(suffix);
}

Log::~Log()
{
  pthread_mutex_lock(&lock_);
  thread_list_.erase(pth_id_);
  pthread_mutex_unlock(&lock_);

  if (!--ref_count_)
    pthread_mutex_destroy(&lock_);
}

void Log::setVerboseLevel(int level)
{
  verbose_mask_ = 0;
  for (int i = 0; i <= level + 1; i++)
    verbose_mask_ |= 1 << i;
}

void Log::setVerboseMask(int mask)
{
  verbose_mask_ = mask;
}

void Log::setPrintLoc(bool enable)
{
  print_loc_ = enable;
}

void Log::setUseColor(bool enable)
{
  use_color_ = enable;
}

void Log::setModuleSuffix(const char* name)
{
  if (name != NULL && name[0])
    {
      strncpy(orig_modsuffix_, name, 15);
      orig_modsuffix_[15] = 0;
      sprintf(modsuffix_, "%s", orig_modsuffix_);        
    }
  else
    {
      modsuffix_[0] = 0;
      orig_modsuffix_[0] = 0;
    }
}
