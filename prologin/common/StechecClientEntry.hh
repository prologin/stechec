/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/

#ifndef STECHECCLIENTENTRY_HH_
# define STECHECCLIENTENTRY_HH_

# include "StechecGameData.hh"
# include "StechecEntry.hh"

/*
** This class provides hooks for, either for real player or for visio.
**
** You can:
**  - overload any virual method you wants
**  - retrieve message from server: void fetchCommand(StechecPkt&)
**  - access to the GameData class: g_
**  - access to the Differ class: differ_
** 
** Note:
**  - afterGame() (where you usually free memory) is called only
**    if beforeGame() was called.
*/
class StechecClientEntry: protected StechecClient, public StechecEntry
{
public:

  StechecClientEntry(GameData* game, ClientDiffer* diff, Client* client);

protected:

  //! Use this fonction in prolo_* to receive data from server.
  //! @return false when empty.
  bool          fetchCommand(StechecPkt *data);

  ClientDiffer* differ_;
  
private:

  // the remaining is private
  friend class CRules;

  void          addPacket(const StechecPkt& pkt);
  void          flushPacketList();
  
  CommandList   cmd_list_;
  unsigned      index_;
};


inline StechecClientEntry::StechecClientEntry(GameData* game,
                                              ClientDiffer* diff,
                                              Client* client)
  : StechecClient(game, client),
    differ_(diff)
{
}

inline bool StechecClientEntry::fetchCommand(StechecPkt *data)
{
  if (index_ < cmd_list_.size())
    {
      *data = cmd_list_[index_++];
      return true;
    }
  return false;
}


inline void StechecClientEntry::addPacket(const StechecPkt& pkt) 
{
  assert(index_ == 0);
  cmd_list_.push_back(pkt);
}

inline void StechecClientEntry::flushPacketList()
{
  index_ = 0;
  cmd_list_.clear();
}

#endif // !STECHECCLIENTENTRY_HH_
