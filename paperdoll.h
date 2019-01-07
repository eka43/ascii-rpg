#ifndef PAPERDOLL_H
#define PAPERDOLL_H

class Unit;
class Item;
class Armor;
class Weapon;

class Paperdoll {
private:
    Armor *head;
    Armor *back;
    Armor *chest;
    Armor *hands;
    Armor *legs;
    Armor *feet;
    
    Weapon *leftHand;
    Weapon *rightHand;

public:
    Paperdoll();
    virtual ~Paperdoll();
    
    void print();
    
    bool equip(Unit *unit, Item *itemToEquip);
    Item *unequip(Unit *unit, int bodyPartID);
};

#endif
