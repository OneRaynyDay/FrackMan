//
//  Squirt.h
//  FrackMan
//
//  Created by hongshuhong on 2/16/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Squirt_h
#define Squirt_h

#include "Actor.h"
#include "StudentWorld.h"
#include "Protester.h"

class Squirt : public Item{
    /*
     1. The Squirt object must have an image ID of IID_SQUIRT.
     2. All Squirt objects must have their x,y location specified for them by the
     FrackMan.
     3. All Squirt objects must have their direction specified for them by the FrackMan.
     4. All Squirt objects start off with an initial travel distance of 4 squares
     5. All Squirt objects have the following graphic parameters:
     a. They have an image depth of 1 – behind actors like Protesters, but above Dirt
     b. They have a size of 1.0
     */
    static const int SQUIRT_DAMAGE = 2;
    static const int SQUIRT_HEALTH = 5;
public:
    Squirt(StudentWorld* w, FrackMan* f, Direction dir, int startX, int startY) : Item(w, f, IID_WATER_SPURT, startX, startY, dir, 1.0, 1){
        setVisible(true);
        hit = false;
        hitpoints = SQUIRT_HEALTH;
    }
    virtual ~Squirt();
    virtual void doSomething();
    virtual int getPoints(){ return 0; }
    virtual int getSound(){ return SOUND_PROTESTER_ANNOYED; }
    bool hitSomeone(){ return hit; }
    void changeState(int x, int y, int size);
private:
    int hitpoints;
    bool hit;
};

#endif /* Squirt_h */
