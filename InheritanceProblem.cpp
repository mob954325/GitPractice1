#include <iostream>
#include <vector>

using namespace std;

#pragma region ItemClasses

// Base Class
class Item
{
public:
	Item() = default;
	Item(int id, string name, int level, char rank) : id(id), name(name), level(level), rank(rank) {};
	virtual ~Item() {}

	virtual void ShowInfo()
	{
		cout << id << ", 이름 : " << name << ", Level : " << level << ", Rank : " << rank << endl;
	}

	void RankUp()
	{
		switch (rank)
		{
		case'C':
			rank = 'B';
			break;
		case'B':
			rank = 'A';
			break;
		case'A':
			rank = 'S';
			break;
		default:
			rank = 'S';
			break;
		}
	}

	void LevelUp()
	{
		if (level < 10)
		{
			level++;
		}
	}

	int GetId() { return id; }
	void SetId(int id) { this->id = id; }
protected:
	int id = -1;
	string name = "Empty";
	int level = 0;
	char rank = 0;
};

// Derived
class Weapon : public Item
{
public:
	Weapon(int id, string name, int level, int rank, int power) : Item(id, name, level, rank), power(power) {};
	~Weapon() {};

	void ShowInfo() override
	{
		cout << id << ", 이름 : " << name << " Level : " << level << " Rank : " << rank << " Power : " << power << endl;
	}
private:
	int power = 0;
};

class Armor : public Item
{
public:
	Armor(int id, string name, int level, int rank, int defence) : Item(id, name, level, rank), defence(defence) {};
	~Armor() {};

	void ShowInfo() override
	{
		cout << id << ", 이름 : " << name << " Level : " << level << " Rank : " << rank << " Defence : " << defence << endl;
	}
private:
	int defence = 0;
};

class Ring : public Item
{
public:
	Ring(int id, string name, int level, int rank, int magic) : Item(id, name, level, rank), magic(magic) {};
	~Ring() {};

	void ShowInfo() override
	{
		cout << id << ", 이름 : " << name << " Level : " << level << " Rank : " << rank << " Magic : " << magic << endl;
	}
private:
	int magic = 0;
};

class Shoe : public Item
{
public:
	Shoe(int id, string name, int level, int rank, int speed) : Item(id, name, level, rank), speed(speed) {};
	~Shoe() {};

	void ShowInfo() override
	{
		cout << id << ", 이름 : " << name << " Level : " << level << " Rank : " << rank << " Speed : " << speed << endl;
	}
private:
	int speed = 0;
};

#pragma endregion

#pragma region ItemManager
class ItemManager
{
private:
	Item** items;
	int capacity = 0; // 매니저 크기
	int itemCount = 0;	  // 현재 가지고 있는 아이템 개수

public:
	ItemManager(int size) 
	{ 
		items = new Item*[size];
		capacity = size;
		itemCount = 0;
	};

	~ItemManager() 
	{ 
		for (int i = 0; i < itemCount; i++)
		{
			delete items[i];
		}

		delete[] items; 
	};

	void AddItem(Item* item);
	void RemoveItem(int id);
	void LevelUp(int index);
	void ItemSynthesis(int targetIndex, int ingredientIndex);
	void Show();
};

void ItemManager::AddItem(Item* item)
{
	if (itemCount < capacity)
	{
		items[itemCount] = item;

		cout << "[ 아이템 추가 ]\n";
		item->ShowInfo();
		itemCount++;
	}
	else
	{
		cout << "ItemManager가 가득 찼습니다.\n";
		return;
	}
}

void ItemManager::RemoveItem(int id)
{
	for (int i = 0; i < itemCount; i++)
	{
		if(items[i]->GetId() == id)
		{
			delete items[i];

			// 배열 정리
			for (int j = i; j < itemCount - 1; j++)
			{
				items[j] = items[j + 1];
			}

			itemCount--;
			break;
		}
	}
}

void ItemManager::LevelUp(int id)
{
	for (int i = 0; i < itemCount; i++)
	{
		if (items[i]->GetId() == id)
		{
			items[i]->LevelUp();
		}
	}
}

void ItemManager::ItemSynthesis(int taregetId, int ingredientId)
{
	Item* target = nullptr;
	Item* ingredient = nullptr;
	int ingredientIndex = -1;
	for (int i = 0; i < itemCount; i++)
	{
		if (items[i]->GetId() == taregetId)
		{
			target = items[i];
		}
		else if (items[i]->GetId() == ingredientId)
		{
			ingredient = items[i];
			ingredientIndex = i;
		}
	}

	if (target->GetId() == -1 || ingredientIndex == -1)
	{
		cout << "잘못된 id입니다.\n";
		return;
	}
	else
	{
		target->RankUp();
		int nextId = max(taregetId, ingredientId) + 1;
		target->SetId(nextId);

		delete ingredient;
		for (int i = ingredientIndex; i < itemCount - 1; i++)
		{
			items[i] = items[i + 1];
		}
		itemCount--;
	}
}

void ItemManager::Show()
{
	cout << "목록\n";
	for (int i = 0; i < itemCount; i++)
	{
		items[i]->ShowInfo();
	}
}

#pragma endregion

#pragma region main
int main()
{
	cout << "아이템을 입력하세요. (고유아이디, 이름, 레벨, 등급)\n";

	ItemManager manager(8);

	Weapon* deger = new Weapon(1, "단검", 1, 'A', 10);
	manager.AddItem(deger);
	manager.Show();
	cout << endl;

	Armor* armor = new Armor(2, "갑옷", 1, 'B', 5);
	manager.AddItem(armor);
	manager.Show();
	cout << endl;

	Armor* armor2 = new Armor(3, "갑옷2", 1, 'B', 3);
	manager.AddItem(armor2);
	manager.Show();
	cout << endl;

	cout << "레벨업 할 아이템 입력: ";
	int id;
	cin >> id;

	manager.LevelUp(id);
	manager.Show();
	cout << endl;

	cout << "아이템 합성 하기  ( 아이템 2개 입력 - 등급 높은 아이템 하나)  (S, A, B, C 등급)\n";
	cout << "재료 : ";
	int target, ingredien;
	cin >> target >> ingredien;
	manager.ItemSynthesis(target, ingredien);
	manager.Show();
}
#pragma endregion