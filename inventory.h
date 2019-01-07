#ifndef INVENTORY_H
#define INVENTORY_H

class Item;
class Unit;

class Inventory {
private:
    Item **items;
    int maxNumItems;
    int numItems; // 이것은 앞에서부터의 갯수가 아니라, 중간에 이빨이 빠져 있을 수 있는 갯수이다.
    
public:
    Inventory();
    Inventory(int maxNumItems);
    void init(int maxNumItems);
    
    virtual ~Inventory();
    
    void print();
    void printIndex();

    //--------------------------------------------------------------------------
    // CRUD: Create, Read, Update, Delete
    //--------------------------------------------------------------------------
    
    bool add(Item *item); // 빈곳에 넣는다.
    bool insertItemAt(Item *item, int index); // given index에 넣어야 함.
    
    Item *getItemAt(int index);
    
    Item *removeItemAt(int index); // given index의 item을 return한다. 단 그 자리에 NULL을 넣는다.
    
    void useItemAt(int index, Unit *unit); // given index의 item을 use하고, 만약 그 아이템이 disposable이면 delete한다. 그 아이템이 removable(equippable)이면 delete(heap에서)하지는 않고, inventory에서만 빠진다.
    
    bool isFull();
    
    int size();
    int capacity();
    
    void pack(); // 빈 슬롯이 있다면, 모두 없앤다. 즉 앞으로 땡긴다.
};

#endif

