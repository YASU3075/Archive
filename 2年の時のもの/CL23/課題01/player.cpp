//-----------------------------------------------
// [player.cpp]
// 制作日：2024/05/24
// 制作者：安田晴人
//-----------------------------------------------

#include "player.h"
#include <string>

void Player::SetName(std::string name)
{
	m_name = name;
}

void Player::SetGender(std::string gender)
{
	m_gender = gender;
}

void Player::SetJob(std::string job)
{
	m_job = job;
}

void Player::SetLevel(int level)
{
	if (level < 0 || level > 100)
	{
		level = 0;
	}

	m_level = level;
}

void Player::SetHp(int hp)
{
	if (hp < 0 || hp > 100)
	{
		hp = 0;
	}

	m_hp = hp;
}

void Player::SetMp(int mp)
{
	if (mp < 0 || mp > 100)
	{
		mp = 0;
	}

	m_mp = mp;
}

void Player::SetAtk(int atk)
{
	if (atk < 0 || atk > 100)
	{
		atk = 0;
	}

	m_atk = atk;
}

void Player::SetDef(int def)
{
	if (def < 0 || def > 100)
	{
		def = 0;
	}

	m_def = def;
}

void Player::SetAgi(int agi)
{
	if (agi < 0 || agi > 100)
	{
		agi = 0;
	}

	m_agi = agi;
}

void Player::SetDex(int dex)
{
	if (dex < 0 || dex > 100)
	{
		dex = 0;
	}

	m_dex = dex;
}

void Player::SetWis(int wis)
{
	if (wis < 0 || wis > 100)
	{
		wis = 0;
	}

	m_wis = wis;
}


std::string Player::GetName()
{
	return m_name;
}

std::string Player::GetGender()
{
	return m_gender;
}

std::string Player::GetJob()
{
	return m_job;
}

int Player::GetLevel()
{
	return m_level;
}

int Player::GetHp()
{
	return m_hp;
}

int Player::GetMp()
{
	return m_mp;
}

int Player::GetAtk()
{
	return m_atk;
}

int Player::GetDef()
{
	return m_def;
}

int Player::GetAgi()
{
	return m_agi;
}

int Player::GetDex()
{
	return m_dex;
}

int Player::GetWis()
{
	return m_wis;
}