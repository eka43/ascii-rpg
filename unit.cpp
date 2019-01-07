#include <iostream>
#include <stdlib.h> 
using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "pet.h"
#include "board.h"

//------------------------------------------------------------------------------
// class Unit
//------------------------------------------------------------------------------

Unit::Unit() {
    init('U', -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Unit::Unit(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp) {
    init(shape, row, col, dir, hp, maxHp, mp, maxMp, atk, def, gold, exp);
}

void Unit::init(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp) {
    this->shape = shape;
    
    this->row = row;
    this->col = col;

    this->dir = dir;

    this->hp = hp;
    this->maxHp = maxHp;
    this->mp = mp;
    this->maxMp = maxMp;
    
    this->atk = atk;
    this->def = def;
    this->gold = gold;
    this->exp = exp;
    
    board = NULL;
    
    died = false;
    frozen = false;
}

Unit::~Unit() {
}

void Unit::print() {
    cout << shape;
}

void Unit::printStat() {
    cout << "[" << shape << "]: HP(" << hp << "/" << maxHp << ") MP(" << mp << "/" << maxMp << ") ATK(" << atk << ") DEF(" << def << ") GOLD(" << gold << ") EXP(" << exp << ")";
}

void Unit::setBoard(Board *board) {
    this->board = board;
}

int Unit::getRow() {
    return row;
}

void Unit::setRow(int row) {
    this->row = row;
}

int Unit::getCol() {
    return col;
}

void Unit::setCol(int col) {
    this->col = col;
}

int Unit::getDir() {
    return dir;
}

char Unit::getShape() {
    return shape;
}

bool Unit::incHp(int hpInc) {
    if (hp == maxHp) {
        return false;
    }
    
    if (hp + hpInc >= maxHp) {
        hp = maxHp;
    }
    else {
        hp += hpInc;
    }
    
    return true;
}

void Unit::decHp(int hpDec) {
    if (hp - hpDec <= 0) {
        hp = 0;
        died = true;
    }
    else {
        hp -= hpDec;
    }
}

int Unit::getMp() {
    return mp;
}

bool Unit::incMp(int mpInc) {
    if (mp == maxMp) {
        return false;
    }
    
    if (mp + mpInc >= maxMp) {
        mp = maxMp;
    }
    else {
        mp += mpInc;
    }
    
    return true;
}

void Unit::decMp(int mpDec) {
    if (mp - mpDec <= 0) {
        mp = 0;
    }
    else {
        mp -= mpDec;
    }
}

// 이 rpg에서는 오직 weapon과 armor로만 atk와 def를 더하고 빼므로, 0 아래로 떨어질 일은 없다. 또한 max는 inf이다.

int Unit::getAtk() {
    return atk;
}

void Unit::incAtk(int atkInc) {
    atk += atkInc;
}

void Unit::decAtk(int atkDec) {
    atk -= atkDec;
}

int Unit::getDef() {
    return def;
}

void Unit::incDef(int defInc) {
    def += defInc;
}

void Unit::decDef(int defDec) {
    def -= defDec;
}

int Unit::getGold() {
    return gold;
}

void Unit::incGold(int gold) {
    this->gold += gold;
}

bool Unit::decGold(int gold) {
    if (this->gold < gold) {
        return false;
    }
    this->gold -= gold;
    return true;
}

int Unit::getExp() {
    return exp;
}

void Unit::incExp(int exp) {
    this->exp += exp;
}

void Unit::decExp(int exp) {
    this->exp -= exp;
}

bool Unit::isDead() {
	return died;
}

bool Unit::isFrozen() {
    return frozen;
}

void Unit::freeze() {
    frozen = true;
}

void Unit::unfreeze() {
    frozen = false;
}

bool Unit::isHero() { // fake body
    return false;
}

bool Unit::isMonster() { // fake body
    return false;
}

bool Unit::isBoss() { // fake body
    return false;
}

bool Unit::isPet() { // fake body
    return false;
}

bool Unit::isMerchant() { // fake body
    return false;
}

void Unit::useItem(int index) { // fake body
}

void Unit::effect(Unit *unit, Item *item, Prop *prop, int skillNumber) { // fake body
}

bool Unit::equip(Item *itemToEquip) { // fake body
    return false;
}

void Unit::unequip(int bodyPartIDToUnequip) { // fake body
}

bool Unit::canReceiveItem() { // fake body
    return false;
}

// pre-condition: canReceiveItem() == true
void Unit::receiveItem(Item *item) { // fake body
}

void Unit::printSlots() {
}

void Unit::printBackpack() {
}

Item *Unit::getSlotItemAt(int index) {
    return NULL;
}

Item *Unit::removeSlotItemAt(int index) {
    return NULL;
}

int Unit::capacitySlots() {
    return 0;
}

Item *Unit::getBackpackItemAt(int index) {
    return NULL;
}

Item *Unit::removeBackpackItemAt(int index) {
    return NULL;
}

int Unit::sizeBackpack() {
    return 0;
}
