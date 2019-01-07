#include <iostream>
#include <stdlib.h>

using namespace std;

#include "main.h"
#include "prop.h"
#include "unit.h"
#include "board.h"

//------------------------------------------------------------------------------
// class Prop
//------------------------------------------------------------------------------

Prop::Prop() {
    shape = 'P';
    
    board = NULL;
}

Prop::Prop(char shape) {
    this->shape = shape;

    board = NULL;
}

Prop::~Prop() {
}

void Prop::print() {
    cout << shape;
}

void Prop::setBoard(Board *board) {
    this->board = board;
}

int Prop::getRow() {
    return row;
}

void Prop::setRow(int row) {
    this->row = row;
}

int Prop::getCol() {
    return col;
}

void Prop::setCol(int col) {
    this->col = col;
}

bool Prop::isClimbable() {
    return false;
}

bool Prop::isTree() {
    return false;
}

bool Prop::isPortal() {
    return false;
}

void Prop::trigger(Unit *unit) {
}

//------------------------------------------------------------------------------
// class Tree
//------------------------------------------------------------------------------

Tree::Tree() : Prop(TREE_SHAPE) {
}

Tree::~Tree() {
}

bool Tree::isTree() {
    return true;
}

//------------------------------------------------------------------------------
// class Portal
//------------------------------------------------------------------------------

Portal::Portal() : Prop(PORTAL_SHAPE) {
}

Portal::~Portal() {
}

bool Portal::isClimbable() {
    return true;
}

bool Portal::isPortal() {
    return true;
}

void Portal::trigger(Unit *unit) {
    if (unit == NULL) {
        cout << "Portal::trigger(): error - unit is NULL!" << endl;
        exit(1);
    }

    while (true) {
        Portal* destRandPortal = board->getRandomPortal();
        int destRandPortalRow = destRandPortal->getRow();
        int destRandPortalCol = destRandPortal->getCol();

        if (board->validate(destRandPortalRow, destRandPortalCol) &&
            board->getProp(destRandPortalRow, destRandPortalCol) != NULL &&
            board->getItem(destRandPortalRow, destRandPortalCol) == NULL &&
            board->getUnit(destRandPortalRow, destRandPortalCol) == NULL) {
            board->setUnit(unit->getRow(), unit->getCol(), NULL);
            board->setUnit(destRandPortalRow, destRandPortalCol, unit);
            board->startWave(NULL, NULL, NULL,
                             unit->getRow(), unit->getCol(),
                             5, // range
                             1, // delay
                             '@', // shape
                             -1); // skill number
            break;
        }
    }
}
