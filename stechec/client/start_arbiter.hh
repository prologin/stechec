/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2006 Prologin
*/

#ifndef START_ARBITER_HH_
# define START_ARBITER_HH_

/*!
** @file stechec/client/start_arbiter.hh
** @ingroup client
*/

# include "xml/xml_config.hh"

//! @ingroup client
//! @{

bool start_arbiter(xml::XMLConfig& cfg);
void clean_arbiter();

//! @}

#endif /* !START_ARBITER_HH_ */
