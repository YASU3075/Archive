//-----------------------------------------------
// [monster.cpp]
// 制作日：2024/07/10
// 制作者：安田晴人
//-----------------------------------------------

#include <string>
#include "monster.h"

void Monster::SetRace(std::string race)
{
	m_race = race;
}

void Monster::SetExp(int exp)
{
	if (exp < 0 || exp > 100)
	{
		exp = 0;
	}
	m_exp = exp;
}

std::string Monster::GetRace(void)
{
	return m_race;
}

int Monster::GetExp(void)
{
	return m_exp;
}
