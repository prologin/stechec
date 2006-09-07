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
** `NOTICE' file in the root directory.
** 
** The TBT Team consists of people listed in the `AUTHORS' file.
*/

#ifndef SPLAYER_HH_
# define SPLAYER_HH_

# include "Team.hh"
# include "Player.hh"

class SRules;
class STeam;

class SPlayer : public Player
{
public:
  SPlayer(SRules* r, const MsgPlayerInfo* m, STeam* t);

  //! @brief Check some thing, if this player could be valid.
  bool acceptPlayerCreation();

  //! @brief Set player position.
  //! @note Use this function instead of setting pos_ directly, so
  //!   the field can be updated too.
  //! @param pos Where to move.
  //! @param advertise_client Whether or not send a MSG_PLAYERPOS to clients.
  void setPosition(const Position& pos, bool advertise_client = true);

  //! @brief Set new player status.
  void setStatus(enum eStatus new_status);

  //! @brief Set status to prone if he has to
  void setProne();
  
  //! @brief player goes in reserve if he can.
	// 		KO players try to regain consciousness
  void prepareKickoff();
	
  //! @brief Try some action, consider player's agility.
  bool tryAction(int modifier);
	
	//! @brief Finish the current action
  void finishAction(bool reroll);

	enum eRoll action_attempted_;
	bool reroll_enabled_;


private:

  void msgMove(const ActMove* m);
	void msgStandUp(const ActStandUp* m);
  void msgBlock(const ActBlock* m);
  void msgPass(const ActPass* m);
  bool filterMove(const ActMove* m);
  bool filterStandUp(const ActStandUp* m);
  bool filterBlock(const ActBlock* m);
  bool filterPass(const ActPass* m);
  
	void sendRoll(int result, int modifier, int required);
	
  //! @brief Move this player.
  //! @return non-zero if action failed.
  int doMove(const ActMove* m);
	//! @brief Stand up this player.
  void doStandUp(const ActStandUp* m);
  //! @brief Block a player.
  //! @return non-zero if action failed.
  int doBlock(const ActBlock* m);
  //! @brief Do a pass.
  //! @return non-zero if action failed.
  int doPass(const ActPass* m);

  void blockPushChoice(SPlayer* target);
  void blockPush(ActBlockPush* m);
  
	
	int finishMove(bool reroll);
	void finishStandUp(bool reroll);
	int finishPickUp(bool reroll);
	int finishThrow(bool reroll);
		
  //! @brief Check for armor and eventually injury.
  void checkArmor(int arMod, int injMod);
  //! @brief Roll for injury.
  void rollInjury(int injMod);
  //! @brief Roll for casualty.
  enum eStatus rollCasualty();

	Position aim_;
	
  SRules* r_;	///< Server rules.
  STeam* t_;	///< Player's team.

  SPlayer* target_push_; ///< Player that is being pushed after a block.
};


#endif /* !SPLAYER_HH_ */
