//-----------------------------------------------
// LSystem.h
// 制作日：2025/12/17
// 制作者：安田晴人
//-----------------------------------------------
#pragma once

#include <string>
#include <unordered_map>

class LSYSTEM
{
private:
	std::string m_axiom;
	std::unordered_map<char, std::string> m_rules;

public:
	LSYSTEM(const std::string& axiom) : m_axiom(axiom) {}
	void AddRule(char symbol, const std::string& replacement);
	std::string Generate(int iterations) const;
};