//----------------------------------------------
// battle.h
// §ì“ú:2024/09/06
// §ìÒ:ˆÀ“c°l
//----------------------------------------------

#ifndef _BATTLE_H
#define _BATTLE_H

#include "fighter.h"

class Battle
{
private:
	Fighter* m_Fighter[2];

public:
	Battle(Fighter* pFighterA, Fighter* pFighterB);
	~Battle() {}

	void Fight(void);
};


#endif

