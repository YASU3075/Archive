//-----------------------------------------------
// guild.h
// 制作日：2024/06/28
// 制作者：安田晴人
//-----------------------------------------------

#include "guild.h"
#include "character.h"


Guild::Guild(int memberMax) : m_memberMax(memberMax)
{//コンストラクタ
	m_memberList = new Character* [m_memberMax];

	for (int i = 0; i < m_memberMax; i++)
	{
		m_memberList[i] = nullptr;
	}

}

Guild::~Guild()
{//デストラクタ
	delete[] m_memberList;
}

int Guild::AddMember(std::string name, std::string gender, std::string  job, int level,
	int hp, int mp, int atk, int def, int agi, int dex, int wis)
{
	//リストから開いている場所を探してキャラクターを追加する
	for (int i = 0; i < m_memberMax; i++)
	{
		if (m_memberList[i] == nullptr)
		{
			m_memberList[i] = new Character(name, gender, job, level, hp, mp, atk, def, agi, dex, wis);

			//追加したら配列のインデックス（添え字）をreturnして終了
			return i;
		}
	}

	return -1;
}

void Guild::DeleteMember(int id)
{//削除
	//もし-1だったら何もしないでreturn

	//もし範囲外だったら何もしないでreturn
	if (id < 0 || m_memberMax < id)
	{
		return;
	}

	//※対象のポインタにnullptrが入っていたら何もしないでreturn
	else if (m_memberList[id] == nullptr)
	{
		return;
	}

	//対象を削除する
	delete m_memberList[id];

	//対象のポインタにnullptrを入れる
	m_memberList[id] = nullptr;
}


int Guild::GetMemberByName(std::string name, int startId)
{//名前での検索
	if (startId < 0 || startId >= m_memberMax)
	{
		return -1;
	}

	for (int i = startId; i < m_memberMax; i++)
	{
		if (m_memberList[i] == nullptr)
		{
			continue;
		}

		if (m_memberList[i]->GetName() == name)
		{//名前が等しいとき
			return i;
		}

	}

	return -1;
}

bool Guild::CheckMemberByHp(int hp, int id, int range)
{
	if (id < 0 || id > m_memberMax || (range != 0 && range != 1))
	{
		return false;
	}

	if (m_memberList[id] != nullptr)
	{
		int cmp = m_memberList[id]->GetHp();

		if (range == 0)
		{//以上
			if (cmp >= hp)
			{
				return true;
			}
		}

		else if (range == 1)
		{//以下
			if (cmp <= hp)
			{
				return true;
			}
		}
	}

	return false;
}

void Guild::ShowCharacter(int id)
{//指定したキャラクターを表示

	if (id < 0 || id >= m_memberMax)
	{
		return;
	}

	if (m_memberList[id] != nullptr)
	{//指定したキャラクターがヌルで無かったら
		std::cout << "=================================" << std::endl;
		std::cout << "   ID ：" << id + 1 << std::endl;
		std::cout << "　名前：" << m_memberList[id]->GetName() << std::endl;
		std::cout << "=================================" << std::endl << std::endl;
	}

}

void Guild::ShowCharacterAllStatus(int id)
{//指定したキャラクターを表示

	if (id < 0 || id >= m_memberMax)
	{
		return;
	}

	if (m_memberList[id] != nullptr)
	{//指定したキャラクターがヌルで無かったら
		std::cout << "=================================" << std::endl;
		std::cout << "   ID ：" << id + 1 << std::endl;
		std::cout << "　名前：" << m_memberList[id]->GetName() << std::endl;
		std::cout << "　性別：" << m_memberList[id]->GetGender() << std::endl;
		std::cout << "　職業：" << m_memberList[id]->GetJob() << std::endl;
		std::cout << "レベル：" << m_memberList[id]->GetLevel() << std::endl;
		std::cout << "　体力：" << m_memberList[id]->GetHp() << std::endl;
		std::cout << "魔法力：" << m_memberList[id]->GetMp() << std::endl;
		std::cout << "攻撃力：" << m_memberList[id]->GetAtk() << std::endl;
		std::cout << "防御力：" << m_memberList[id]->GetDef() << std::endl;
		std::cout << "素早さ：" << m_memberList[id]->GetAgi() << std::endl;
		std::cout << "器用さ：" << m_memberList[id]->GetDex() << std::endl;
		std::cout << "　賢さ：" << m_memberList[id]->GetWis() << std::endl;
		std::cout << "=================================" << std::endl << std::endl;
	}

}

void Guild::ShowMembers()
{//全キャラクターを表示
	for (int i = 0; i < m_memberMax; i++)
	{
		ShowCharacter(i);
	}
}



void Guild::SortMemberByLevel()
{//レベルでのソート
	
	int level[MEMBER_MAX] = { NULL };

	for (int i = 0; i < m_memberMax; i++)
	{
		if (m_memberList[i] != nullptr)
		{
			//キャラクターを作成してたらそのレベルを配列に格納
			level[i] = m_memberList[i]->GetLevel();
		}
	}

	for (int i = 0; i < m_memberMax; i++)
	{
		if (level[i] != NULL)
		{
			for (int j = i; j < m_memberMax; j++)
			{
				if (level[j] != NULL && level[j] < level[i])
				{
					int work = level[i];
					Character* pCharacter = m_memberList[i];

					level[i] = level[j];
					m_memberList[i] = m_memberList[j];

					level[j] = work;
					m_memberList[j] = pCharacter;

				}
			}
		}
	}

}

void Guild::SaveMembers()
{//セーブ
	std::ofstream file;
	file.open("savedata.txt");
	int select = 0;

	if (file.fail())
	{
		std::cout << "ファイルを開けませんでした" << std::endl;
		file.close();
		return;
	}

	std::cout << "現在のキャラクターをセーブしてよろしいですか？" << std::endl;
	std::cout << "[1:はい]  [2:いいえ]" << std::endl;
	std::cin >> select;

	if (select == 1)
	{

		for (int i = 0; i < m_memberMax; i++)
		{
			if (m_memberList[i] != nullptr)
			{
				file << m_memberList[i]->GetName() << " ";
				file << m_memberList[i]->GetGender() << " ";
				file << m_memberList[i]->GetJob() << " ";
				file << m_memberList[i]->GetLevel() << " ";
				file << m_memberList[i]->GetHp() << " ";
				file << m_memberList[i]->GetMp() << " ";
				file << m_memberList[i]->GetAtk() << " ";
				file << m_memberList[i]->GetDef() << " ";
				file << m_memberList[i]->GetAgi() << " ";
				file << m_memberList[i]->GetDex() << " ";
				file << m_memberList[i]->GetWis() << " " << std::endl;
			}
		}

		std::cout << "セーブ完了" << std::endl;
	}

	else
	{
		std::cout << std::endl << "セーブがキャンセルされました" << std::endl;
	}

	file.close();
}

void Guild::LoadMembers()
{//ロード
	std::ifstream file;
	file.open("savedata.txt");

	if (file.fail())
	{
		std::cout << "ファイルを開けませんでした" << std::endl;
		file.close();
		return;
	}

	std::string name;
	std::string gender;
	std::string  job;
	int level = 0;
	int hp = 0;
	int mp = 0;
	int atk = 0;
	int def = 0;
	int agi = 0;
	int dex = 0;
	int wis = 0;
	int num = 0;
	int select;

	std::cout << "現在のキャラクターを削除してセーブされているキャラクターをロードしますか？" << std::endl;
	std::cout << "[1:はい]  [2:いいえ]" << std::endl;
	std::cin >> select;
	
	if (select == 1)
	{

		for (int i = 0; i < m_memberMax; i++)
		{
			DeleteMember(i);
		}

		while (file >> name >> gender >> job >> level >> hp >> mp >> atk >> def >> agi >> dex >> wis)
		{
			m_memberList[num] = new Character(name, gender, job, level, hp, mp, atk, def, agi, dex, wis);
			num++;

		}

		std::cout << "ロード完了" << std::endl;

		ShowMembers();
	}

	else
	{
		std::cout << std::endl << "ロードがキャンセルされました" << std::endl;
	}

	file.close();
}
