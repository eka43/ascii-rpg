#include <iostream>
#include <stdlib.h> 
#include <math.h>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "hero.h"
#include "pet.h"
#include "monster.h"
#include "boss.h"
#include "tile.h"
#include "board.h"

#include "tile_item.h"
#include "set.h"
#include "pqueue.h"

//------------------------------------------------------------------------------
// class Boss
//------------------------------------------------------------------------------

Boss::Boss() : Monster(BOSS_SHAPE, BOSS_ROW, BOSS_COL, BOSS_DIR, BOSS_HP, BOSS_MAX_HP, BOSS_MP, BOSS_MAX_MP, BOSS_ATK, BOSS_DEF, BOSS_GOLD, BOSS_EXP) {
    range = BOSS_RANGE;
    resetPath();
}

Boss::Boss(char shape, int row, int col, int dir, int hp, int maxHp, int mp, int maxMp, int atk, int def, int gold, int exp)
    : Monster(shape, row, col, dir, hp, maxHp, mp, maxMp, atk, def, gold, exp) {
    range = BOSS_RANGE;
    resetPath();
}

void Boss::resetPath() {
    pathLen = 0;
    pathFound = false;
    
    curIndexPath = -1;
}

Boss::~Boss() {
}

void Boss::move(int dir) {
    if (died || frozen) {
        return;
    }
    
//==============================================================================
//==============================================================================
//==============================================================================
// IQ120이나 IQ150에 의해 따라갈 path가 있다면.. 처리
//==============================================================================
//==============================================================================
//==============================================================================
    if (pathFound) {
        int curRow = path[ROW][curIndexPath];
        int curCol = path[COL][curIndexPath];
        
        // Unit이 가로막고 있지 않다면..
        if (board->getUnit(curRow, curCol) == NULL) {
            board->setUnit(row, col, NULL);
            board->setUnit(curRow, curCol, this);
            
            // 도착?
            if (curRow == goalRow && curCol == goalCol) {
                dir = originalDir;
                resetPath();
            }
            // 도착하지 않았다면..
            else {
                // 다음 위치를 미리 index로 set.
                curIndexPath++;
                if (curIndexPath >= pathLen) {
                    resetPath();
                }
            }
            
            return;
        }
        // Unit이 막고 있다면.. 기다린다.
        else {
            if (board->getUnit(curRow, curCol) != NULL &&
                board->getUnit(curRow, curCol)->isHero()) {
                resetPath();
                // 이제 다음번 move()가 실행될 때, 아래의 IQ100코드에서 붙어있으므로 한대 때리게 된다.
            }
            // 만약 몬스터등이었다면, 기다렸다가 비켜나면 가던길을 계속 간다.
            // Issue: 그 몬스터들이 stuck되었거나, 어떤 unit이 다른 방향으로 가고 있었다면..
            //        서로 기다리게 되므로, dead lock이 걸린다. 즉 영원히 둘이 기다리게 된다.
            return;
        }
    }
    
//==============================================================================
//==============================================================================
//==============================================================================
#ifdef BOSS_IQ_100
//==============================================================================
//==============================================================================
//==============================================================================
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // IQ100 앨거리듬:
    //     hero를 찾아서, 그 방향으로 이동한다.
    //     range를 주고, 그 range안에 hero를 찾아서 포인터를 얻어낸다.
    //     그 방향을 찾아서 그 방향을 dir에 set해주면..
    //     붙어 있으면 한대 때린다.
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // hero를 찾아서, 그 방향으로 이동한다.
    // range를 주고, 그 range안에 hero를 찾아서 포인터를 얻어낸다.
    int heroRow = -1;
    int heroCol = -1;
    Hero *hero = NULL;
    
    for (int i = row - range; i < row + range && hero == NULL; i++) {
        for (int j = col - range; j < col + range && hero == NULL; j++) {
            if (board->validate(i, j) &&
                board->getUnit(i, j) != NULL &&
                board->getUnit(i, j)->isHero()) {
                heroRow = i;
                heroCol = j;
                hero = (Hero *)board->getUnit(i, j); // 슬픈이야기 3번 case의 2번 방식.
            }
        }
    }
    
    // hero를 찾았다면..
    if (hero != NULL) { // found!!
        // 그 방향을 찾아서 그 방향을 dir에 set해주면 되고, 만약 붙어 있으면 한대 때린다.
        
        // 일단 같은 수평선상에 있는지 보자.
        if (row == hero->getRow()) {
            if (col > hero->getCol()) { // left side에 있다.
                if (col == hero->getCol()+1) { // adjacent?
                    hero->decHp(atk);
                    return;
                }
                else {
                    dir = DIR_W;
                }
            }
            else { // right side에 있다.
                if (col == hero->getCol()-1) { // adjacent?
                    hero->decHp(atk);
                    return;
                }
                else {
                    dir = DIR_E;
                }
            }
        }
        // 일단 같은 수직선상에 있는지 보자.
        else if (col == hero->getCol()) {
            if (row > hero->getRow()) { // up side에 있다.
                if (row == hero->getRow()+1) { // adjacent?
                    hero->decHp(atk);
                    return;
                }
                else {
                    dir = DIR_N;
                }
            }
            else { // down side에 있다.
                if (row == hero->getRow()-1) { // adjacent?
                    hero->decHp(atk);
                    return;
                }
                else {
                    dir = DIR_S;
                }
            }
        }
        // 그 외는 4개의 대각선 블럭에 hero가 들어 있다.
        else {
            int rowDiff = abs(row - hero->getRow());
            int colDiff = abs(col - hero->getCol());
            
            if (row > hero->getRow() && col > hero->getCol()) { // top-left block
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
            else if (row > hero->getRow() && col < hero->getCol()) { // top-right block
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
            else if (row < hero->getRow() && col < hero->getCol()) { // bottom-right block
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
            else { // if (row < hero->getRow() && col > hero->getCol()) { // bottom-left block
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
//==============================================================================
//==============================================================================
//==============================================================================
#endif // BOSS_IQ_100
//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
//==============================================================================
#ifdef BOSS_IQ_120
//==============================================================================
//==============================================================================
//==============================================================================
    // 위에서 heroFound가 true인데, 결과 dir으로 가려고 했더니 막혔다면..
    // 아래의 IQ120이 가동해야 된다는 말이다.
    if (hero != NULL && !canMove(dir)) {
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------
        // IQ120 앨거리듬: Wall Follow Algorithm
        //     위에서 IQ100을 가동해서, hero를 찾았는데, 그 방향이 막혔다면, IQ120을 가동한다.
        // Algorithm:
        //     1. 현재 방향으로 전진하려고 해본다.
        //          1-a. 안 막혔다면..
        //              전진한다.
        //                  Goal에 도달했다면.. 2로 간다.
        //              다시 오른쪽으로 돈다.
        //          1-b. 막혔다면..
        //              왼쪽으로 돈다.
        //              다시 1번으로 간다.
        //     2. 끝.
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------

        //----------------------------------------------------------------------
        // 앨거리듬의 implementation: 준비 과정이 어느 정도 필요하다.
        //----------------------------------------------------------------------
        originalDir = dir;
        
        //----------------------------------------------------------------------
        // 아래에서 wall을 따라갈 때, 언제까지 가야할 지 결정하기 위해, goal을 찾아야 한다.
        // Goal찾기 앨거리듬:
        //     현재 방향으로 막히지 않은 곳이 나올때까지 간다.
        //     그 막히지 않은 곳이 goal이다.
        //----------------------------------------------------------------------
        goalRow = row;
        goalCol = col;
        
        while (true) {
            if (dir == DIR_N) {
                goalRow--;
            }
            else if (dir == DIR_E) {
                goalCol++;
            }
            else if (dir == DIR_S) {
                goalRow++;
            }
            else if (dir == DIR_W) {
                goalCol--;
            }
            
            if (goalRow >= 0 && goalRow < board->getRowSize() &&
                goalCol >= 0 && goalCol < board->getColSize() &&
                board->getUnit(goalRow, goalCol) == NULL &&
                board->getProp(goalRow, goalCol) == NULL) {
                break;
            }
        }
        
//        cout << "Hero::move()::IQ120: goalRow = " << goalRow << " goalCol = " << goalCol << endl;
//        exit(1);
        
        //----------------------------------------------------------------------
        // 아래에서 본격적으로 앨거리듬을 돌릴때 필요한 임시 로컬 변수들과 member variable들을..
        // 최대한 준비하고 시작해본다.
        //----------------------------------------------------------------------
//        #define BOSS_IQ120_MAX_NUM_ITERATION 300
        int numIteration = 0;
        
        pathLen = 0;
        
        int curRow = row;
        int curCol = col;
        int curDir = dir;
        
        while (true) {
//            cout << "Hero::move()::IQ120: while (true) {" << endl;
            int nextRow = curRow;
            int nextCol = curCol;

            // 1. 현재 방향으로 전진하려고 해본다.
            if (curDir == DIR_N) {
                nextRow--;
            }
            else if (curDir == DIR_E) {
                nextCol++;
            }
            else if (curDir == DIR_S) {
                nextRow++;
            }
            else if (curDir == DIR_W) {
                nextCol--;
            }
            
            // 1-a. 안 막혔다면..
            if (nextRow >= 0 && nextRow < board->getRowSize() &&
                nextCol >= 0 && nextCol < board->getColSize() &&
                board->getProp(nextRow, nextCol) == NULL) { // 여기에서 unit은 장애물로 간주하지 않는다.
                //--------------------------------------------------------------
                // 전진한다.
                //--------------------------------------------------------------
                curRow = nextRow;
                curCol = nextCol;
                
                // 현재 위치를 떠나기 전에 path에 기록해둔다.
                path[ROW][pathLen] = curRow;
                path[COL][pathLen] = curCol;
                pathDir[pathLen] = curDir;
                pathLen++;
                
                // Goal에 도달했다면.. 2로 간다.
                if (curRow == goalRow && curCol == goalCol) {
                    pathFound = true;
                    curIndexPath = 0;
                    break;
                }
                
                // 다시 오른쪽으로 돈다.
                curDir = (curDir + 1) % NUM_DIRS;
            }
            // 1-b. 막혔다면..
            else {
                // 왼쪽으로 돈다.
                curDir = (curDir - 1 + 4) % NUM_DIRS; // 왼쪽으로 돌때는, 1을 빼면 되지만, DIR_N이었을 경우, 0에서 -1을 하면, -1이 되기 때문에, + 4를 한 후, 반드시 NUM_DIRS로 modulus해야 한다.

                // 다시 1번으로 간다. -> 어차피 loop을 돌면 1번이다.
            }
            
            numIteration++;
            if (numIteration >= BOSS_IQ120_MAX_NUM_ITERATION) {
                resetPath(); // 너무 오래 걸리면 없던 걸로..
                break;
            }
        }
        // 2. 끝.
    }
//==============================================================================
//==============================================================================
//==============================================================================
#endif // BOSS_IQ_120
//==============================================================================
//==============================================================================
//==============================================================================
    
//==============================================================================
//==============================================================================
//==============================================================================
#ifdef BOSS_IQ_150
//==============================================================================
//==============================================================================
//==============================================================================
//function A*(start, goal)
    Tile *start = board->getTile(row, col);
    Tile *goal = board->getTile(heroRow, heroCol);
//    cout << "Boss::move()::IQ150: start = " << TileItem(start) << endl;
//    cout << "Boss::move()::IQ150: goal  = " << TileItem(goal) << endl;

//    // The set of nodes already evaluated
//    closedSet := {}
    Set<TileItem> closedSet;
//
//    // The set of currently discovered nodes that are not evaluated yet.
//    // Initially, only the start node is known.
//    openSet := {start}
    PQueue<TileItem> openSet;
    TileItem startTileItem(start);
    openSet.enqueue(startTileItem);
//
//    // For each node, which node it can most efficiently be reached from.
//    // If a node can be reached from many nodes, cameFrom will eventually contain the
//    // most efficient previous step.
//    cameFrom := an empty map
//
//    // For each node, the cost of getting from the start node to that node.
//    gScore := map with default value of Infinity
    for (int i = 0; i < board->getRowSize(); i++) {
        for (int j = 0; j < board->getColSize(); j++) {
            board->getTile(i, j)->cameFrom = NULL;
            board->getTile(i, j)->g = 2147483647; // INF = MAX_INT
        }
    }
//
//    // The cost of going from start to start is zero.
//    gScore[start] := 0
    start->g = 0;
//
//    // For each node, the total cost of getting from the start node to the goal
//    // by passing by that node. That value is partly known, partly heuristic.
//    fScore := map with default value of Infinity
    for (int i = 0; i < board->getRowSize(); i++) {
        for (int j = 0; j < board->getColSize(); j++) {
            board->getTile(i, j)->f = INF; // INF = MAX_INT
        }
    }	
//
//    // For the first node, that value is completely heuristic.
//    fScore[start] := heuristic_cost_estimate(start, goal)
    start->h = calcHeuristic(start->row, start->col, goal->row, goal->col);
    start->f = start->h;
    
//    while openSet is not empty
    while (openSet.size() > 0) {
//        current := the node in openSet having the lowest fScore[] value
        TileItem curMinFTileItem = openSet.dequeue();
//        cout << "Boss::move()::IQ150: curMinFTileItem = " << curMinFTileItem << endl;
        
//        if current = goal
        if (curMinFTileItem.tile->row == goal->row && curMinFTileItem.tile->col == goal->col) {
//            return reconstruct_path(cameFrom, current)
            reconstruct_path(start, goal);
            pathFound = true;
            curIndexPath = 0;
//            cout << "PATH FOUND!!!!!!!!!!!!!!!!!!!!!" << endl;
            break;
        }
		
//        openSet.Remove(current)
//        closedSet.Add(current)
        closedSet.add(curMinFTileItem);

//        for each neighbor of current
        for (int neighbourDir = 0; neighbourDir < NUM_DIRS; neighbourDir++) {
            Tile *curNeighbour = getNeighbour(curMinFTileItem.tile, neighbourDir);
            
            if (curNeighbour == NULL) {
                continue;
            }
            
            if (curNeighbour->row == goal->row && curNeighbour->col == goal->col) {
                //            return reconstruct_path(cameFrom, current)
                curNeighbour->cameFrom = curMinFTileItem.tile;
                reconstruct_path(start, goal);
                pathFound = true;
                curIndexPath = 0;
//                cout << "PATH FOUND!!!!!!!!!!!!!!!!!!!!!" << endl;
                break;
            }
            
//            if neighbor in closedSet
            if (closedSet.find(TileItem(curNeighbour))) {
                continue; // Ignore the neighbor which is already evaluated.
            }

//            if neighbor not in openSet    // Discover a new node
            if (!openSet.find(TileItem(curNeighbour))) {
//                openSet.Add(neighbor)
                openSet.enqueue(TileItem(curNeighbour));
            }

//            // The distance from start to a neighbor
//            //the "dist_between" function may vary as per the solution requirements.
//            tentative_gScore := gScore[current] + dist_between(current, neighbor)
            double tentativeGScore = curMinFTileItem.tile->g + 1.0;
//            if tentative_gScore >= gScore[neighbor]
            if (tentativeGScore >= curNeighbour->g) {
                continue;        // This is not a better path.
            }

//            // This path is the best until now. Record it!
//            cameFrom[neighbor] := current
            curNeighbour->cameFrom = curMinFTileItem.tile;
//            gScore[neighbor] := tentative_gScore
            curNeighbour->g = tentativeGScore;
//            fScore[neighbor] := gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)
            curNeighbour->h = calcHeuristic(curNeighbour->row, curNeighbour->col, goal->row, goal->col);
            curNeighbour->f = curNeighbour->g + curNeighbour->h;
            
//            cout << "Boss::move()::IQ150: adding curNeighbour = " << curNeighbour << endl;
        }
        
//        cout << "openSet: ";
//        while (openSet.size() > 0) {
//            TileItem curItem = openSet.dequeue();
//            cout << curItem << " ";
//        }
//        cout << endl;
//        exit(1);
    }
    
//    exit(1);
//    return failure
//==============================================================================
//==============================================================================
//==============================================================================
#endif // BOSS_IQ_150
//==============================================================================
//==============================================================================
//==============================================================================

    Monster::move(dir);
}

//function reconstruct_path(cameFrom, current)
void Boss::reconstruct_path(Tile *start, Tile *goal) {
//    cout << "Boss::reconstruct_path(): start = " << TileItem(start) << endl;
//    cout << "Boss::reconstruct_path(): goal  = " << TileItem(goal) << endl;

//    total_path := [current]
//    while current in cameFrom.Keys:
//        current := cameFrom[current]
//        total_path.append(current)
//    return total_path
    
    //--------------------------------------------------------------------------
    // get length of path
    //--------------------------------------------------------------------------
    Tile *curTile = goal->cameFrom;
    
    pathLen = 0;
    
    // while the curTile is not the start tile
    while (!(curTile->row == start->row && curTile->col == start->col)) {
        pathLen++;
        
        curTile = curTile->cameFrom;
    }

//    cout << "Boss::reconstruct_path(): pathLen = " << pathLen << endl;

    //--------------------------------------------------------------------------
    // record the path and the dirs
    //--------------------------------------------------------------------------
    Tile *nextTile = goal;
    curTile = goal->cameFrom;

    int i = pathLen-1;
    
    // while the curTile is not the start tile
    while (!(curTile->row == start->row && curTile->col == start->col)) {
        path[0][i] = curTile->row;
        path[1][i] = curTile->col;
        pathDir[i+1] = getDir(curTile, nextTile);

        nextTile = curTile;
        curTile = curTile->cameFrom;
        
        i--;
    }

    pathDir[i+1] = getDir(curTile, nextTile);
    
//    cout << "Boss::reconstruct_path(): pathLen = " << pathLen << endl;
//    cout << "Boss::reconstruct_path(): path = ";
//    for (int i = 0; i < pathLen; i++) {
//        cout << "(" << path[0][i] << ", " << path[1][i] << ":" << pathDir[i] << ") ";
//    }
//    cout << endl;
}

bool Boss::isBoss() {
    return true;
}

//battle
void Boss::interact(Unit *unit) {
    Monster::interact(unit);
}

// helper function for move()::IQ120
bool Boss::canMove(int dir) {
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
        return true;
    }
    
    return false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Helper Functions for IQ150: A*
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

double Boss::calcHeuristic(int row, int col, const int goalRow, const int goalCol) {
	int rowDiff = abs(goalRow - row);
	int colDiff = abs(goalCol - col);
	
    return sqrt((rowDiff * rowDiff) + (colDiff * colDiff));
}

Tile *Boss::getNeighbour(Tile *curTile, int neighbourDir) {
    int nextRow = curTile->row;
    int nextCol = curTile->col;
    
    if (neighbourDir == DIR_N) {
        nextRow--;
    }
    else if (neighbourDir == DIR_E) {
        nextCol++;
    }
    else if (neighbourDir == DIR_S) {
        nextRow++;
    }
    else if (neighbourDir == DIR_W) {
        nextCol--;
    }
    
    if (nextRow >= 0 && nextRow < board->getRowSize() &&
        nextCol >= 0 && nextCol < board->getColSize() &&
        board->getProp(nextRow, nextCol) == NULL) {
        return board->getTile(nextRow, nextCol);
    }
    
    return NULL;
}

int Boss::getDir(Tile *source, Tile *target) {
    if (source->row-1 == target->row && source->col == target->col) {
        return DIR_N;
    }
    else if (source->row == target->row && source->col+1 == target->col) {
        return DIR_E;
    }
    else if (source->row+1 == target->row && source->col == target->col) {
        return DIR_S;
    }
    else if (source->row == target->row && source->col-1 == target->col) {
        return DIR_W;
    }

    return DIR_NONE;
}
