#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "pet.h"
#include "board.h"
#include "inventory.h"
#include "sll.h"
#include "paperdoll.h"

Hero::Hero()
    : Unit(HERO_SHAPE, HERO_ROW, HERO_COL, HERO_DIR, HERO_HP, HERO_MAX_HP,
           HERO_MP, HERO_MAX_MP, HERO_ATK, HERO_DEF, HERO_GOLD, HERO_EXP) {
    init();
}


Hero::Hero(char shape, int row, int col, int hp, int maxHp, int mp, int maxMp,
           int atk, int def, int gold, int exp)
    : Unit(shape, row, col, dir, hp, maxHp, mp, maxMp, atk, def, gold, exp) {
    init();
}

void Hero::init() {
    slots = new Inventory();
    backpack = new SLL();

    paperdoll = new Paperdoll();
}

Hero::~Hero() {
    if (slots != NULL) {
        delete slots;
        slots = NULL;
    }
    
    if (paperdoll != NULL) {
        delete paperdoll;
        paperdoll = NULL;
    }
}

void Hero::printStat() {
    Unit::printStat();
    cout << endl;
    
    cout << "Slots     : ";
    slots->print();
    cout << endl;
    cout << "            ";
    slots->printIndex();
    cout << endl;

    cout << "Backpack  : ";
    backpack->print();
    cout << endl;
    cout << "            ";
    backpack->printIndex();
    cout << endl;

    cout << "Paperdoll : ";
    paperdoll->print();
    cout << endl;
}

bool Hero::isHero() {
    return true;
}

void Hero::move(int dir) {
    if (died || frozen) {
        return;
    }
    
    this->dir = dir;
    
    bool moved = false;
    
    int prevRow = row;
    int prevCol = col;
    
    int nextRow = row;
    int nextCol = col;
    
    if (dir == DIR_N) {
        nextRow--;
    }
    else if (dir == DIR_E) {
        nextCol++;
    }
    else if (dir == DIR_S) {
        nextRow++;
    }
    else if (dir == DIR_W) {
        nextCol--;
    }
    
    if (board->validate(nextRow, nextCol) &&
        (board->getProp(nextRow, nextCol) == NULL ||
         board->getProp(nextRow, nextCol)->isClimbable()) &&
        board->getUnit(nextRow, nextCol) == NULL) {
        board->setUnit(prevRow, prevCol, NULL);
        board->setUnit(nextRow, nextCol, this);
        moved = true;
    }
    
    if (moved) {
        if (board->getItem(nextRow, nextCol) != NULL) {
            if (!slots->isFull()) {
                slots->add(board->getItem(nextRow, nextCol));
            }
            else {
                backpack->add(board->getItem(nextRow, nextCol));
            }
            board->setItem(nextRow, nextCol, NULL);
        }
    }
    
    if (moved) {
        // 움직이기 전 주변의 monster들을 모두 unfreeze한다.
        for (int i = prevRow-1; i <= prevRow+1; i++) {
            for (int j = prevCol-1; j <= prevCol+1; j++) {
                if (board->validate(i, j) &&
                    board->getUnit(i, j) != NULL &&
                    board->getUnit(i, j)->isMonster()) {
                    board->getUnit(i, j)->unfreeze();
                }
            }
        }
        
        // 새로 움직인 자리 주변의 monster들을 모두 freeze한다.
        for (int i = nextRow-1; i <= nextRow+1; i++) {
            for (int j = nextCol-1; j <= nextCol+1; j++) {
                if (board->validate(i, j) &&
                    board->getUnit(i, j) != NULL &&
                    board->getUnit(i, j)->isMonster()) {
                    board->getUnit(i, j)->freeze();
                }
            }
        }
    }
    
    // 만약 못 움직였을 경우, 그 자리에 unit이 있다면, 그것이 merchant들 monster, boss, pet이든,
    // 그 unit의 interact를 call헤봐야 한다.
    if (!moved) {
        if (board->validate(nextRow, nextCol) &&
            board->getUnit(nextRow, nextCol) != NULL) {
            board->getUnit(nextRow, nextCol)->interact(this);
        }
    }
    
    // trigger prop
    if (board->validate(nextRow, nextCol) &&
        board->getProp(nextRow, nextCol) != NULL) {
        board->getProp(nextRow, nextCol)->trigger(this);
    }
}

void Hero::useItem(int index) {
    slots->useItemAt(index, this);
}

void Hero::interact(Unit *unit) {
}

void Hero::castMagic(int magicNumber) {
    if (magicNumber == MAGIC_NULLIFY) {
        board->startWave(this, NULL, NULL,
                         row, col,
                         MAGIC_NULLIFY_RANGE, // range
                         MAGIC_NULLIFY_DELAY, // delay
                         MAGIC_NULLIFY_SHAPE, // shape
                         MAGIC_NULLIFY); // skill number
    }
}

void Hero::effect(Unit *unit, Item *item, Prop *prop, int skillNumber) {
    if (skillNumber == MAGIC_NULLIFY) {
        if (unit != NULL && unit->isMonster()) {
            unit->decHp(2147483647);
        }
    }
}

bool Hero::equip(Item *itemToEquip) {
    if (paperdoll->equip(this, itemToEquip)) {
        return true;
    }
    else {
        return false;
    }
}

void Hero::unequip(int bodyPartIDToUnequip) {
	slots->add(paperdoll->unequip(this, bodyPartIDToUnequip));
}

bool Hero::canReceiveItem() {
    return true;
}

// pre-condition: canReceiveItem() == true
void Hero::receiveItem(Item *item) {
    if (!slots->isFull()) {
        slots->add(item);
    }
    else {
        backpack->add(item);
    }
}

void Hero::printSlots() {
    for (int i = 0; i < slots->capacity(); i++) {
        cout << setw(3) << i << ": ";
        if (slots->getItemAt(i) != NULL) {
            slots->getItemAt(i)->printFullSpec();
        }
        else {
            cout << "[EMPTY]";
        }
        cout << endl;
    }
}
    
void Hero::printBackpack() {
    for (int i = 0; i < backpack->size(); i++) {
        cout << setw(3) << (i+100) << ": ";
        backpack->getItemAt(i)->printFullSpec();
        cout << endl;
    }
}

Item *Hero::getSlotItemAt(int index) {
    if (index < 0 || index >= slots->capacity()) {
        return NULL;
    }
    
    return slots->getItemAt(index);
}

Item *Hero::removeSlotItemAt(int index) {
    if (index < 0 || index >= slots->capacity()) {
        return NULL;
    }
    
    return slots->removeItemAt(index);
}

int Hero::capacitySlots() {
    return slots->capacity();
}

Item *Hero::getBackpackItemAt(int index) {
    if (index < 0 || index >= backpack->size()) {
        return NULL;
    }
    
    return backpack->getItemAt(index);
}

Item *Hero::removeBackpackItemAt(int index) {
    if (index < 0 || index >= backpack->size()) {
        return NULL;
    }
    
    return backpack->removeItemAt(index);
}

int Hero::sizeBackpack() {
    return backpack->size();
}

void Hero::moveToBackpack(int index) {
	backpack->add(slots->getItemAt(index));
	slots->removeItemAt(index);
}

void Hero::moveToSlots(int indexFromBackpack, int getindexPutSlots) {
	slots->insertItemAt(backpack->getItemAt(indexFromBackpack), getindexPutSlots);
	backpack->removeItemAt(indexFromBackpack);
}


