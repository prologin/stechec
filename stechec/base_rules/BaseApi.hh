/*
** TowBowlTactics, an adaptation of the tabletop game Blood Bowl.
** 
** Copyright (C) 2006, 2007 The TBT Team.
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

#ifndef BASEAPI_HH_
# define BASEAPI_HH_

# include "BaseCRules.hh"

class Event;

/*!
** Base class for API. Add the common API methods for all
** contests.
*/
template <typename T>
class BaseApi
{
public:
  BaseApi(T* rules);
  virtual ~BaseApi();

  //! @brief Set the class (usually, in the GUI) that will
  //!  receive rules event. This class must inherit from Event.
  void          setEventHandler(Event* evp);

  //! @brief Get the current game state.
  //! @return A constant describing the current game state.
  int           getState() const;

  //! @brief Get the current game state, stringinfied.
  const char*	getStateString() const;
  
  //! @brief Get the team id [0..nbTeams-1].
  int           getTeamId() const;

  //! @brief Get the number of real team (spectators not counted)
  //!  that are playing.
  int           getTeamNumber() const;

  //! @brief Check if the rules is processing some command, or if it
  //!  safe to send the next one in a sequence.
  //! @see BaseCRules::isBusy()
  //! @return true if rules is processing a message.
  bool		isBusy() const;
  
protected:
  T*            rules_; ///< Contest CRules.
};

template <typename T>
inline BaseApi<T>::BaseApi(T* rules)
  : rules_(rules)
{
}

template <typename T>
inline BaseApi<T>::~BaseApi()
{
}

template <typename T>
inline void BaseApi<T>::setEventHandler(Event* ev)
{
  rules_->setEventHandler(ev);
}

template <typename T>
inline int BaseApi<T>::getState() const
{
  return rules_->getState();
}

template <typename T>
inline const char* BaseApi<T>::getStateString() const
{
  switch (rules_->getState())
    {
    case GS_WAIT:
      return "GS_WAIT";
    case GS_END:
      return "GS_END";
    }
  return "Undefined";
}

template <typename T>
inline int BaseApi<T>::getTeamId() const
{
  return rules_->getCoachId();
}

template <typename T>
inline int BaseApi<T>::getTeamNumber() const
{
  return rules_->getTeamNumber();
}

template <typename T>
inline bool BaseApi<T>::isBusy() const
{
  return rules_->isBusy();
}

#endif /* !BASEAPI_HH_ */
