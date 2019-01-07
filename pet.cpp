#include <iostream>
#include <stdlib.h>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "pet.h"
#include "monster.h"
#include "boss.h"
#include "board.h"
#include "inventory.h"

Pet::Pet() : Unit(PET_SHAPE, PET_ROW, PET_COL, PET_DIR, PET_HP, PET_MAX_HP, PET_MP, PET_MAX_MP, PET_ATK, PET_DEF, PET_GOLD, PET_EXP) {
	findBossRange = PET_FIND_RANGE;
	stayRange = PET_RANGE;
    
    bossFound = false;
    adjacentBoss = false;
}

Pet::Pet(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp)
    : Unit(shape, row, col, dir, hp, maxHp, mp, maxMp, atk, def, gold, exp) {
	findBossRange = PET_FIND_RANGE;
	stayRange = PET_RANGE;
    
    bossFound = false;
    adjacentBoss = false;
}

Pet::~Pet() {
}

bool Pet::isPet() {
    return true;
}

void Pet::bodyguard() {
	int bossRow = -1;
    int bossCol = -1;
    
    Boss *boss = NULL;
    bossFound = false;
    
    for (int i = row - findBossRange; i < row + findBossRange && !bossFound; i++) {
        for (int j = col - findBossRange; j < col + findBossRange && !bossFound; j++) {
            if (board->validate(i, j) &&
                board->getUnit(i, j) != NULL &&
                board->getUnit(i, j)->isBoss()) {
                bossRow = i;
                bossCol = j;
                boss = (Boss *)board->getUnit(i, j); // 슬픈이야기 3번 case의 2번 방식.
                bossFound = true;
            }
        }
    }
    
    if (bossFound) { // found!!
        // 그 방향을 찾아서 그 방향을 dir에 set해주면 되고, 만약 붙어 있으면 한대 때린다.
        // 일단 같은 수평선상에 있는지 보자.
        if (row == boss->getRow()) {
            if (col > boss->getCol()) { // left side에 있다.
                if (col == boss->getCol()+1) { // adjacent?
                    boss->decHp(atk);
                    adjacentBoss = true;
                    return;
                }
                else {
                    dir = DIR_W;
                }
            }
            else { // right side에 있다.
                if (col == boss->getCol()-1) { // adjacent?
                    boss->decHp(atk);
                    adjacentBoss = true;
                    return;
                }
                else {
                    dir = DIR_E;
                }
            }
        }
        // 일단 같은 수직선상에 있는지 보자.
        else if (col == boss->getCol()) {
            if (row > boss->getRow()) { // up side에 있다.
                if (row == boss->getRow()+1) { // adjacent?
                    boss->decHp(atk);
                    adjacentBoss = true;
                    return;
                }
                else {
                    dir = DIR_N;
                }
            }
            else { // down side에 있다.
                if (row == boss->getRow()-1) { // adjacent?
                    boss->decHp(atk);
                    adjacentBoss = true;
                    return;
                }
                else {
                    dir = DIR_S;
                }
            }
        }
        // 그 외는 4개의 대각선 블럭에 boss가 들어 있다.
        else {
            int rowDiff = abs(row - boss->getRow());
            int colDiff = abs(col - boss->getCol());
            
            if (row > boss->getRow() && col > boss->getCol()) { // top-left block
                if (rowDiff == colDiff) {
                    dir = (rand() % 2 == 0)?DIR_N:DIR_W;
                }
                else if (rowDiff > colDiff) {
                    dir = DIR_N;
                }
                else {
                    dir = DIR_W;
                }
            }
            else if (row > boss->getRow() && col < boss->getCol()) { // top-right block
                if (rowDiff == colDiff) {
                    dir = (rand() % 2 == 0)?DIR_N:DIR_E;
                }
                else if (rowDiff > colDiff) {
                    dir = DIR_N;
                }
                else {
                    dir = DIR_E;
                }
            }
            else if (row < boss->getRow() && col < boss->getCol()) { // bottom-right block
                if (rowDiff == colDiff) {
                    dir = (rand() % 2 == 0)?DIR_S:DIR_E;
                }
                else if (rowDiff > colDiff) {
                    dir = DIR_S;
                }
                else {
                    dir = DIR_E;
                }
            }
            else { // if (row < boss->getRow() && col > boss->getCol()) { // bottom-left block
                if (rowDiff == colDiff) {
                    dir = (rand() % 2 == 0)?DIR_S:DIR_W;
                }
                else if (rowDiff > colDiff) {
                    dir = DIR_S;
                }
                else {
                    dir = DIR_W;
                }
            }
        }
    }
}

void Pet::move(int dir) {	
	if (died || frozen) {
        return;
    }
    
    this->dir = dir;
    
    bodyguard();
    
    if (adjacentBoss) {
        adjacentBoss = false;
        return;
    }
    
    bool moved = false;
    
    int prevRow = row;
    int prevCol = col;
    
    int nextRow = row;
    int nextCol = col;
    
    if (!bossFound) {
        Hero *hero = NULL;
        hero = (Hero *)board->getHero();

		//nextRow = hero->getRow() + (rand() % (2*stayRange+1) + (-stayRange));
		//nextCol = hero->getCol() + (rand() % (2*stayRange+1) + (-stayRange));
		nextRow = hero->getRow() -1;
		nextCol = hero->getCol() + (rand() % (2*stayRange+1) + (-stayRange));
    }
    else {
        if (this->dir == DIR_N) {
            nextRow--;
        }
        else if (this->dir == DIR_E) {
            nextCol++;
        }
        else if (this->dir == DIR_S) {
            nextRow++;
        }
        else if (this->dir == DIR_W) {
            nextCol--;
        }
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

void Pet::interact(Unit *unit) {
    if (unit == NULL) {
        cout << "Pet::interact(): error: unit == NULL" << endl;
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
