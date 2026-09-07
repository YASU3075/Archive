//-----------------------------------------------
// ポリモーフィズムの演習
// main.cpp
// 制作日：2024/07/19
// 制作者：安田晴人
//-----------------------------------------------

#include "animal.h"
#include "cat.h" 
#include "dog.h" 
#include "armadillo.h" 
#include <random>

static Animal* AnimalFactory(int n, std::string name)
{
    Animal* pAnimal = nullptr;

    switch (n)
    {
    case 0:
        pAnimal = new Cat(name);
        break;

    case 1:
        pAnimal = new Dog(name);
        break;

    case 2:
        pAnimal = new Armadillo(name);
        break;

    default:
        break;
    }

    return pAnimal;
}


int main()
{
    std::random_device rd;
    //std::mt19937 mt(rd());

    std::string names[] =
    {
        "ミケランジェロ",
        "そういちろう",
        "パン",
        "みかん",
        "ポチ",
        "クリスティーナ"
    };

    
    //パターン①
    Animal* pAnimal = AnimalFactory(rd() % 3, names[rd() % 6]);

    std::cout << "何の動物がいるのかな？\n";
    pAnimal->Action();

    delete pAnimal;

    //パターン②
    Animal* pAnimals[10];

    //初期化？
    for (int i = 0; i < 10; i++)
    {
        pAnimals[i] = AnimalFactory(rd() % 3, names[rd() % 6]);
    }

    //更新とか描画？
    for (int i = 0; i < 10; i++)
    {
        pAnimals[i]->Action();
    }
    
    //あとかたづけ
    for (int i = 0; i < 10; i++)
    {
        delete pAnimals[i];
    }

}


