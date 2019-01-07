#ifndef HERO_H
#define HERO_H

#include "unit.h"
//class Item;
class Inventory;
class Paperdoll;
class SLL;

class Hero : public Unit {
protected:
//    Item *pocket;
    Inventory *slots;
    SLL *backpack;
    
    Paperdoll *paperdoll;

public:
    Hero();
    Hero(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
    void init();
    virtual ~Hero();
    
    virtual void printStat();
    
    virtual bool isHero();
    
    virtual void move(int dir);
    virtual void useItem(int index);
    virtual void interact(Unit *unit);

    virtual void castMagic(int magicNumber);
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
	
	void moveToBackpack(int index);
	void moveToSlots(int indexFromBackpack, int getindexPutSlots);

};

#endif
