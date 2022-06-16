#include <iostream>
#include <cassert>

class Creature {
protected:
    std::string m_name;
    char m_sim;
    int m_hp;
    int m_dmg;
    int m_gold;
public:
    Creature(std::string name,char sim,int hp,int dmg,int gold):m_name(name),m_sim(sim),m_hp(hp),m_dmg(dmg),m_gold(gold) {

    }
    std::string getName() { return m_name; }
    char getSim() { return m_sim; }
    int getHealth() { return m_hp; }
    int getDamage() { return m_dmg; }
    int getGold() { return m_gold; }
    void reduceHealth(int m) {
        m_hp -= m;
    }
    bool isDead() {
        if (m_hp <= 0) {
            return true;
        }
        else
            return false;
    }
    void addGold(int a) {
        m_gold += a;
    }
    
    
};
class Player:public Creature {
private:
    int m_level = 1;
    
public:
    
    Player(std::string name) :Creature(name,'@', 10, 1, 0) {
        
    }
    void levelUp() {
        ++m_level;
        ++m_dmg;
    }

    bool hasWon() {
        if (m_level >= 20) {
            return true;
        }
        else
            return false;
    }
    bool isDead() {
        if (m_hp <= 0)
            return true;
        else
            return false;
    }
    int getLevel() { return m_level; }
   
    using Creature::m_hp;

};

class Monster:public Creature {
protected:

public:
   
    struct MonsterData {
        std::string name;
        char symbol;
        int health;
        int damage;
        int gold;
        
    };
    enum Type {
        DRAGON,
        ORC,
        SLIME,
        MAX_TYPES
    };
    Monster(Type type) : Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold) {}
  static MonsterData monsterData[Monster::MAX_TYPES];
  static Type getRandomMonster() {

      static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
      return static_cast<Type>(rand() * fraction * (MAX_TYPES-1 + 1));
  }
  friend void attackMonster(Monster &m, Player &p);
  friend void attackPlayer(Monster& m, Player& p);
};


Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
    { "dragon", 'D', 20, 4, 100 },
    { "orc", 'o', 4, 2, 25 },
    { "slime", 's', 1, 1, 10 }
};
int getRandomNumForFight(int min=1,int max=2) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
void attackPlayer(Monster &m, Player &p) {
    p.m_hp = p.getHealth() - m.getDamage();
    std::cout << "Monster hit you for " << m.getDamage() << ".Now you have " << p.getHealth() << "hp." << std::endl;
}
void attackMonster(Monster &m,Player &p) {
    m.m_hp = m.getHealth() - p.getDamage();
  
    std::cout << "You hit the " << m.getName() << " for " << p.getDamage() << ".Now monster has " << m.m_hp << " hp." << std::endl;

}


void fightMonster(Monster& m, Player& p) {


    char choise;
    do {
     //   if (!p.isDead()) {
        if (p.isDead()) {
            std::cout << "You die with " << p.getLevel() << " lvl and " << p.getGold() << " gold." << std::endl;
            break;
        }
            do {
                std::cout << "Do you want (R)un or (F)ight?: ";
                std::cin >> choise;
            } while (choise != 'R' && choise != 'F');
            if (choise == 'R') {
                rand();
                int decision = getRandomNumForFight();
                assert(decision >= 1 && decision <= 2);
                if (decision == 1) {
                    std::cout << "You sucessfully run. We're going to your next enemy..." << std::endl;
                    break;
                }
                else if (decision == 2) {
                    std::cout << "Running failed.You must to fight!" << std::endl;
                    attackPlayer(m, p);
                }

            }
            else if (choise == 'F') {
                attackMonster(m, p);
                if (m.getHealth() <= 0) {
                    std::cout << "A " << m.getName() << " is dead now.You earn "<<m.getGold()<<" gold." << std::endl;
                    p.addGold(m.getGold());
                    p.levelUp();
                    std::cout << "Level up!Your level now:" << p.getLevel() << std::endl;

                }

                else {
                    attackPlayer(m, p);
                }

                if (p.hasWon()) {
                    std::cout << "You win with level " << p.getLevel() << " and " << p.getGold() << " gold.Congratulation!" << std::endl;
                }

         //   }
         
        }
        }while (m.getHealth() > 0);
    
}


int main()
{
    char answer;
    
    do {
        char choise;
        std::string n;
        std::cout << "Enter your name: ";
        std::cin >> n;
        Player p(n);
        std::cout << "Welcome," << p.getName() << ".\n" << "You have " << p.getHealth() << " health, " << p.getDamage() << " damage and are carrying " << p.getGold() << " gold." << std::endl;

        srand(static_cast<unsigned int>(time(0)));
        rand();
        do {
            Monster m = Monster::getRandomMonster();
            std::cout << "You met a " << m.getName() << " (" << m.getSim() << ").\n";
            fightMonster(m, p);
        } while (!p.hasWon() && !p.isDead());
        do {
            std::cout << "Do you want to play it again? -(Y)es,(N)o:";
            std::cin >> answer;
        } while (answer != 'Y' && answer != 'N');
    } while (answer != 'N');
    

    return 0;
}
