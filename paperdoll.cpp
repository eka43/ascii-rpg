#include <iostream>

using namespace std;

#include "main.h"
#include "item.h"
#include "unit.h"
#include "paperdoll.h"

Paperdoll::Paperdoll() {
    head = NULL;
    back = NULL;
    chest = NULL;
    hands = NULL;
    legs = NULL;
    feet = NULL;
    
    leftHand = NULL;
    rightHand = NULL;
}

Paperdoll::~Paperdoll() {
    if (head != NULL) {
        delete head;
        head = NULL;
    }
    if (back != NULL) {
        delete back;
        back = NULL;
    }
    if (chest != NULL) {
        delete chest;
        chest = NULL;
    }
    if (hands != NULL) {
        delete hands;
        hands = NULL;
    }
    if (legs != NULL) {
        delete legs;
        legs = NULL;
    }
    if (feet != NULL) {
        delete feet;
        feet = NULL;
    }
    if (leftHand != NULL) {
        delete leftHand;
        leftHand = NULL;
    }
    if (rightHand != NULL) {
        delete rightHand;
        rightHand = NULL;
    }
}

void Paperdoll::print() {
    cout << "0:HEAD[" << ((head == NULL)?' ':head->getShape()) << "] ";
    cout << "1:BACK[" << ((back == NULL)?' ':back->getShape()) << "] ";
    cout << "2:CHEST[" << ((chest == NULL)?' ':chest->getShape()) << "] ";
    cout << "3:HANDS[" << ((hands == NULL)?' ':hands->getShape()) << "] ";
    cout << "4:LEGS[" << ((legs == NULL)?' ':legs->getShape()) << "] ";
    cout << "5:FEET[" << ((feet == NULL)?' ':feet->getShape()) << "] ";
    cout << "6:LH[" << ((leftHand == NULL)?' ':leftHand->getShape()) << "] ";
    cout << "7:RH[" << ((rightHand == NULL)?' ':rightHand->getShape()) << "]";
}

bool Paperdoll::equip(Unit *unit, Item *itemToEquip) {
    if (unit == NULL) {
        cout << "Paperdoll::equip(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (itemToEquip == NULL) {
        cout << "Paperdoll::equip(): if (itemToEquip == NULL) {" << endl;
        exit(1);
    }
    
    if (itemToEquip->isEquippable()) {
        EquippableItem *equippableItem = (EquippableItem *)itemToEquip;
        
        if (equippableItem->isArmor()) {
            Armor *armor = (Armor *)equippableItem;
            
            int bodyPartID = armor->getBodyPartID();
            if (bodyPartID == ARMOR_BODYPART_ID_HEAD) {
                if (head == NULL) {
                    head = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_BACK) {
                if (back == NULL) {
                    back = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_CHEST) {
                if (chest == NULL) {
                    chest = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_HANDS) {
                if (hands == NULL) {
                    hands = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_LEGS) {
                if (legs == NULL) {
                    legs = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            else if (bodyPartID == ARMOR_BODYPART_ID_FEET) {
                if (feet == NULL) {
                    feet = armor;
                    unit->incDef(armor->getDef());
                    return true;
                }
                else {
                    return false;
                }
            }
            
            // 숙제.. 나머지 부분.. 그냥 복붙............................................................. 부츠까지(손을 빼고)
        }
        else if (equippableItem->isWeapon()) {
            Weapon *weapon = (Weapon *)equippableItem;
            
            if (weapon->getNumHands() == WEAPON_ONE_HAND) {
                if (rightHand == NULL) {
                    rightHand = weapon;
                    unit->incAtk(weapon->getAtk());
                    return true;
                }
                else if (leftHand == NULL) {
                    if (rightHand->getNumHands() == WEAPON_ONE_HAND) {
                        leftHand = weapon;
                        unit->incAtk(weapon->getAtk());
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
            else { // if (weapon->getNumHands() == WEAPON_TWO_HAND) {
                if (rightHand == NULL) {
                    rightHand = weapon;
                    unit->incAtk(weapon->getAtk());
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            cout << "Paperdoll::equip(): unsupported type item!!!" << endl;
            exit(1);
        }
    }
    
    return false;
}

//#define ARMOR_BODYPART_ID_NONE -1
//#define ARMOR_BODYPART_ID_HEAD 0
//#define ARMOR_BODYPART_ID_CHEST 1
//#define ARMOR_BODYPART_ID_GLOVES 2
//#define ARMOR_BODYPART_ID_BACK 3
//#define ARMOR_BODYPART_ID_PANTS 4
//#define ARMOR_BODYPART_ID_BOOTS 5
//#define WEAPON_BODYPART_ID_LEFT_HAND 6
//#define WEAPON_BODYPART_ID_RIGHT_HAND 7

Item *Paperdoll::unequip(Unit *unit, int bodyPartID) {
	cout << "Inside of the unequip!" << endl;
	cout << "bodypart id is " << bodyPartID << endl;
    
//    Armor *armor = getRandomArmorByBodyPartID(bodyPartID);
//    armor->print();
//
//    if(armor==NULL)
//        cout<< "armor is NULL!"<<endl;
//    int bodyPartID = bodyPartID;
//    cout<< "body id is" << bodyPartID << endl;
	
    if (unit == NULL) {
        cout << "Paperdoll::unequip(): if (unit == NULL) {" << endl;
        exit(1);
    }
    
    if (bodyPartID < ARMOR_BODYPART_ID_HEAD || bodyPartID > WEAPON_BODYPART_ID_RIGHT_HAND) {
        cout << "Paperdoll::unequip(): if (bodyPartID < ARMOR_BODYPART_ID_HEAD || bodyPartID > WEAPON_BODYPART_ID_RIGHT_HAND) {" << endl;
        exit(1);
    }
    
    Item *itemToReturn = NULL;

    if (bodyPartID == ARMOR_BODYPART_ID_HEAD ||
		bodyPartID == ARMOR_BODYPART_ID_BACK ||
		bodyPartID == ARMOR_BODYPART_ID_CHEST ||
		bodyPartID == ARMOR_BODYPART_ID_HANDS ||
		bodyPartID == ARMOR_BODYPART_ID_LEGS ||
		bodyPartID == ARMOR_BODYPART_ID_FEET) {
			
		cout << "Inside of the body p1!" << endl;
        
		if (bodyPartID == ARMOR_BODYPART_ID_HEAD) {
			
			cout << "Inside of the head!" << endl;
			
            if (head != NULL) {
                unit->decDef(head->getDef());
                itemToReturn = head;
                head = NULL;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
            }
        }
        else if (bodyPartID == ARMOR_BODYPART_ID_BACK) {
            if (back != NULL) {
                unit->decDef(back->getDef());
                itemToReturn = back;
                back = NULL;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
            }
        }
        else if (bodyPartID == ARMOR_BODYPART_ID_CHEST) {
            if (chest != NULL) {
                unit->decDef(chest->getDef());
                itemToReturn = chest;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
                chest = NULL;
            }
        }
        else if (bodyPartID == ARMOR_BODYPART_ID_HANDS) {
            if (hands != NULL) {
                unit->decDef(hands->getDef());
                itemToReturn = hands;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
                hands = NULL;
            }
        }
        else if (bodyPartID == ARMOR_BODYPART_ID_LEGS) {
            if (legs != NULL) {
                unit->decDef(legs->getDef());
                itemToReturn = legs;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
                legs = NULL;
            }
        }
        else if (bodyPartID == ARMOR_BODYPART_ID_FEET) {
            if (feet != NULL) {
                itemToReturn = feet;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
                feet = NULL;
            }
        }
    }
    else if (bodyPartID == WEAPON_BODYPART_ID_LEFT_HAND ||
             bodyPartID == WEAPON_BODYPART_ID_RIGHT_HAND) {
        if (bodyPartID == WEAPON_BODYPART_ID_RIGHT_HAND) {
            if (rightHand != NULL) {
                unit->decAtk(rightHand->getAtk());
                itemToReturn = rightHand;
                cout << "Shape of itemToReturn is :";
                itemToReturn->print();
                cout << endl;
                
                // 만약 leftHand에 또 다른 한손 무기가 있을 경우는, 오른손으로 옮겨잡는다.
                rightHand = leftHand;
                leftHand = NULL;
            }
        }
        else if (bodyPartID == WEAPON_BODYPART_ID_LEFT_HAND) {
            if (leftHand != NULL) {
                unit->decAtk(leftHand->getAtk());
                itemToReturn = leftHand;
                cout << "Shape of armor is :";
                itemToReturn->print();
                cout << endl;
                leftHand = NULL;
            }
        }
    }
    else {
        cout << "Paperdoll::unequip(): unsupported type item!!!" << endl;
        exit(1);
    }
    
    return itemToReturn;
}
