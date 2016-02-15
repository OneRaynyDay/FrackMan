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
public:
    // It must have a simple constructor and destructor that initialize a
    // new Dirt object.
    Dirt(int startX, int startY) : Actor(IID_DIRT, startX, startY){};
    ~Dirt();
private:
    // You may add any set of public/private methods and private
    // member variables to your Dirt class as you see fit, so long as you
    // use good object oriented programming style (e.g., you must not duplicate functionality across classes)
};

#endif /* Dirt_h */
