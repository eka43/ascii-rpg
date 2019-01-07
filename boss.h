#ifndef BOSS_H
#define BOSS_H

#include "monster.h"

#define ROW 0
#define COL 1

class Tile;

class Boss : public Monster {
protected:
    int range; // range to search the hero
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // Member Variables for IQ120/IQ150
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    int goalRow;
    int goalCol;
    int originalDir;

    int path[2][BOSS_IQ120_MAX_LEN_MEMORY];

    int pathDir[BOSS_IQ120_MAX_LEN_MEMORY];

    int pathLen;
    int pathFound;
    
    int curIndexPath;
    
    //--------------------------------------------------------------------------
    // Helper Functions for IQ120
    //--------------------------------------------------------------------------
    bool canMove(int dir);
	
    //--------------------------------------------------------------------------
    // Helper Functions for IQ150
    //--------------------------------------------------------------------------
    double calcHeuristic(int row, int col, const int goalRow, const int goalCol);
    void reconstruct_path(Tile *start, Tile *goal);
    Tile *getNeighbour(Tile *curTile, int neighbourDir);
    int getDir(Tile *source, Tile *target);

public:
    Boss();
    Boss(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp);
    void resetPath();
    virtual ~Boss();
    
    virtual bool isBoss();
    
    virtual void move(int dir) ;
    virtual void interact(Unit *unit);
	
	friend class Iq150Boss;
};

#endif
