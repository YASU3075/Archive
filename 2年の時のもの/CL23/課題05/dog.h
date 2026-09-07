//-----------------------------------------------
// dog.h
// 制作日：2024/07/19
// 制作者：安田晴人
//-----------------------------------------------

#ifndef _DOG
#define _DOG

#include "animal.h"

class Dog : public Animal
{
public:
	Dog() {}
	Dog(std::string name) : Animal(name) {}
	~Dog() {}

	void Action() override
	{
		std::cout << "犬の" << GetName();
		std::cout << "は「わんわん」と吠えた。\n";
	}

};

#endif
