//-----------------------------------------------
// CL23-02-使ったら責任もって後片付け
// character.cpp
// 制作日：2024/06/23
// 制作者：安田晴人
//-----------------------------------------------
#include <string>
#include "character.h"


void Character::SetName(std::string name)
{
	m_name = name;
}

void Character::SetGender(std::string gender)
{
	m_gender = gender;
}

void Character::SetJob(std::string job)
{
	m_job = job;
}

void Character::SetLevel(int level)
{
	if (level < 0 || level > 100)
	{
		level = 0;
	}

	m_level = level;
}

void Character::SetHp(int hp)
{
	if (hp < 0 || hp > 100)
	{
		hp = 0;
	}

	m_hp = hp;
}

void Character::SetMp(int mp)
{
	if (mp < 0 || mp > 100)
	{
		mp = 0;
	}

	m_mp = mp;
}

void Character::SetAtk(int atk)
{
	if (atk < 0 || atk > 100)
	{
		atk = 0;
	}

	m_atk = atk;
}

void Character::SetDef(int def)
{
	if (def < 0 || def > 100)
	{
		def = 0;
	}

	m_def = def;
}

void Character::SetAgi(int agi)
{
	if (agi < 0 || agi > 100)
	{
		agi = 0;
	}

	m_agi = agi;
}

void Character::SetDex(int dex)
{
	if (dex < 0 || dex > 100)
	{
		dex = 0;
	}

	m_dex = dex;
}

void Character::SetWis(int wis)
{
	if (wis < 0 || wis > 100)
	{
		wis = 0;
	}

	m_wis = wis;
}


std::string Character::GetName()
{
	return m_name;
}

std::string Character::GetGender()
{
	return m_gender;
}

std::string Character::GetJob()
{
	return m_job;
}

int Character::GetLevel()
{
	return m_level;
}

int Character::GetHp()
{
	return m_hp;
}

int Character::GetMp()
{
	return m_mp;
}

int Character::GetAtk()
{
	return m_atk;
}

int Character::GetDef()
{
	return m_def;
}

int Character::GetAgi()
{
	return m_agi;
}

int Character::GetDex()
{
	return m_dex;
}

int Character::GetWis()
{
	return m_wis;
}