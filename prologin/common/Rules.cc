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

# include "ClientCx.hh"
# include "Contest.hh"
# include "CRules.hh"

// Here we are after the connection with the server, but before the UI
// is loaded (client-side only). It is useful to set the rules in a
// consistent (ie usable) state _before_ loading champion's library, so the
// champion could initialize its static variables with api call (useful for
// foreign language like caml).
extern "C" bool hook_before_ui_load(ClientCx* ccx, BaseCRules* rules)
{
  StechecApi* api = reinterpret_cast<StechecApi*>(rules->getApi());

  while (api->getState() == GS_WAIT || api->getState() == GS_BEFOREGAME)
    ccx->process(true);

  return api->getState() != GS_END;
}

std::ostream& operator<< (std::ostream& os, const StechecPkt& pkt)
{
  os << pkt.type;
  if (pkt.nb_args > 0)
    {
      os << " [";
      for (int i = 0; i < pkt.nb_args - 1; i++)
        os << pkt.arg[i] << ", ";
      os << pkt.arg[pkt.nb_args - 1] << "]";
    }
  os << " (client_id: " << pkt.client_id << ")";
  return os;
}
