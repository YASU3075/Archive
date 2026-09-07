//-----------------------------------------------
// fighter.h
// 制作日：2024/09/14
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _FIGHTER_H
#define _FIGHTER_H

#include "character.h"

class Fighter : public Character
{
public:
	Fighter() {}
	Fighter(std::string name, std::string gender, std::string job, int level, int hp, int mp, int atk, int def, int agi, int dex, int wis) :
		Character(name, gender, job, level, hp, mp, atk, def, agi, dex, wis) {}
	~Fighter() {}

	//純粋仮想関数
	virtual void Attack() = 0;
	virtual void Defence() = 0;
	virtual int Damage() = 0;
};

#endif