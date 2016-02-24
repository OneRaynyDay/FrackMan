//
//  HardcoreProtester.h
//  FrackMan
//
//  Created by hongshuhong on 2/24/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef HardcoreProtester_h
#define HardcoreProtester_h
#include "Actor.h"
#include "StudentWorld.h"
#include "Protester.h"

class HardcoreProtester : public Protester{
    static const int INIT_HITPOINTS = 20;
public:
    /* 1. Each Hardcore Protester must have an image ID of IID_HARD_CORE_PROTESTER.
    2. Each Hardcore Protester must start out facing left.
    3. Each Hardcore Protester must decide how many squares,
    numSquaresToMoveInCurrentDirection, it will decide to move left before possibly switching its direction. This value must be: 8 <= numSquaresToMoveInCurrentDirection <= 60
    4. Each Hardcore Protester starts out with 20 hit-points.
    5. Each Hardcore Protester starts out NOT in a leave-the-oil-field state.
    6. Each Hardcore Protester have the following graphic parameters:
    a. It has an image depth of 0 – they’re always in the foreground
    b. It has a size of 1.0 */
    HardcoreProtester(StudentWorld* w, int startX, int startY) : Protester(w, startX, startY, IID_HARD_CORE_PROTESTER, INIT_HITPOINTS){};
    
private:
};

#endif /* HardcoreProtester_h */
