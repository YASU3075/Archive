//-----------------------------------------------
// cat.h
// 制作日：2024/07/19
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _CAT
#define _CAT

#include "animal.h"

class Cat : public Animal
{
public:
	Cat() {}
	Cat(std::string name) : Animal(name) {}
	~Cat() {}

	void Action() override
	{
		std::cout << "猫の" << GetName();
		std::cout << "は「にゃ～ん」と鳴いた。\n";
	}

};

#endif
