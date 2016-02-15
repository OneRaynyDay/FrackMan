//
//  FrackMan.h
//  FrackMan
//
//  Created by hongshuhong on 2/14/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

/* PLEASE REFER TO PG 52 & 53 OF SPEC */

#ifndef FrackMan_h
#define FrackMan_h

// It must have an Image ID of IID_PLAYER
#include "Actor.h"
#include "GameConstants.h"

/* The fact that it depends on Actor is TENTATIVE. we might change this! */
// It (or its base class) must make itself visible via a call to setVisible(true);
class FrackMan : public Actor{
public:
    // It must have a simple constructor and destructor that
    // initializes/deinitializes the FrackMan – see the FrackMan section
    // for more details on where to initially place the FrackMan on the screen.
    
    // from pg 28: The FrackMan must always start at location x=30, y=60
    FrackMan() : Actor(IID_PLAYER, START_X, START_Y){};
    ~FrackMan();
    
    /* It must have a limited version of a doSomething() method that lets
    the user pick a direction by hitting a directional key. If the player
    hits a directional key during the current tick, it updates the
    FrackMan’s location to the target square, removing any Dirt
    objects that overlap with the FrackMan’s 4x4 graphic image. To
    move the FrackMan, all this doSomething() method has to do is
    properly adjust the player’s X,Y coordinates using GraphObject’s
    moveTo() method and our graphics system will automatically
    animate its movement it around the oil field!
     */
    virtual void doSomething();
    
private:
    const int START_Y = 60;
    const int START_X = 30;
    // You may add any public/private methods and private member
    // variables to your player class as you see fit, so long as you use
    // good object oriented programming style (e.g., you must not duplicate functionality across classes).
    // The FrackMan, in its default state:
};


#endif /* FrackMan_h */
