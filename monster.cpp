#include <iostream>

using namespace std;

#include "main.h"
#include "unit.h"
#include "monster.h"
#include "pet.h"
#include "board.h"

//------------------------------------------------------------------------------
// class Monster
//------------------------------------------------------------------------------

Monster::Monster() : Unit(MONSTER_SHAPE, MONSTER_ROW, MONSTER_COL, MONSTER_DIR, MONSTER_HP, MONSTER_MAX_HP, MONSTER_MP, MONSTER_MAX_MP, MONSTER_ATK, MONSTER_DEF, MONSTER_GOLD, MONSTER_EXP) {
}

Monster::Monster(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp)
    : Unit(shape, row, col, dir, hp, maxHp, mp, maxMp, atk, def, gold, exp) {
}

Monster::~Monster() {
}

void Monster::move(int dir) {
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
    
    if (nextRow >= 0 && nextRow < board->getRowSize() &&
        nextCol >= 0 && nextCol < board->getColSize() &&
        board->getUnit(nextRow, nextCol) == NULL &&
        board->getProp(nextRow, nextCol) == NULL) {
        board->setUnit(prevRow, prevCol, NULL);
        board->setUnit(nextRow, nextCol, this);
        moved = true;
    }
}

bool Monster::isMonster() {
    return true;
}

//battle
void Monster::interact(Unit *unit) {
    if (unit == NULL) {
        cout << "Monster::interact(): error: unit == NULL" << endl;
        exit(1);
    }
    
    if (isDead()) {
        return;
    }
    
    decHp(unit->getAtk()); //ADD hero hits mon
    
    if (!isDead()) {
        unit->decHp(atk);
    }
    else {
        unit->incGold(gold);
        unit->incExp(exp);
        decGold(gold);
        decGold(exp);
    }
}
