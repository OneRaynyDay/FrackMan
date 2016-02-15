//
//  Barrel.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Barrel.h"
/* The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
 2. Otherwise, if the Barrel is not currently visible AND the FrackMan is within a radius of 4.0 of it (<= 4.00 units away), then:
 a. The Barrel must make itself visible with the setVisible() method.
 b. The Barrel’s doSomething() method must immediately return.
 3. Otherwise, if the Barrel is within a radius of 3.0 (<= 3.00 units away) from the
 FrackMan, then the Barrel will activate, and:
 a. The Barrel must set its state to dead (so that it will be removed by your
 StudentWorld class from the game at the end of the current tick).
 b. The Barrel must play a sound effect to indicate that the player picked up the
 Goodie: SOUND_FOUND_OIL.
 c. The Barrel must increase the player’s score by 1000 points.
 d. If necessary, the Barrel may inform the StudentWorld object that it was
 picked up. Once all Barrels on the level have been picked up, the player finishes the level and may advance to the next level. */

void Barrel::doSomething(){
    int dist = getWorld()->dist(getX(), getY(), getPlayer()->getX(), getPlayer()->getY());
    if(!isDiscovered()){
        if(dist <= 4){
            setDiscovered();
            return;
        }
    }
    if(dist <= 3){
        consume();
        getWorld()->playSound(SOUND_DIG);
        getPlayer()->increasePoints(SCORE_INCREASE);
        getWorld()->decreaseBarrels();
    }
    
}