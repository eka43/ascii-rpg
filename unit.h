#ifndef UNIT_H
#define UNIT_H

class Item;
class Prop;
class Propzz;
class Board;
class Inventory;

class Unit { // abstract class: This class contains one or more pure virtual functions. This class cannot be used to create an object.
protected:
    char shape;
    
    int row;
    int col;

    int dir;

    int hp;
    int maxHp;
    int mp;
    int maxMp;
    int atk;
    int def;
    int gold;
    int exp;

    Board *board;
    
    bool died;
    bool frozen;

public:
    Unit();
    Unit(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
    void init(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
    virtual ~Unit();
    
    void print();
    virtual void printStat();

    void setBoard(Board *board);
    
    int getRow();
    void setRow(int row);

    int getCol();
    void setCol(int col);

    int getDir();

    char getShape();
    
    bool incHp(int hpInc);
    void decHp(int hpDec);
    
    int getMp();
    bool incMp(int mpInc);
    void decMp(int mpDec);
    
    int getAtk();
    void incAtk(int atkInc);
    void decAtk(int atkDec);
    
    int getDef();
    void incDef(int defInc);
    void decDef(int defDec);
    
    int getGold();
    void incGold(int gold);
    bool decGold(int gold);
    
    int getExp();
    void incExp(int exp);
    void decExp(int exp);

	bool isDead();
    
    bool isFrozen();
    void freeze();
    void unfreeze();

    virtual bool isHero();
    virtual bool isMonster();
    virtual bool isBoss();
    virtual bool isPet();
    virtual bool isMerchant();
    
    virtual void move(int dir) = 0; // pure virtual function
    virtual void useItem(int index);
	virtual void interact(Unit *unit) = 0; //ADD
    virtual void effect(Unit *unit, Item *item, Prop *prop, int skillNumber);
    
    virtual bool equip(Item *itemToEquip);
    virtual void unequip(int bodyPartIDToUnequip);
    
    virtual bool canReceiveItem();
    // pre-condition: canReceiveItem() == true
    virtual void receiveItem(Item *item);
	
    virtual void printSlots();
    virtual void printBackpack();
    
	virtual Item *getSlotItemAt(int index);
	virtual Item *removeSlotItemAt(int index);
    virtual int capacitySlots();

    virtual Item *getBackpackItemAt(int index);
    virtual Item *removeBackpackItemAt(int index);
    virtual int sizeBackpack();
};

#endif
