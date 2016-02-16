//
//  Sonar.h
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Sonar_h
#define Sonar_h

#include "Actor.h"
#include "FrackMan.h"
#include <cmath>

class Sonar : public Item{
    /* When it is first created:
    1. All Sonar Kits must have an image ID of IID_SONAR.
    2. All Sonar Kits must have their x,y location specified for them when they are
        created.
    3. All Sonar Kits must start off facing rightward.
    4. All Sonar Kits starts out visible.
    5. A Sonar Kit is only pickup-able by the FrackMan.
    6. A Sonar Kit will always start out in a temporary state (where they will only
                                                               remain in the oil field for a limited number of ticks before disappearing) – the number of ticks T a Sonar Kit will exist can be determined from the following formula:
    T = min(100, 300 – 10*current_level_number)
    7. Sonar Kits have the following graphic parameters:
    a. They have an image depth of 2 – behind actors like Protesters, but above
    Dirt
    b. They have a size of 1.0
    In addition to any other initialization that you decide to do in your Sonar Kit class, a Sonar Kit object must set itself to be visible using the GraphObject class’s setVisible() method, e.g.:
        setVisible(true); */
static const int INIT_HITPOINTS = 100;
static const int DIRT_ROWS = 60;
static const int SCORE_INCREASE = 75;
public:
    Sonar(StudentWorld* w, FrackMan* f);
    virtual ~Sonar();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
private:
    int hitpoints;
};
#endif /* Sonar_h */
