//
//  Pool.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Pool.h"

/*
 When it is first created:
 1. All Water Pools must have an image ID of IID_WATER_POOL.
 2. All Water Pools must have their x,y location specified for them when they are
 created.
 3. All Water Pools must start off facing rightward.
 4. All Water Pools start out visible.
 5. A Water Pool is only pickup-able by the FrackMan.
 6. A Water Pool will always start out in a temporary state (where they will only
 remain in the oil field for a limited number of ticks before disappearing) – the number of ticks T that a Water Pool will exist can be determined from the following formula:
 T = min(100, 300 – 10*current_level_number)
 7. Water Pools have the following graphic parameters:
 a. They have an image depth of 2 – behind actors like Protesters, but above
 Dirt
 b. They have a size of 1.0
 */

Pool::Pool(StudentWorld* w, FrackMan* f, int startX, int startY) : Item(w, f, IID_WATER_POOL, startX, startY){
    setVisible(true);
    hitpoints = min(100, 300 - 10*w->getLevel());
}

/*
 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
 2. Otherwise, if the Water pool is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Water pool will activate, and:
 e. The Water pool must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
 f. The Water pool must play a sound effect to indicate that the FrackMan picked up the Goodie: SOUND_GOT_GOODIE.
 g. The Water pool must tell the FrackMan object that it just received 5 new squirts of water so it can update its inventory.
 h. The Water pool increases the player’s score by 100 points (This increase can be performed by the FrackMan class or the Sonar Kit class).
 3. Since the Pool of Water is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
 */

void Pool::doSomething(){
    if(isDead())
        return;
    hitpoints--;
    if(hitpoints <= 0){
        consume();
    }
    if(getWorld()->checkDiscoveredFrackMan(this)){
        getPlayer()->increaseWater(5);
    }
}