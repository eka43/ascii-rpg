#include <iostream>
#include <stdlib.h>

using namespace std;

#include "main.h"
#include "item.h"
#include "prop.h"
#include "unit.h"
#include "hero.h"
#include "board.h"
#include "inventory.h"
#include "merchant.h"

Merchant::Merchant() : Unit(MERCHANT_SHAPE, -1, -1, DIR_NONE, 0, 0, 0, 0, 0, 0, 0, 0) {
    itemsToSell = new Inventory(10);
    
    for (int i = 0; i < 10; i++) {
        itemsToSell->add(createRandomItem());
    }
    
    itemsBought = new Inventory(20);
}

Merchant::~Merchant() {
}

bool Merchant::isMerchant() {
    return true;
}

void Merchant::move(int dir) {
}

void Merchant::interact(Unit *unit) {
    char userCommand;
    
    while (true) {
        // show the menu
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "=========================== EZ SHOP ==========================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;

        cout << "(B)uy, (S)ell, or e(X)it: ";
        
        cin >> userCommand;
        
        if (userCommand == 'b' || userCommand == 'B') {
            buy(unit);
        }
        else if (userCommand == 's' || userCommand == 'S') {
            sell(unit);
        }
        else if (userCommand == 'x' || userCommand == 'X') {
            break;
        }
    }
}

void Merchant::buy(Unit *unit) {
    int indexToBuy;
    
    while (true) {
        // show the menu
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "=========================== EZ SHOP ==========================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << endl;
        cout << "--------------------------------------------------------------" << endl;
        cout << "                         Items to sell" << endl;
        cout << "--------------------------------------------------------------" << endl;

        // show the items' stat
        for (int i = 0; i < itemsToSell->capacity(); i++) {
            cout << i << ": ";
            itemsToSell->getItemAt(i)->printFullSpec();
            cout << endl;
        }

        cout << "--------------------------------------------------------------" << endl;
        cout << "                         Items bought" << endl;
        cout << "--------------------------------------------------------------" << endl;

        for (int i = 0; i < itemsBought->size(); i++) {
            cout << i+100 << ": ";
            itemsBought->getItemAt(i)->printFullSpec();
            cout << endl;
        }

        cout << "--------------------------------------------------------------" << endl;
        // show the user stat
        unit->printStat();

        cout << "--------------------------------------------------------------" << endl;
        cout << "Enter the index of item to purchase (-1 to exit): ";
        // get the input
        cin >> indexToBuy;
        
        if (indexToBuy == -1) {
            break;
        }
        // 0-9 사이의 index 번호를 입력하면.. 그 아이템의 가격을 Hero가 afford 할 수 있는지 체크.
        else if (indexToBuy >= 0 && indexToBuy < 10) {
            // 돈이 충분할 경우..
            if (unit->getGold() >= itemsToSell->getItemAt(indexToBuy)->getPrice()) {
                // 물건을 받을수 있는지, 여유공간이 인벤토리나 백팩에 있는지 점검
                if (unit->canReceiveItem()) {
                    // 돈을 받는다.
                    unit->decGold(itemsToSell->getItemAt(indexToBuy)->getPrice());
                    
                    // 여유공간이 있을 경우, 물건의 clone을 만들고..
                    Item *itemClone = itemsToSell->getItemAt(indexToBuy)->clone();
//                    cout << "Here you go: ";
//                    itemClone->print();
//                    cout << endl;
                    unit->receiveItem(itemClone);
                }
                else {
                    cout << "error: you don't have enough room to receive this item." << endl;
                }
            }
            else {
                cout << "error: you can't afford this item's price." << endl;
            }
        }
        else if (indexToBuy >= 100 && indexToBuy < itemsBought->size()+100) {
            int indexToBuyBack = indexToBuy - 100;
            // 돈이 충분할 경우..
            if (unit->getGold() >= itemsBought->getItemAt(indexToBuyBack)->getPrice()) {
                // 물건을 받을수 있는지, 여유공간이 인벤토리나 백팩에 있는지 점검
                if (unit->canReceiveItem()) {
                    // 돈을 받는다.
                    unit->decGold(itemsBought->getItemAt(indexToBuyBack)->getPrice());
                    unit->receiveItem(itemsBought->removeItemAt(indexToBuyBack));
                    itemsBought->pack(); // 빈 슬롯이 있다면, 모두 없앤다. 즉 앞으로 땡긴다.
                }
                else {
                    cout << "error: you don't have enough room to receive this item." << endl;
                }
            }
            else {
                cout << "error: you can't afford this item's price." << endl;
            }
        }
        else {
            cout << "error: invalid input." << endl;
        }
    }
}

void Merchant::sell(Unit *unit) {
    int indexToSell;
    
    while (true) {
        // show the menu
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "=========================== EZ SHOP ==========================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;
        cout << "==============================================================" << endl;

        cout << "---------------------------- Slots----------------------------" << endl;
        unit->printSlots();
        cout << "-------------------------- Backpack --------------------------" << endl;
        unit->printBackpack();
        cout << "--------------------------------------------------------------" << endl;
        // show the user stat
		cout << endl;
		cout << "------------------------- Your Stat --------------------------" << endl;
        unit->printStat();
		cout << endl;
        cout << "-------------------- Merchant Items Bought --------------------------" << endl;
		// show the item stat
        if (itemsBought->size() > 0) {
            itemsBought->print();
            cout << endl;
        }
        else {
            cout << "No items bought!!!" << endl;
        }
			
        cout << "--------------------------------------------------------------" << endl;
		cout << "Enter the index of item to sell (0-" << (unit->capacitySlots()-1) << ", or 100-" << (unit->sizeBackpack()+100-1) << ", or -1 to exit): ";
        // get the input
        cin >> indexToSell;
        
        if (indexToSell == -1) {
            break;
        }
        // 0-4 사이의 index 번호를 입력하면.. slots의 물건을 판다.
		else if (indexToSell >= 0 && indexToSell < unit->capacitySlots()) {
            if (unit->getSlotItemAt(indexToSell) != NULL)	{
                Item *itemToSell = unit->removeSlotItemAt(indexToSell);
                cout << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
                cout << "You sold " << itemToSell->getName() << " at " <<itemToSell->getPrice() << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
                cout << endl;
                unit->incGold(itemToSell->getPrice());
                itemsBought->add(itemToSell);
            }
            else {
                cout << endl;
                cout << "error: It's an empty space!" << endl;
                cout << endl;
            }
        }
        else if (indexToSell >= 100 && indexToSell < (unit->sizeBackpack()+100)) {
            if (unit->getBackpackItemAt(indexToSell-100) != NULL)    {
                Item *itemToSell = unit->removeBackpackItemAt(indexToSell-100);
                cout << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
                cout << "You sold " << itemToSell->getName() << " at " << itemToSell->getPrice() << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
                cout << endl;
                unit->incGold(itemToSell->getPrice());
                itemsBought->add(itemToSell);
            }
            else {
                cout << endl;
                cout << "error: It's an empty space!" << endl;
                cout << endl;
            }
        }
        else {
            cout << endl;
            cout << "error: invalid input." << endl;
            cout << endl;
        }
    }
}
