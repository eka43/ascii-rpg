#ifndef PET_H
#define PET_H

#include "unit.h"

class Pet : public Unit {
protected:
	int findBossRange;
	int stayRange;
    bool bossFound;
    bool adjacentBoss;
public:
	Pet();
	Pet(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
	virtual ~Pet();
	
	virtual bool isPet();
	
	void bodyguard();
	
	virtual void move(int dir);
    virtual void interact(Unit *unit);
};

#endif
