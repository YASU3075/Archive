//-----------------------------------------------
// armadillo.h
// 制作日：2024/07/19
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _ARMADILLO
#define _ARMADILLO

#include "animal.h"

class Armadillo : public Animal
{
public:
	Armadillo() {}
	Armadillo(std::string name) : Animal(name) {}
	~Armadillo() {}

	void Action() override
	{
		std::cout << "アルマジロの" << GetName();
		std::cout << "は「コロコロ」と転がった。\n";
	}

};

#endif
