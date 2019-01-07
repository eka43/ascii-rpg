#ifndef PROP_H
#define PROP_H

class Unit;
class Board;

class Prop {
protected:
    char shape;
	
	int row;
	int col;

    Board *board;
    
public:
    Prop();
    Prop(char shape);
	
    virtual ~Prop();

    void print();
    
    void setBoard(Board *board);
	
	int getRow();
    void setRow(int row);

	int getCol();
    void setCol(int col);

    virtual bool isClimbable();

    virtual bool isTree();
    virtual bool isPortal();

    virtual void trigger(Unit *unit);
};

class Tree : public Prop {
protected:
public:
    Tree();
    virtual ~Tree();
    
    virtual bool isTree();
};

class Portal : public Prop {
protected:
public:
    Portal();
	
    virtual ~Portal();
    
    virtual bool isClimbable();

    virtual bool isPortal();

    virtual void trigger(Unit *unit);
};

#endif
