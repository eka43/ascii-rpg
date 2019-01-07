#ifndef MONSTER_H
#define MONSTER_H

#include "unit.h"

class Monster : public Unit {
protected:
public:
    Monster();
    Monster(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
    virtual ~Monster();
    
    virtual bool isMonster();
    
    virtual void move(int dir) ;
    virtual void interact(Unit *unit);
};

#endif
