#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;
/*최우선으로 상점
상점은 장비 아이템 상점, 소비 아이템 상점이 있다.(부모 클래스 상점 밑에 자녀 클래스로 장비, 소비 상점 분리)

소유 아이템을 상인에게 판매할 수 있어야 한다.판매가격은 원가의 60 % 이다.

유저 인벤토리와 직접적인 상호작용이 필요하다.*/

class Item 
{
protected:
	string name;
	int price;
public:
	Item(string n, int p) : name(n), price(p) {}
	virtual ~Item() = default;

	string getName() const { return name; }
	int getPrice() const { return price; }
	virtual void use() = 0;
};

class Weapon : public Item 
{
private:
	int attackPower;
public:
	Weapon(string n, int p, int atk) : Item(n, p), attackPower(atk) {}

	void use() override {
		cout << name << "을(를) 장착했습니다! 공격력 +" << attackPower << endl;
	}
};

class Potion : public Item 
{
public:
	Potion(string n, int p) : Item(n, p) {}

	virtual void use() = 0;
};

class HealingPotion : public Potion
{
private:
	int healing;
public:
	HealingPotion(string n, int p, int heal) : Potion(n, p), healing(heal){}
	void use() override {
		cout << name << "을(를) 사용했습니다. " << healing << "만큼 회복합니다." << endl;
	}
};

class StrengthPotion : public Potion
{
private:
	int strength;
public:
	StrengthPotion(string n, int p, int str) : Potion(n, p), strength(str) {}
	void use() override {
		cout << name << "을(를) 사용했습니다. " << strength << "만큼 공격력이 상승합니다." << endl;
	}
};

// ---------------------- 플레이어 ----------------------
class Player 
{
public:
	int gold = 500;
	vector<unique_ptr<Item>> inventory;

	void showInventory() const {
		cout << "[인벤토리]\n";
		for (size_t i = 0; i < inventory.size(); ++i) {
			cout << i + 1 << ". " << inventory[i]->getName() << endl;
		}
	}

	void addItem(unique_ptr<Item> item) {
		inventory.push_back(move(item));
	}
};

//------------------------------상점-------------------------------
class Shop 
{
protected:
	vector<unique_ptr<Item>> items;
public:
	void useritem(Player& player)
	{
		player.showInventory();
	}
	virtual void showitem() = 0;
	virtual void buying(Player& player) = 0;
	virtual void selling(Player& player) = 0;
	virtual ~Shop() = default;

	void addItem(unique_ptr<Item> item) {
		items.push_back(move(item));
	}
};

class Blacksmith : public Shop 
{
public:
	Blacksmith()
	{
		addItem(make_unique<Weapon>("Iron Sword", 100, 10));
		addItem(make_unique<Weapon>("Katana", 200, 20));
		addItem(make_unique<Weapon>("Two-Handed Sword", 400, 30));
	}
	void showitem() override 
	{
		for (size_t i = 0; i < items.size(); ++i) 
		{
			cout << i + 1 << ". " << items[i]->getName() << " - " << items[i]->getPrice() << "G" << endl;
		}
		//아이템 전체 목록 cout
	}
	void buying(Player& player) override
	{
		showitem();
		cout << "보유 금화 : " << player.gold << endl;
		cout << "구매할 아이템 : ";
		int i;
		cin >> i;
		if(i < 1 || i >items.size())
		{
			cout << "입력이 잘못됐습니다." << endl;
			return;
		}
		else 
		{
			if (player.gold >= items[i - 1]->getPrice())
			{
				cout << items[i - 1]->getName() << "을 구매했습니다" << endl;
				player.gold -= items[i - 1]->getPrice();
				player.addItem(move(items[i - 1]));
				items.erase(items.begin() + (i - 1));
				cout << "잔여 금화 : " << player.gold << endl;
			}
			else
			{
				cout << "금화가 부족합니다.";
			}
		}
		//인벤토리에 접근해서 gold -= cost;
		//아이템 추가 inven.push_back(뭐시기)
	}
	void selling(Player& player) override
	{
		player.showInventory();
		cout << "판매할 아이템 : ";
		int i;
		cin >> i;
		if (i < 1 || i > player.inventory.size()) 
		{
			cout << "입력이 잘못됐습니다." << endl;
			return;
		}
		player.gold += player.inventory[i - 1]->getPrice()*6/10;
		items.push_back(move(player.inventory[i - 1]));
		player.inventory.erase(player.inventory.begin() + (i - 1));

		//인벤토리에 접근해서 gold += cost*6/10;
		//아이템 차감
	}
};

class ItemShop : public Shop
{
public:
	ItemShop()
	{
		addItem(make_unique<HealingPotion>("Healing Potion", 50, 50));
		addItem(make_unique<StrengthPotion>("Strength Potion", 200, 10));
	}
	void showitem() override
	{
		for (size_t i = 0; i < items.size(); ++i)
		{
			cout << i + 1 << ". " << items[i]->getName() << " - " << items[i]->getPrice() << "G" << endl;
		}
		//아이템 전체 목록 cout
	}
	void buying(Player& player) override
	{
		showitem();
		cout << "보유 금화 : " << player.gold << endl;
		cout << "구매할 아이템 : ";
		int i;
		cin >> i;
		if (i < 1 || i >items.size())
		{
			cout << "입력이 잘못됐습니다." << endl;
			return;
		}
		else
		{
			if (player.gold >= items[i - 1]->getPrice())
			{
				cout << items[i - 1]->getName() << "을 구매했습니다" << endl;
				player.gold -= items[i - 1]->getPrice();
				player.addItem(move(items[i - 1]));
				items.erase(items.begin() + (i - 1));
				cout << "잔여 금화 : " << player.gold << endl;
			}
			else
			{
				cout << "금화가 부족합니다.";
			}
		}
		//인벤토리에 접근해서 gold -= cost;
		//아이템 추가 inven.push_back(뭐시기)
	}
	void selling(Player& player) override
	{
		player.showInventory();
		cout << "판매할 아이템 : ";
		int i;
		cin >> i;
		if (i < 1 || i > player.inventory.size())
		{
			cout << "입력이 잘못됐습니다." << endl;
			return;
		}
		player.gold += player.inventory[i - 1]->getPrice() * 6 / 10;
		items.push_back(move(player.inventory[i - 1]));
		player.inventory.erase(player.inventory.begin() + (i - 1));
		//인벤토리에 접근해서 gold += cost*6/10;
		//아이템 차감
	}
};


int main() {
	Player player;
	Blacksmith blacksmith;
	ItemShop itemshop;

	while (true) {
		cout << "\n--- 메뉴 ---\n";
		cout << "1. 대장간 보기\n";
		cout << "2. 대장간 구매\n";
		cout << "3. 대장간 판매\n";
		cout << "4. 소비 아이템 상점 보기\n";
		cout << "5. 소비 아이템 상점 구매\n";
		cout << "6. 소비 아이템 상점 판매\n";
		cout << "7. 인벤토리 보기\n";
		cout << "8. 종료\n";
		cout << "선택: ";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1:
			blacksmith.showitem();
			break;
		case 2:
			blacksmith.buying(player);
			break;
		case 3:
			blacksmith.selling(player);
			break;
		case 4:
			itemshop.showitem();
			break;
		case 5:
			itemshop.buying(player);
			break;
		case 6:
			itemshop.selling(player);
			break;
		case 7:
			player.showInventory();
			break;
		case 8:
			cout << "프로그램을 종료합니다.\n";
			return 0;
		default:
			cout << "잘못된 입력입니다.\n";
		}
	}
}
