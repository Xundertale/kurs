#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Character {
protected:
    string name;
    int hp;
    int attack;
    int level;
    int xp;
public:
    Character(const string& n, int h, int a) : name(n), hp(h), attack(a), level(1), xp(0) {}

    virtual ~Character() {}

    void gainXP(int amount) {
        xp += amount;
        while (xp >= getXPForNextLevel()) {
            levelUp();
        }
    }

    int getXPForNextLevel() const {
        return level * 30;
    }
    void increaseAttack(int amount) {
        attack += amount;
        cout << name << " атака увеличена на " << amount << " Текущий урон: " << attack << "\n";
    }

    void levelUp() {
        level++;
        hp = 100 + (level - 1) * 20;
        attack += 4;
        cout << name << " достиг уровня " << level << "! Уровень увеличен.\n";
        cout << "HP: " << hp << ", ДМГ: " << attack << "\n";
    }

    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0;
        cout << name << " получил " << dmg << " урона! Осталось HP: " << hp << "\n";
    }
    bool isAlive() const { return hp > 0; }
    string getName() const { return name; }
    int getHP() const { return hp; }
    int getAttack() const { return attack; }
    void heal(int amount) {}
    void printStatus() const {
        cout << name << " | Уровень: " << level << " | HP: " << hp << " | ДМГ: " << attack << "\n";
    }

    virtual void attackTarget(Character& target) {
        cout << name << " атакует " << target.getName() << "!\n";
        target.takeDamage(attack);
    }

};


class Player : public Character {
private:
    int gold;
public:
    vector<string> inventoryItems;
    Player(const string& n)
        : Character(n, 100, 15), gold(50) {
    }

    void gainGold(int amount) {
        gold += amount;
        cout << "Вы получили " << amount << " золота. Всего: " << gold << "\n";
    }

    void spendGold(int amount) {
        if (gold >= amount) {
            gold -= amount;
            cout << "Потрачено " << amount << " золота.\n";
        }
        else {
            cout << "Недостаточно золота!\n";
        }
    }
    int getGold() const { return gold; }

    void showInventory() {
        cout << "Инвентарь:\n";
        for (const auto& item : inventoryItems)
            cout << "- " << item << "\n";
    }
    void useHealthPotion() {
        auto it = find(inventoryItems.begin(), inventoryItems.end(), "Аптечка");
        if (it != inventoryItems.end()) {
            hp += 30;
            if (hp > 100) hp = 100;
            cout << "Вы использовали аптечку! HP восстановлено до " << hp << "\n";
            inventoryItems.erase(it);
        }
        else {
            cout << "У вас нет аптечки.\n";
        }

    }
};

class Enemy : public Character {
public:
    Enemy(const string& n, int hp, int atk)
        : Character(n, hp, atk) {
    }
};

class Location {
private:
    string name;
    vector<Enemy> enemies;
public:
    Location(const string& n) : name(n) {}
    void addEnemy(const Enemy& enemy) {
        enemies.push_back(enemy);
    }
    void showEnemies() {
        cout << "Враги в данной локации:\n";
        for (size_t i = 0; i < enemies.size(); ++i) {
            cout << i + 5 << ". " << enemies[i].getName()
                << " | HP: " << enemies[i].getHP() << "\n";
        }
    }
    Enemy* getEnemy(int index) {
        if (index >= 0 && index < (int)enemies.size())
            return &enemies[index];
        else
            return 0;
    }
    string getName() const { return name; }
};


class Game {
private:
    Player player;
    vector<Location> locations;
    int currentLocationIndex;

public:
    Game() : player("Герой"), currentLocationIndex(0) {

        Location loc1("Затерянный лес");
        loc1.addEnemy(Enemy("Палочный тролль", 30, 5));
        loc1.addEnemy(Enemy("Деревянный голем", 40, 2));
        loc1.addEnemy(Enemy("муравей гигант", 25, 5));
        loc1.addEnemy(Enemy("каменый голем", 80, 4));
        loc1.addEnemy(Enemy("босс троль", 35, 10));

        Location loc2("Древний замок");
        loc2.addEnemy(Enemy("Рыцарь-призрак", 60, 10));
        loc2.addEnemy(Enemy("Демон-защитник", 80, 15));
        loc2.addEnemy(Enemy("скелет", 35, 7));
        loc2.addEnemy(Enemy("нежить", 45, 8));
        loc2.addEnemy(Enemy("Скелет Санс. Санс Скелет", 16, 99));
        loc2.addEnemy(Enemy("Стражи бездны", 70, 14));
        loc2.addEnemy(Enemy("Дух-загадка", 55, 13));

        Location loc3("Вулканическая пещера");
        loc3.addEnemy(Enemy("Огненный элементаль", 45, 9));
        loc3.addEnemy(Enemy("Лавовый гремучий дракон", 120, 20));
        loc3.addEnemy(Enemy("Магма-существо", 60, 12));
        loc3.addEnemy(Enemy("Пылающий голем", 80, 15));
        loc3.addEnemy(Enemy("Коварный огненный духовник", 50, 16));

        Location loc4("Облачные вершины");
        loc4.addEnemy(Enemy("Вьючный грифон", 70, 14));
        loc4.addEnemy(Enemy("Облачный дракон", 150, 25));
        loc4.addEnemy(Enemy("Громоносец", 90, 18));
        loc4.addEnemy(Enemy("Небесный страж", 80, 16));
        loc4.addEnemy(Enemy("Тень урагана", 55, 19));

        Location loc5("Город слез");
        loc5.addEnemy(Enemy("Плачущий хаос", 70, 17));
        loc5.addEnemy(Enemy("Призрак плачющего мира", 60, 14));
        loc5.addEnemy(Enemy("Коралловый монстр", 50, 10));
        loc5.addEnemy(Enemy("Глубинный тролль", 80, 20));
        loc5.addEnemy(Enemy("Владыка плачущих глубин", 200, 30));

        locations.push_back(loc1);
        locations.push_back(loc2);
        locations.push_back(loc3);
        locations.push_back(loc4);
        locations.push_back(loc5);
    }


    void start() {
        cout << "Добро пожаловать в текстовую RPG!\n";
        printMenu();
        string choice;
        while (true) {
            cout << "\nВведите команду (1-7), или 'exit' для выхода:\n";
            cin >> choice;
            if (choice == "exit") break;
            handleCommand(choice);
        }
        cout << "Игра завершена.\n";
    }

    void printMenu() {
        cout << "--- Главное меню ---\n";
        cout << "1. Перемещение\n";
        cout << "2. Посмотреть статус\n";
        cout << "3. Осмотреть локацию\n";
        cout << "4. Начать бой\n";
        cout << "5. Посмотреть инвентарь\n";
        cout << "6. Зайти к прадовцу в лавку\n";
        cout << "7. Использовать аптечку\n";
        cout << "8. Выйти из игры\n";
    }

    void handleCommand(const string& cmd) {
        if (cmd == "1") {
            changeLocation();
        }
        else if (cmd == "2") {
            player.printStatus();
            cout << "Золото: " << player.getGold() << "\n";
        }
        else if (cmd == "3") {
            showLocationDetails();
        }
        else if (cmd == "4") {
            startBattle();
        }
        else if (cmd == "5") {
            player.showInventory();
        }
        else if (cmd == "6") {
            buyItem();
        }
        else if (cmd == "7") {
            player.useHealthPotion();
        }
        else if (cmd == "8") {
            exit(0);
        }
        else {
            cout << "Неверная команда.\n";
        }
    }

    void changeLocation() {
        cout << "Доступные локации:\n";
        for (size_t i = 0; i < locations.size(); ++i) {
            cout << i + 1 << ". " << locations[i].getName() << "\n";
        }
        int choice;
        cout << "Выберите номер локации: ";
        cin >> choice;
        if (choice > 0 && choice <= (int)locations.size()) {
            currentLocationIndex = choice - 1;
            cout << "Вы перешли в " << locations[currentLocationIndex].getName() << ".\n";
        }
        else {
            cout << "Неверный выбор.\n";
        }
    }

    void showLocationDetails() {
        Location& loc = locations[currentLocationIndex];
        cout << "Вы в " << loc.getName() << ".\n";
        loc.showEnemies();
    }

    void startBattle() {
        Location& loc = locations[currentLocationIndex];
        loc.showEnemies();
        int enemyChoice;
        cout << "Выберите врага для боя (номер): ";
        cin >> enemyChoice;
        Enemy* enemy = loc.getEnemy(enemyChoice - 1);
        if (enemy && enemy->isAlive()) {
            battle(player, *enemy);
        }
        else {
            cout << "Некорректный выбор врага.\n";
        }
    }

    void battle(Player& p, Enemy& e) {
        cout << "Бой начался!\n";
        while (p.isAlive() && e.isAlive()) {
            p.attackTarget(e);
            if (!e.isAlive()) break;
            e.attackTarget(p);
        }
        if (p.isAlive()) {
            cout << "Вы победили врага!\n";
                p.gainGold(25);
                p.gainXP(20);
            }
        
        else {
            cout << "Вы проIбали...\n";
        }
    }
    void buyItem() {
        cout << "Магазин:\n";
        cout << "1. Эссенция война +6 к атаке (Цена 35 золота)\n";
        cout << "2. Аптечка (Восстановит 30 HP, Цена 20 золота)\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            if (player.getGold() >= 30) {
                player.spendGold(30);
                player.increaseAttack(6);
                player.inventoryItems.push_back("Эссенция война +6 к атаке");
            }
            else {
                cout << "Недостаточно золота.\n";
            }
        }
        else if (choice == 2) {
            if (player.getGold() >= 15) {
                player.spendGold(20);
                player.inventoryItems.push_back("Аптечка");
                cout << "Аптечка добавлена в инвентарь.\n";
            }
            else {
                cout << "Недостаточно золота.\n";
            }
        }
        else {
            cout << "Некорректный выбор.\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    srand(time(0));
    Game game;
    game.start();
}