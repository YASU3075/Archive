//-----------------------------------------------
// LSystem.cpp
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#include "renderer/LSystem.h"

//-----------------------------------------------
// ルールを追加する関数
//-----------------------------------------------
void LSYSTEM::AddRule(char symbol, const std::string& replacement)
{
	m_rules[symbol] = replacement;
}

//-----------------------------------------------
// 指定された回数だけ生成を行う関数
//-----------------------------------------------
std::string LSYSTEM::Generate(int iterations) const
{
	std::string current = m_axiom;
	for (int i = 0; i < iterations; i++)
	{
		std::string next;
		for (char c : current)
		{
			auto it = m_rules.find(c);
			if (it != m_rules.end())
			{
				next += it->second; // ルールが存在する場合、置換
			}
			else
			{
				next += c; // ルールが存在しない場合、そのまま
			}
		}
		current = next;
	}
	return current;
}