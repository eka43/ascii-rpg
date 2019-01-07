#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "inventory.h"

Inventory::Inventory() {
    init(INVENTORY_MAX_NUM_ITEMS);
}

Inventory::Inventory(int maxNumItems) {
    init(maxNumItems);
}

void Inventory::init(int maxNumItems) {
    this->maxNumItems = maxNumItems;
    items = new Item *[maxNumItems];
    numItems = 0;
    
    for (int i = 0; i < maxNumItems; i++) {
        items[i] = NULL;
    }
}

Inventory::~Inventory() {
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] != NULL) {
            delete items[i];
        }
    }
    
    delete [] items;
    items = NULL;
    maxNumItems = 0;
    numItems = 0;
}

void Inventory::print() {
    cout << "[";
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] != NULL) {
            items[i]->print();
        }
        else {
            cout << " ";
        }
        
        if (i < maxNumItems - 1) {
            cout << "|";
        }
    }
    cout << "]";
}

void Inventory::printIndex() {
    cout << " ";
    for (int i = 0; i < capacity(); i++) {
        cout << i << " ";
    }
}

bool Inventory::add(Item *item) { // 빈곳에 넣는다.
    for (int i = 0; i < maxNumItems; i++) {
        if (items[i] == NULL) {
            items[i] = item;
            numItems++;
            return true;
        }
    }
    
    return false;
}

bool Inventory::insertItemAt(Item *item, int index) { // given index에 넣어야 함.
    if (index < 0 || index >= maxNumItems) {
        cout << "Inventory::insertItemAt(): error - index out of bound: " << index << endl;
        exit(1);
    }
    
    if (items[index] == NULL) {
        items[index] = item;
        numItems++;
        return true;
    }
    
    return false;
}

Item *Inventory::getItemAt(int index) {
    if (index < 0 || index >= maxNumItems) {
        cout << "Inventory::getItemAt(): error - index out of bound: " << index << endl;
        exit(1);
    }
    
    return items[index];
}

Item *Inventory::removeItemAt(int index) { // given index의 item을 return한다. 단 그 자리에 NULL을 넣는다.
    if (index < 0 || index >= maxNumItems) {
        cout << "Inventory::removeItemAt(): error - index out of bound: " << index << endl;
        exit(1);
    }
    
    if (items[index] != NULL) {
        Item *itemToReturn = items[index];
        items[index] = NULL;
        numItems--;
        return itemToReturn;
    }
    
    return NULL;
}

void Inventory::useItemAt(int index, Unit *unit) { // given index의 item을 use하고, 만약 그 아이템이 disposable이면 delete한다. 그 아이템이 removable(equippable)이면 delete(heap에서)하지는 않고, inventory에서만 빠진다.
    if (index < 0 || index >= maxNumItems) {
        cout << "Inventory::useItemAt(): error - index out of bound: " << index << endl;
        exit(1);
    }
    
    if (items[index] != NULL) {
        if (items[index]->use(unit)) {
            if (!items[index]->isRemovable()) {
                delete items[index];
            }
            items[index] = NULL;
            numItems--;
        }
    }
}

bool Inventory::isFull() {
    return numItems >= maxNumItems;
}

int Inventory::size() {
    return numItems;
}

int Inventory::capacity() {
    return maxNumItems;
}

void Inventory::pack() { // 빈 슬롯이 있다면, 모두 없앤다. 즉 앞으로 땡긴다.
    for (int i = 0; i < maxNumItems-1; i++) {
        if (items[i] == NULL) {
            for (int j = i; j < maxNumItems-1; j++) {
                items[j] = items[j+1];
            }
        }
    }
}
