#ifndef MERCHANT_H
#define MERCHANT_H

#include "unit.h"

class Inventory;

class Merchant : public Unit {
protected:
    Inventory *itemsToSell;
    Inventory *itemsBought;
    
public:
    Merchant();
    virtual ~Merchant();
    
    virtual bool isMerchant();
    
    virtual void move(int dir);
    virtual void interact(Unit *unit);
    
    void buy(Unit *unit);
    void sell(Unit *unit);
};

#endif
