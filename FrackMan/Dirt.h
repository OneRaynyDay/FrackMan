//
//  Dirt.h
//  FrackMan
//
//  Created by hongshuhong on 2/14/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

/* PLEASE REFER TO PG 52 OF SPEC */

#ifndef Dirt_h
#define Dirt_h

// It must have an Image ID of IID_DIRT
#include "Actor.h"
#include "GameConstants.h"


class Dirt : public Actor{
static constexpr double DIRT_SIZE = 0.25;
static const int DIRT_DEPTH = 3;
public:
    // It must have a simple constructor and destructor that initialize a
    // new Dirt object.
    Dirt(int startX, int startY) : Actor(IID_DIRT, startX, startY, right, DIRT_SIZE, DIRT_DEPTH){
        setVisible(true);
    };
    virtual ~Dirt(){};
    virtual bool isBlock(){ return true; }
    virtual bool isStrong(){
        return true; //can't be bribed
    }
    /* We don't need these. We have a separate vector of Dirt */
    virtual void consume(){};
    virtual int getSound(){ return SOUND_NONE; }
    virtual int getPoints(){ return 0; }
    virtual void setDead(){};
    virtual bool isDead() const {return false;};
    virtual void doSomething(){}; // doesn't need to do anything
private:
    // You may add any set of public/private methods and private
    // member variables to your Dirt class as you see fit, so long as you
    // use good object oriented programming style (e.g., you must not duplicate functionality across classes)
};

#endif /* Dirt_h */
