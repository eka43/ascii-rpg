#ifndef BOARD_H
#define BOARD_H

class Tile;
class Hero;
class Pet;
class Monster;
class Boss;
class Merchant;
class Prop;
class Portal;
class Item;
class Shockwave;

class Board {
private:
    Tile ***board; // 2D array of class Tile
    int rowSize;
    int colSize;
    
    Hero *hero;
    
    Monster **mons;
    int maxNumMons; // capacity
    int numMons; // size
	
    Boss **bosses;
    int maxNumBosses; // capacity
    int numBosses; // size
    
	Portal **portals;
	int maxNumPortals; //capacity
	int numPortals; //size
	
	Pet **pets;
	int maxNumPets; //capacity
	int numPets; //size

    Merchant **merchants;
    int maxNumMerchants; //capacity
    int numMerchants; //size

    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    Shockwave **shockwaves;
    int maxNumShockwaves;
    int numShockwaves;
    
public:
    Board();
    Board(int rowSize, int colSize);
    virtual ~Board();

    // recycle한 코드 = prevent redundant code = avoid duplicated code
    // = logical error를 피할 수 있다.
    // = 만약 init하는 코드가 여기저기 있다면, 어떤 member variable은 어디선가 소외되어
    //   init 안 될 수도 있다. 발견하기 힘들다.
    void init(int rowSize, int colSize);
    
    void print();

    int getRowSize();
    int getColSize();

    Tile *getTile(int row, int col);
    
	Hero *getHero();
    
    bool validate(int row, int col);
	
	bool isOutOfBoundary(int row, int col);// ADD
	bool isBlocked(int row, int col);// ADD

    Unit *getUnit(int row, int col);
    void setUnit(int row, int col, Unit *unit);

    Item *getItem(int row, int col);
    void setItem(int row, int col, Item *item);

    Prop *getProp(int row, int col);
    void setProp(int row, int col, Prop *prop);
    
    void moveMons();
    void moveBosses();
	void movePets();

	Portal *getRandomPortal();
	
	//--------------------------------------------------------------------------
    // for boss IQ 150
    //--------------------------------------------------------------------------	
	void setf(int row, int col, int x);
	void setg(int row, int col, int x);
	void seth(int row, int col, int x);
	void switchIsEmpty(int row, int col, int x);
	void switchIsOpen(int row, int col, int x);
	bool askIsOpen(int row, int col);
    //--------------------------------------------------------------------------
    // for shockwave
    //--------------------------------------------------------------------------
    void initFx();
    void destroyFx();
    
    void clearFx(int row, int col);
    void setFx(int row, int col, char fx);
    void startWave(Unit *waveUnit, Item *waveItem, Prop *waveProp,
                   int row, int col, int range, int frameInterval, char fxShape, int skillNumber);
};

#endif
