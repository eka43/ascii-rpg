#include <iostream>
#include <stdlib.h>

using namespace std;

#include "main.h"
#include "prop.h"
#include "item.h"
#include "unit.h"
#include "hero.h"
#include "monster.h"
#include "boss.h"
#include "pet.h"
#include "merchant.h"
#include "tile.h"
#include "board.h"
#include "shockwave.h"

#ifdef TEST_CASE_ON

//char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//    "..........",
//};

//// Boss IQ120
//char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
//    "..........",
//    "..Y.Y.....",
//    "..Y&Y.....",
//    "..Y.Y.....",
//    "...Y......",
//    "..........",
//    "..........",
//    "...H......",
//    "..........",
//    "..........",
//};

// Boss IQ120
//// Boss IQ120: debug: symptom이 좌상단의 &가 계속 board의 boundary를 타고 돈다.
//char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
//"....&Y......YY.........Y.......mYY...b.m",
//".............Y...bO..b......b....Y......",
//"..Y..bO.....b.....m.......YY...........b",
//".........Yb.....Y.Y.......m..Y..........",
//"........b..............mb...........YYY.",
//"..O.Y.m.b....YYb..b.H....Y...Y..b......m",
//"..b............Y.O............m.........",
//"........YY...........b.Y..Y.....Y..Y.bb.",
//"...O......YYbY&..m...Y..Y...............",
//"m....................Y..Y....Y.b.....YY.",
//};

// Boss IQ120: debug: symptom이 좌상단의 &가 계속 board의 boundary를 타고 돈다.
// * main.h의 ROW_SIZE와 COL_SIZE를 각각 10과 40으로 고쳐줄 것.
//char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
//    "....&Y......YY.........Y........YY......",
//    ".............Y...................Y......",
//    "........................................",
//    "........................................",
//    "........................................",
//    "....................H..................",
//    "........................................",
//    ".......................Y..Y.............",
//    ".....................Y..Y...............",
//    ".....................Y..Y....Y.......YY.",
//};

// Boss IQ120: debug: symptom이 좌상단의 &가 계속 board의 boundary를 타고 돈다.
// * main.h의 ROW_SIZE와 COL_SIZE를 각각 10과 40으로 고쳐줄 것.
//char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
//    ".....Y......YY.........Y........YY......",
//    ".............Y...................Y......",
//    "........................................",
//    "................&..............&........",
//    "........................................",
//    "...................GH^..................",
//    "....................b...................",
//    "....................b..Y..Y.............",
//    "..G.................bY..Y...............",
//    "..............bbbbbbbY..Y&...Y.......YY.",
//};

// Boss IQ120: debug: symptom이 좌상단의 &가 계속 board의 boundary를 타고 돈다.
// * main.h의 ROW_SIZE와 COL_SIZE를 각각 10과 40으로 고쳐줄 것.
char TEST_BOARD[ROW_SIZE][COL_SIZE+1] = {
    "........................................",
    "........................................",
    "....Y.Y.................................",
    "....Y.Y.................................",
    "....Y&Y.................................",
    "....Y.Y.................................",
    ".....Y..................................",
    "........................................",
    ".....H..................................",
    "........................................",
};

#endif

Board::Board() {
//    init(ROW_SIZE, COL_SIZE);
    init(HUGE_ROW_SIZE, HUGE_COL_SIZE);
}

Board::Board(int rowSize, int colSize) {
    init(rowSize, colSize);
}

// recycle한 코드 = prevent redundant code = avoid duplicated code
// = logical error를 피할 수 있다.
// = 만약 init하는 코드가 여기저기 있다면, 어떤 member variable은 어디선가 소외되어
//   init 안 될 수도 있다. 발견하기 힘들다.
void Board::init(int rowSize, int colSize) {
    this->rowSize = rowSize;
    this->colSize = colSize;
    
    // create 2D array of class Tile
    board = new Tile **[rowSize]; // array to hold rows(lines)
    
    for (int i = 0; i < rowSize; i++) {
        board[i] = new Tile *[colSize]; // array to hold Tile objects = 1 line
    }
    
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            board[i][j] = new Tile(i, j); // object of class Tile = 1 tile
        }
    }
    
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifdef TEST_CASE_ON
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    maxNumPortals = 0;
    maxNumMons = 0;
    maxNumBosses = 0;
	maxNumPets = 0;
    maxNumMerchants = 0;

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (TEST_BOARD[i][j] == PORTAL_SHAPE) {
                maxNumPortals++;
            }
            else if (TEST_BOARD[i][j] == MONSTER_SHAPE) {
                maxNumMons++;
            }
            else if (TEST_BOARD[i][j] == BOSS_SHAPE) {
                maxNumBosses++;
            }
			else if(TEST_BOARD[i][j] == PET_SHAPE) {
				maxNumPets++;
			}
            else if(TEST_BOARD[i][j] == MERCHANT_SHAPE) {
                maxNumMerchants++;
            }
        }
    }
    
    portals = new Portal *[maxNumPortals];
    numPortals = 0;
    
    mons = new Monster *[maxNumMons];
    numMons = 0;
    
    bosses = new Boss *[maxNumBosses];
    numBosses = 0;
    
	pets = new Pet *[maxNumPets];
	numPets = 0;
	
    merchants = new Merchant *[maxNumMerchants];
    numMerchants = 0;

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            //------------------------------------------------------------------
            // Units
            //------------------------------------------------------------------
            if (TEST_BOARD[i][j] == HERO_SHAPE) {
                hero = new Hero();
                hero->setBoard(this);
                setUnit(i, j, hero);
            }
            else if (TEST_BOARD[i][j] == MONSTER_SHAPE) {
                mons[numMons] = new Monster();
                mons[numMons]->setBoard(this);
                setUnit(i, j, mons[numMons]);
                numMons++;
            }
            else if (TEST_BOARD[i][j] == BOSS_SHAPE) {
                bosses[numBosses] = new Boss();
                bosses[numBosses]->setBoard(this);
                setUnit(i, j, bosses[numBosses]);
                numBosses++;
            }            
			else if (TEST_BOARD[i][j] == PET_SHAPE) {
				pets[numPets] = new Pet();
				pets[numPets]-> setBoard(this);
				setUnit(i, j, pets[numPets]);
				numPets++;
			}
            else if (TEST_BOARD[i][j] == MERCHANT_SHAPE) {
                merchants[numMerchants] = new Merchant();
                merchants[numMerchants]-> setBoard(this);
                setUnit(i, j, merchants[numMerchants]);
                numMerchants++;
            }
            //------------------------------------------------------------------
            // Prop
            //------------------------------------------------------------------
            else if (TEST_BOARD[i][j] == 'Y') {
                setProp(i, j, new Tree());
            }
            else if (TEST_BOARD[i][j] == 'O') {
                portals[numPortals] = new Portal();
                portals[numPortals]->setBoard(this);
                setProp(i, j, portals[numPortals]);
                numPortals++;
            }
            //------------------------------------------------------------------
            // Items
            //------------------------------------------------------------------
            else if (TEST_BOARD[i][j] == 'b') {
                setItem(i, j, new Potion());
            }
        }
    }
    
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    
    hero = new Hero();
    hero->setBoard(this);
    setUnit(HERO_ROW, HERO_COL, hero);
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // init props
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // create random trees
    //--------------------------------------------------------------------------
    for (int i = 0; i < HUGE_MAX_NUM_TREES; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                setProp(randRow, randCol, new Tree());
                break;
            }
        }
    }

    //--------------------------------------------------------------------------
    // create random portals
    //--------------------------------------------------------------------------
/*    for (int i = 0; i < HUGE_MAX_NUM_PORTALS; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                Portal *newPortal = new Portal();
                newPortal->setBoard(this);
                setProp(randRow, randCol, newPortal);
                break;
            }
        }
    }*/
	
	maxNumPortals = HUGE_MAX_NUM_PORTALS;
    portals = new Portal *[maxNumPortals];
    numPortals = 0;
    
    for (int i = 0; i < maxNumPortals; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                portals[i] = new Portal();
                portals[i]->setBoard(this);
                setProp(randRow, randCol, portals[i]);
                break;
            }
        }
        numPortals++;
    }

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // init items
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    for (int i = 0; i < HUGE_MAX_NUM_POTIONS; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                setItem(randRow, randCol, new Potion());
                break;
            }
        }
    }
	
	//--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // init units
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // init monsters
    //--------------------------------------------------------------------------
    maxNumMons = HUGE_MAX_NUM_MONS;
    mons = new Monster *[maxNumMons];
    numMons = 0;
    
    for (int i = 0; i < maxNumMons; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                mons[i] = new Monster();
                mons[i]->setBoard(this);
                setUnit(randRow, randCol, mons[i]);
                break;
            }
        }
        numMons++;
    }

    //--------------------------------------------------------------------------
    // init bosses
    //--------------------------------------------------------------------------
    maxNumBosses = HUGE_MAX_NUM_BOSSES;
    bosses = new Boss *[maxNumBosses];
    numBosses = 0;
    
    for (int i = 0; i < maxNumBosses; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                bosses[i] = new Boss();
                bosses[i]->setBoard(this);
                setUnit(randRow, randCol, bosses[i]);
                break;
            }
        }
        numBosses++;
    }

    //--------------------------------------------------------------------------
    // init pets
    //--------------------------------------------------------------------------
    maxNumPets = HUGE_MAX_NUM_PETS;
    pets = new Pet *[maxNumPets];
    numPets = 0;
    
    for (int i = 0; i < maxNumPets; i++) {
        while (true) {
            int randRow = hero->getRow() + (rand() % (2*PET_RANGE+1) + (-PET_RANGE));
            int randCol = hero->getCol() + (rand() % (2*PET_RANGE+1) + (-PET_RANGE));
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                pets[i] = new Pet();
                pets[i]->setBoard(this);
                setUnit(randRow, randCol, pets[i]);
                break;
            }
        }
        numPets++;
    }
    
    //--------------------------------------------------------------------------
    // init merchants
    //--------------------------------------------------------------------------
    maxNumMerchants = HUGE_MAX_NUM_MERCHANTS;
    merchants = new Merchant *[maxNumMerchants];
    numMerchants = 0;
    
    for (int i = 0; i < maxNumMerchants; i++) {
        while (true) {
            int randRow = rand() % rowSize;
            int randCol = rand() % colSize;
            
            if (validate(randRow, randCol) &&
                getProp(randRow, randCol) == NULL &&
                getItem(randRow, randCol) == NULL &&
                getUnit(randRow, randCol) == NULL) {
                merchants[i] = new Merchant();
                merchants[i]->setBoard(this);
                setUnit(randRow, randCol, merchants[i]);
                break;
            }
        }
        numMerchants++;
    }
    
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    initFx();
}

Board::~Board() {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            delete board[i][j];
        }
    }
    
    for (int i = 0; i < rowSize; i++) {
        delete [] board[i];
    }
    
    delete [] board;
    board = NULL;
}

void Board::print() {
    // process all shockwaves --------------------------------------------------
    for (int i = 0; i < maxNumShockwaves; i++) {
        if (shockwaves[i] != NULL) {
            if (!shockwaves[i]->getWaveActivated()) {
                delete shockwaves[i];
                shockwaves[i] = NULL;
                numShockwaves--;
            }
            else {
                shockwaves[i]->print();
            }
        }
    }
    // process all shockwaves end ----------------------------------------------
    
	for (int j = 0; j < colSize; j++) {
		cout << "-";
	}
	cout << endl;

    int ii = 0; // row index of minimap
    
    for (int i = hero->getRow()-VIEWPORT_HALF_ROW_SIZE; i <= hero->getRow()+VIEWPORT_HALF_ROW_SIZE; i++) {
        for (int j = hero->getCol()-VIEWPORT_HALF_COL_SIZE; j <= hero->getCol()+VIEWPORT_HALF_COL_SIZE; j++) {
            if (validate(i, j)) {
                board[i][j]->print();
            }
            else {
                cout << '#';
            }
        }
        
        cout << "   "; // gap between the viewport and the minimap
        
        //----------------------------------------------------------------------
        // print a row of minimap
        //----------------------------------------------------------------------
        if (ii < MINIMAP_ROW_SIZE) {
            for (int jj = 0; jj < MINIMAP_COL_SIZE; jj++) {
                Hero *heroFound = NULL;
                Boss *bossFound = NULL;
                Merchant *merchantFound = NULL;
                Portal *portalFound = NULL;

                // scan the area of the current cell (ii. jj)
                for (int iii = ii*MINIMAP_CELL_ROW_SIZE; iii < (ii+1)*MINIMAP_CELL_ROW_SIZE; iii++) {
                    for (int jjj = jj*MINIMAP_CELL_COL_SIZE; jjj < (jj+1)*MINIMAP_CELL_COL_SIZE; jjj++) {
                        if (board[iii][jjj]->getUnit() != NULL &&
                            board[iii][jjj]->getUnit()->isHero()) {
                            heroFound = (Hero *)board[iii][jjj]->getUnit();
                        }
                        if (board[iii][jjj]->getUnit() != NULL &&
                            board[iii][jjj]->getUnit()->isBoss()) {
                            bossFound = (Boss *)board[iii][jjj]->getUnit();
                        }
                        if (board[iii][jjj]->getUnit() != NULL &&
                            board[iii][jjj]->getUnit()->isMerchant()) {
                            merchantFound = (Merchant *)board[iii][jjj]->getUnit();
                        }
                        if (board[iii][jjj]->getProp() != NULL &&
                            board[iii][jjj]->getProp()->isPortal()) {
                            portalFound = (Portal *)board[iii][jjj]->getProp();
                        }
                    }
                }
                
                if (heroFound != NULL) {
                    heroFound->print();
                }
                else if (bossFound != NULL) {
                    bossFound->print();
                }
                else if (merchantFound != NULL) {
                    merchantFound->print();
                }
                else if (portalFound != NULL) {
                    portalFound->print();
                }
                else {
                    cout << ".";
                }
            }
            
            ii++;
        }
        //----------------------------------------------------------------------
        cout << endl;
    }
}

int Board::getRowSize() {
    return rowSize;
}

int Board::getColSize() {
    return colSize;
}

Tile *Board::getTile(int row, int col) {
    return board[row][col];
}

Hero *Board::getHero() {
	return hero;
}

bool Board::validate(int row, int col) {
    return row >= 0 && row < rowSize && col >= 0 && col < colSize;
}

bool Board::isOutOfBoundary(int row, int col) { // ADD
    return row < 0 || row >= rowSize || col < 0 || col >= colSize;
}

bool Board::isBlocked(int row, int col) { // ADD
    if (isOutOfBoundary(row, col)) {
        return true;
    }
    else {
        return (getUnit(row, col) != NULL || getProp(row, col) != NULL);
    }
}

Unit *Board::getUnit(int row, int col) {
    if (!validate(row, col)) {
        cout << "Board::getUnit(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    return board[row][col]->getUnit();
}

void Board::setUnit(int row, int col, Unit *unit) {
    if (!validate(row, col)) {
        cout << "Board::getUnit(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    board[row][col]->setUnit(unit);
    
    if (unit != NULL) {
        unit->setRow(row);
        unit->setCol(col);
    }
}

Item *Board::getItem(int row, int col) {
    if (!validate(row, col)) {
        cout << "Board::getItem(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    return board[row][col]->getItem();
}

void Board::setItem(int row, int col, Item *item) {
    if (!validate(row, col)) {
        cout << "Board::setItem(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    board[row][col]->setItem(item);
}

Prop *Board::getProp(int row, int col) {
    if (!validate(row, col)) {
        cout << "Board::getProp(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    return board[row][col]->getProp();
}

void Board::setProp(int row, int col, Prop *prop) {
    if (!validate(row, col)) {
        cout << "Board::setProp(): error: invalid row or col: row=" << row << " col=" << col << endl;
        exit(1);
    }
    
    board[row][col]->setProp(prop);
    
    if (prop != NULL) {
        prop->setRow(row);
        prop->setCol(col);
    }
}

void Board::moveMons() {
    for (int i = 0; i < maxNumMons; i++) {
        if (mons[i]->isDead() && mons[i]->getRow() != -1) { // remove corpse from board
            setUnit(mons[i]->getRow(), mons[i]->getCol(), NULL);
            mons[i]->setRow(-1);
            mons[i]->setCol(-1);
        }
        else {
            int randDir = rand() % NUM_DIRS;
            mons[i]->move(randDir);
        }
    }
}

void Board::moveBosses() {
    for (int i = 0; i < maxNumBosses; i++) {
        if (bosses[i]->isDead() && bosses[i]->getRow() != -1) { // remove corpse from board
            setUnit(bosses[i]->getRow(), bosses[i]->getCol(), NULL);
            bosses[i]->setRow(-1);
            bosses[i]->setCol(-1);
        }
        else {
            int randDir = rand() % NUM_DIRS;
            bosses[i]->move(randDir);
        }
    }
}

void Board::movePets() {
    for (int i = 0; i < maxNumPets; i++) {
        if (pets[i]->isDead() && pets[i]->getRow() != -1) { // remove corpse from board
            setUnit(pets[i]->getRow(), pets[i]->getCol(), NULL);
            pets[i]->setRow(-1);
            pets[i]->setCol(-1);
        }
        else {
            int randDir = rand() % NUM_DIRS;
            pets[i]->move(randDir);
        }
    }
}

Portal *Board::getRandomPortal() {
	int randNumPortal = rand() % HUGE_MAX_NUM_PORTALS;

	return portals[randNumPortal];
}

//--------------------------------------------------------------------------
// for boss IQ 150
//--------------------------------------------------------------------------	
void Board::setf(int row, int col, int x) {
	board[row][col]->setf(x);
}

void Board::setg(int row, int col, int x) {
	board[row][col]->setg(x);
}

void Board::seth(int row, int col, int x) {
	board[row][col]->seth(x);
}

void Board::switchIsEmpty(int row, int col, int x) {
	board[row][col]->switchIsEmpty(x);
}

void Board::switchIsOpen(int row, int col, int x) {
	board[row][col]->switchIsOpen(x);
}

bool Board::askIsOpen(int row, int col) {
	return board[row][col]->returnIsOpen();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// for shockwave
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Board::initFx() {
    maxNumShockwaves = MAX_NUM_SHOCKWAVES;
    shockwaves = new Shockwave *[maxNumShockwaves];
    numShockwaves = 0;
    
    for (int i = 0; i < maxNumShockwaves; i++) {
        shockwaves[i] = NULL;
    }
}

void Board::destroyFx() {
    if (shockwaves != NULL) {
        for (int i = 0; i < maxNumShockwaves; i++) {
            if (shockwaves[i] != NULL) {
                delete shockwaves[i];
            }
        }
        
        delete [] shockwaves;
        shockwaves = NULL;
        
        maxNumShockwaves = 0;
        numShockwaves = 0;
    }
}

void Board::clearFx(int row, int col) {
    board[row][col]->clearFx();
}

void Board::setFx(int row, int col, char fx) {
    board[row][col]->setFx(fx);
}

void Board::startWave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
                      int row, int col, int range, int frameInterval, char fxShape, int skillNumber) {
    if (numShockwaves >= maxNumShockwaves) {
        cout << "error: can't start new shockwave!";
        return;
    }
    
    for (int i = 0; i < maxNumShockwaves; i++) {
        if (shockwaves[i] == NULL) {
            shockwaves[i] = new Shockwave(waveUnit, waveItem, waveProp,
                                          row, col, range, frameInterval, fxShape);
            shockwaves[i]->start();
            shockwaves[i]->setSkillNumber(skillNumber);
            numShockwaves++;
            break;
        }
    }
}
